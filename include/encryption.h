#ifndef DIFFIE_HELLMAN_TLS_ENCRYPTION_H
#define DIFFIE_HELLMAN_TLS_ENCRYPTION_H

#include <cstdint>
#include <memory>
#include <vector>

class EncryptionCodec {
private:
    uint8_t _key[16]{};

public:
    EncryptionCodec() = default;

    explicit EncryptionCodec(uint64_t shared_secret);

    std::vector<uint8_t> encrypt(const std::vector<uint8_t> &data);

    std::vector<uint8_t> decrypt(const std::vector<uint8_t> &data);
};

#endif
