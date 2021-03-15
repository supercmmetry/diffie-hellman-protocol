#ifndef DIFFIE_HELLMAN_TLS_ERRORS_H
#define DIFFIE_HELLMAN_TLS_ERRORS_H

#include <exception>
#include <string>

class Error : public std::exception {
private:
    std::string _msg;
public:
    Error(const std::string &msg) {
        _msg = msg;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return _msg.c_str();
    }
};
#endif
