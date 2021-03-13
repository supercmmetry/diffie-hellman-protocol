#include <diffie_hellman.h>
#include <random>

uint64_t DiffieHellman::fast_powmod(uint64_t x, uint64_t n, uint64_t m) {
    if (n == 0) {
        return 1;
    }

    if (n == 1) {
        return x % m;
    }

    uint64_t half_result = fast_powmod(x, n >> 1, m);

    if ((n & 1) == 0) {
        return half_result * half_result;
    } else {
        return half_result * half_result * (x % m);
    }
}

void DiffieHellman::generate_private_key() {
    static std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<uint64_t> dist(0xffffff, 0xffffffffffffffff);
    _private_key = dist(mt);
}

void DiffieHellman::generate_public_key() {
    _public_key = fast_powmod(_g, _private_key, _p);
}

void DiffieHellman::generate_shared_secret(uint64_t shared_public_key) {
    _shared_secret = fast_powmod(shared_public_key, _private_key, _p);
}

uint64_t DiffieHellman::get_public_key() const {
    return _public_key;
}

uint64_t DiffieHellman::get_shared_secret() const {
    return _shared_secret;
}


