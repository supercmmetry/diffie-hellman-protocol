#ifndef DIFFIE_HELLMAN_TLS_HASH_VALIDATOR_H
#define DIFFIE_HELLMAN_TLS_HASH_VALIDATOR_H

#include <memory>

class HashValidator {
public:
    static std::pair<std::shared_ptr<uint8_t>, uint64_t>
    generate_hash(const std::shared_ptr<uint8_t> &data, uint64_t length);

    static bool
    validate_hash(const std::shared_ptr<uint8_t> &data, uint64_t length, const std::shared_ptr<uint8_t> &hash);
};

#endif
