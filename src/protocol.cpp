#include <protocol.h>
#include <thread>
#include <sstream>
#include <iostream>
#include <control.h>
#include <errors.h>
#include <hash_validator.h>

ProtocolEngine::ProtocolEngine(const Session &session) : _session(session) {
    _dhman = DiffieHellman(99991, 1024);
    _dhman.generate_private_key();
    _dhman.generate_public_key();
}

void ProtocolEngine::send_public_key() {
    uint8_t word = ControlWord::EXCHANGE;
    uint64_t public_key = _dhman.get_public_key();

    _session.t_send(&word, sizeof(word));
    _session.t_send(&public_key, sizeof(public_key));
}

void ProtocolEngine::create_shared_secret() {
    uint8_t word = 0xff;
    uint64_t other_pkey = 0;

    _session.t_recv(&word, sizeof(word));

    if (word == ControlWord::EXCHANGE) {
        _session.t_recv(&other_pkey, sizeof(other_pkey));
    } else {
        throw Error("Invalid protocol sequence");
    }

    std::cout << "[MAIN] Received public key: " << "0x" << std::hex << other_pkey << std::endl;

    _dhman.generate_shared_secret(other_pkey);
    std::cout << "[MAIN] Generated shared secret: " << "0x" << std::hex << _dhman.get_shared_secret() << std::endl;

    _codec = EncryptionCodec(_dhman.get_shared_secret());
}

void ProtocolEngine::send_message(const std::string &msg) {
    uint8_t word = ControlWord::TEXT;

    _session.t_send(&word, sizeof(word));

    auto buffer = std::vector<uint8_t>(msg.length());
    for (uint64_t i = 0; i < msg.length(); i++) {
        buffer[i] = msg[i];
    }

    auto cipher = _codec.encrypt(buffer);

    uint64_t length = cipher.size();
    _session.t_send(&length, sizeof(length));
    _session.t_send(cipher.data(), length);

    auto hash = HashValidator::generate_hash(buffer);
    uint64_t hash_size = hash.size();

    _session.t_send(&hash_size, sizeof(hash_size));
    _session.t_send(hash.data(), hash_size);
}

void ProtocolEngine::send_file(const std::string &filename, const std::vector<uint8_t> &data) {
    uint8_t word = ControlWord::FILEDATA;

    _session.t_send(&word, sizeof(word));

    auto buffer = std::vector<uint8_t>(filename.length());
    for (uint64_t i = 0; i < filename.length(); i++) {
        buffer[i] = filename[i];
    }

    auto cipher = _codec.encrypt(buffer);
    uint64_t cipher_length = cipher.size();

    _session.t_send(&cipher_length, sizeof(cipher_length));
    _session.t_send(cipher.data(), cipher_length);

    cipher = _codec.encrypt(data);
    cipher_length = cipher.size();

    _session.t_send(&cipher_length, sizeof(cipher_length));
    _session.t_send(cipher.data(), cipher_length);

    auto hash = HashValidator::generate_hash(data);
    uint64_t hash_size = hash.size();

    _session.t_send(&hash_size, sizeof(hash_size));
    _session.t_send(hash.data(), hash_size);
}

void ProtocolEngine::start_listener(
        const std::function<void(const std::string &)> &text_callback,
        const std::function<void(
                const std::string &,
                const std::vector<uint8_t> &
        )> &file_callback
) {
    std::thread([&]() {
        while (true) {
            try {
                uint8_t word = 0;
                _session.t_recv(&word, sizeof(word));

                switch (word) {
                    case ControlWord::TEXT: {
                        uint64_t length = 0;
                        _session.t_recv(&length, sizeof(length));
                        auto buffer = std::vector<uint8_t>(length);

                        _session.t_recv(buffer.data(), length);

                        auto decoded = _codec.decrypt(buffer);

                        uint64_t hash_size = 0;
                        _session.t_recv(&hash_size, sizeof(hash_size));

                        auto hash = std::vector<uint8_t>(hash_size);
                        _session.t_recv(hash.data(), hash_size);

                        if (!HashValidator::validate_hash(decoded, hash)) {
                            throw Error("Hash mismatch");
                        }

                        std::stringstream stream;

                        for (uint64_t i = 0; i < length; i++) {
                            stream << decoded[i];
                        }

                        std::string message = stream.str();
                        text_callback(message);
                        break;
                    }
                    case ControlWord::FILEDATA: {
                        uint64_t length = 0;
                        _session.t_recv(&length, sizeof(length));
                        auto buffer = std::vector<uint8_t>(length);

                        _session.t_recv(buffer.data(), length);

                        auto decoded = _codec.decrypt(buffer);

                        uint64_t hash_size = 0;
                        _session.t_recv(&hash_size, sizeof(hash_size));

                        auto hash = std::vector<uint8_t>(hash_size);
                        _session.t_recv(hash.data(), hash_size);

                        if (!HashValidator::validate_hash(decoded, hash)) {
                            throw Error("Hash mismatch");
                        }

                        std::stringstream stream;

                        for (uint64_t i = 0; i < length; i++) {
                            stream << decoded[i];
                        }

                        std::string filename = stream.str();

                        _session.t_recv(&length, sizeof(length));
                        buffer = std::vector<uint8_t>(length);

                        _session.t_recv(buffer.data(), length);

                        decoded = _codec.decrypt(buffer);

                        _session.t_recv(&hash_size, sizeof(hash_size));

                        hash = std::vector<uint8_t>(hash_size);
                        _session.t_recv(hash.data(), hash_size);

                        if (!HashValidator::validate_hash(decoded, hash)) {
                            throw Error("Hash mismatch");
                        }

                        file_callback(filename, decoded);
                        break;
                    }
                    default:
                        throw Error("Unexpected control word encountered");
                }

            } catch (std::exception &e) {
                std::cerr << "[ERROR]: " << e.what() << std::endl;
                exit(1);
            }
        }
    }).detach();
}
