#include "menu.h"
#include <iostream>

MenuCLI::MenuCLI(
        const std::shared_ptr<std::mutex> &io_output_mutex,
        const std::shared_ptr<std::mutex> &io_input_mutex,
        const ProtocolEngine &engine
) : _io_input_mutex(io_input_mutex), _io_output_mutex(io_output_mutex), _engine(engine) {

}

void MenuCLI::start() {
    while (true) {
        switch (_ctx) {
            case BASE: {
                int ctx;

                {
                    std::scoped_lock<std::mutex, std::mutex> lock(*_io_output_mutex, *_io_input_mutex);
                    std::cout << std::endl;
                    std::cout << "Select context:" << std::endl;
                    std::cout << "<1> MESSAGE" << std::endl;
                    std::cout << "<2> FILEDATA" << std::endl;
                    std::cout << std::endl;

                    std::cin >> ctx;
                }

                if (ctx == MESSAGE || ctx == FILEDATA) {
                    _ctx = (Context) ctx;
                } else {
                    std::cerr << "Invalid context" << std::endl;
                    exit(1);
                }
                break;
            }

            case MESSAGE: {
                std::string msg;

                {
                    std::scoped_lock<std::mutex, std::mutex> lock(*_io_output_mutex, *_io_input_mutex);
                    std::cout << "MESSAGE> ";
                    std::getline(std::cin, msg);
                }
                
                if (msg.length() > 0) {
                    _engine.send_message(msg);
                }
                break;
            }

            case FILEDATA:
                break;
        }
    }
}
