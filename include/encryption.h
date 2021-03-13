#ifndef DIFFIE_HELLMAN_TLS_ENCRYPTION_H
#define DIFFIE_HELLMAN_TLS_ENCRYPTION_H

#include <cstdint>
#include <memory>

class EncryptionCodec {
private:
    uint8_t _key[16]{};

public:
    explicit EncryptionCodec(uint64_t shared_secret);

    std::shared_ptr<uint8_t> encrypt(const std::shared_ptr<uint8_t> &data, uint64_t length);

    std::shared_ptr<uint8_t> decrypt(const std::shared_ptr<uint8_t> &data, uint64_t length);
};

#endif
