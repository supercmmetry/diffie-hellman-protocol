#ifndef DIFFIE_HELLMAN_TLS_MENU_H
#define DIFFIE_HELLMAN_TLS_MENU_H

#include <mutex>
#include <protocol.h>

class MenuCLI {
private:
    enum Context {
        BASE = 0,
        MESSAGE = 1,
        FILEDATA = 2
    };

    ProtocolEngine _engine;
    Context _ctx = BASE;
    std::shared_ptr<std::mutex> _io_output_mutex;
    std::shared_ptr<std::mutex> _io_input_mutex;
public:
    explicit MenuCLI(
            const std::shared_ptr<std::mutex> &io_output_mutex,
            const std::shared_ptr<std::mutex> &io_input_mutex,
            const ProtocolEngine &engine
    );

    [[noreturn]] void start();
};

#endif
