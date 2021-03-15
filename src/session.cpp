#include <session.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errors.h>
#include <control.h>

void Session::serve() {
    sockaddr_in server_addr{};
    sockaddr_in client_addr{};
    socklen_t sock_addr_size{};

    int server_sock = socket(PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = inet_addr(_ip_addr.c_str());

    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(server_sock, (sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        throw Error("Failed to bind to socket at: " + std::string(_ip_addr));
    }

    if (listen(server_sock, 1) != 0) {
        throw Error("Failed to listen to socket");
    }

    sock_addr_size = sizeof(client_addr);
    _sock = accept(server_sock, (sockaddr *) &client_addr, &sock_addr_size);

    char *ip_addr = new char[INET_ADDRSTRLEN + 1];
    ip_addr[INET_ADDRSTRLEN] = '\0';

    inet_ntop(AF_INET, &client_addr.sin_addr, ip_addr, INET_ADDRSTRLEN);
}

void Session::connect() {
    sockaddr_in server_address{};
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw Error("Failed to create socket");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(_port);

    if (inet_pton(AF_INET, _ip_addr.c_str(), &server_address.sin_addr) <= 0) {
        throw Error("Invalid address");
    }

    if (::connect(_sock, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        throw Error("Connection failed");
    }
}

void Session::t_send(const void *buf, size_t n) const {
    if (send(_sock, buf, n, 0) < n) {
        throw Error("Insufficient data sent");
    }
}

void Session::t_recv(void *buf, size_t n) const {
    if (recv(_sock, buf, n, MSG_WAITALL) < n) {
        throw Error("Insufficient data received");
    }
}

void Session::t_close() {
    close(_sock);
}
