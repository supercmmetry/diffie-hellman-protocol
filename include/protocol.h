#ifndef DIFFIE_HELLMAN_TLS_PROTOCOL_H
#define DIFFIE_HELLMAN_TLS_PROTOCOL_H

#include <session.h>
#include <functional>
#include <diffie_hellman.h>
#include <encryption.h>

class ProtocolEngine {
private:
    Session _session;
    DiffieHellman _dhman;
    EncryptionCodec _codec;

public:
    explicit ProtocolEngine(const Session &session);

    void send_public_key();

    void create_shared_secret();

    void send_message(const std::string &msg);

    void send_file(
            const std::string &filename,
            const std::vector<uint8_t> &data
    );

    void start_listener(
            const std::function<void(const std::string &msg)> &text_callback,
            const std::function<void(
                    const std::string &filename,
                    const std::vector<uint8_t> &data
            )> &file_callback
    );
};

#endif
