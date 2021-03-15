#ifndef DIFFIE_HELLMAN_TLS_SESSION_H
#define DIFFIE_HELLMAN_TLS_SESSION_H

#include <cstdint>
#include <string>
#include <memory>

class Session {
private:
    int _sock{};
    std::string _ip_addr{};
    uint16_t _port{};

public:
    Session(const std::string &ip_addr, uint16_t port) : _ip_addr(ip_addr), _port(port) {}

    Session(const Session &copy) = default;

    void serve();

    void connect();

    void t_send(const void *buf, size_t n) const;

    void t_recv(void *buf, size_t n) const;

    void t_close();


};

#endif
