#ifndef DIFFIE_HELLMAN_TLS_DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_TLS_DIFFIE_HELLMAN_H

#include <cstdint>

class DiffieHellman {
private:
    uint64_t _p;
    uint64_t _g;
    uint64_t _private_key{};
    uint64_t _public_key{};
    uint64_t _shared_secret{};

    uint64_t fast_powmod(uint64_t x, uint64_t n, uint64_t m);

public:
    DiffieHellman() = default;

    DiffieHellman(uint64_t p, uint64_t g) : _p(p), _g(g) {}

    DiffieHellman(const DiffieHellman &copy) : _p(copy._p), _g(copy._g) {}

    void operator=(const DiffieHellman &copy) {
        _p = copy._p;
        _g = copy._g;
    }

    void generate_private_key();

    void generate_public_key();

    void generate_shared_secret(uint64_t shared_public_key);

    [[nodiscard]] uint64_t get_public_key() const;

    [[nodiscard]] uint64_t get_shared_secret() const;
};

#endif
