#include <encryption.h>
#include <cryptopp/secblock.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

EncryptionCodec::EncryptionCodec(uint64_t shared_secret) {
    auto raw_secret = reinterpret_cast<uint8_t *>(&shared_secret);
    for (uint8_t i = 0; i < 8; i++) {
        _key[i] = raw_secret[i];
        _key[i + 8] = raw_secret[i];
    }
}

std::vector<uint8_t> EncryptionCodec::encrypt(const std::vector<uint8_t> &data) {
    CryptoPP::SecByteBlock key_iv(16);
    key_iv.Assign(_key, 16);

    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryptor(key_iv, key_iv.size(), key_iv);
    auto output = std::vector<uint8_t>(data.size());
    encryptor.ProcessData(output.data(), data.data(), data.size());

    return output;
}

std::vector<uint8_t> EncryptionCodec::decrypt(const std::vector<uint8_t> &data) {
    CryptoPP::SecByteBlock key_iv(16);
    key_iv.Assign(_key, 16);

    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryptor(key_iv, key_iv.size(), key_iv);
    auto output = std::vector<uint8_t>(data.size());
    decryptor.ProcessData(output.data(), data.data(), data.size());

    return output;
}
