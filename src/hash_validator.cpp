#include <hash_validator.h>
#include <cryptopp/sha.h>

std::vector<uint8_t>
HashValidator::generate_hash(
        const std::vector<uint8_t> &data
) {
    auto output = std::vector<uint8_t>(CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SHA256().CalculateDigest(output.data(), data.data(), data.size());

    return output;
}

bool HashValidator::validate_hash(
        const std::vector<uint8_t> &data,
        const std::vector<uint8_t> &hash
) {
    return CryptoPP::SHA256().VerifyDigest(hash.data(), data.data(), data.size());
}
