#ifndef DIFFIE_HELLMAN_TLS_HASH_VALIDATOR_H
#define DIFFIE_HELLMAN_TLS_HASH_VALIDATOR_H

#include <memory>
#include <vector>

class HashValidator {
public:
    static std::vector<uint8_t> generate_hash(const std::vector<uint8_t> &data);

    static bool validate_hash(const std::vector<uint8_t> &data, const std::vector<uint8_t> &hash);
};

#endif
