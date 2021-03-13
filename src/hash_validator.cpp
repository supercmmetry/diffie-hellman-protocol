#include <hash_validator.h>
#include <cryptopp/sha.h>


std::pair<std::shared_ptr<uint8_t>, uint64_t>
HashValidator::generate_hash(
        const std::shared_ptr<uint8_t> &data,
        uint64_t length
) {
    auto output = std::make_shared<uint8_t>(CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SHA256().CalculateDigest(output.get(), data.get(), length);

    return std::pair<std::shared_ptr<uint8_t>, uint64_t>(output, CryptoPP::SHA256::DIGESTSIZE);
}

bool HashValidator::validate_hash(
        const std::shared_ptr<uint8_t> &data,
        uint64_t length,
        const std::shared_ptr<uint8_t> &hash
) {
    return CryptoPP::SHA256().VerifyDigest(hash.get(), data.get(), length);
}
