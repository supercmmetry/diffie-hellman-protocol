#include "menu.h"
#include <iostream>
#include <filesystem>

MenuCLI::MenuCLI(
        const std::shared_ptr<std::mutex> &io_output_mutex,
        const std::shared_ptr<std::mutex> &io_input_mutex,
        const ProtocolEngine &engine
) : _io_input_mutex(io_input_mutex), _io_output_mutex(io_output_mutex), _engine(engine) {

}

[[noreturn]] void MenuCLI::start() {
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
                    std::cout << "BASE> ";

                    std::cin >> ctx;
                }

                if (ctx == MESSAGE || ctx == FILEDATA) {
                    _ctx = (Context) ctx;
                } else {
                    std::cerr << "Invalid context" << std::endl;
                }
                break;
            }

            case MESSAGE: {
                std::string input;

                {
                    std::scoped_lock<std::mutex, std::mutex> lock(*_io_output_mutex, *_io_input_mutex);
                    std::cout << "MESSAGE> ";
                    std::getline(std::cin, input);
                }

                if (input == "exit") {
                    _ctx = BASE;
                    break;
                }

                if (input.length() > 0) {
                    _engine.send_message(input);
                }
                break;
            }

            case FILEDATA: {
                std::string input;

                {
                    std::scoped_lock<std::mutex, std::mutex> lock(*_io_output_mutex, *_io_input_mutex);
                    std::cout << "FILEDATA> ";
                    std::getline(std::cin, input);
                }

                if (input == "exit") {
                    _ctx = BASE;
                    break;
                }

                if (input.length() > 0) {
                    if (std::filesystem::exists(input) && !std::filesystem::is_directory(input)) {
                        auto file_data = std::vector<uint8_t>(std::filesystem::file_size(input));
                        auto file = std::fopen(input.c_str(), "rb");
                        std::fread(file_data.data(), 1, file_data.size(), file);
                        _engine.send_file(input, file_data);
                    } else {
                        std::cerr << "[ERROR] Invalid file" << std::endl;
                    }
                }

                break;
            }
        }
    }
}
