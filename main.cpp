#include <iostream>
#include <filesystem>
#include <argparse.h>
#include <session.h>
#include <errors.h>
#include <protocol.h>
#include "menu.h"

int main(int argc, const char *argv[]) {
    argparse::ArgumentParser parser("Diffie-Hellman-TLS CLI", "A simple CLI to communicate using Diffie-Hellman-TLS");

    parser.add_argument()
            .names({"-s", "--server"})
            .description("Enable server mode")
            .required(false);

    parser.add_argument()
            .names({"-c", "--client"})
            .description("Enable client mode")
            .required(false);

    parser.add_argument()
            .names({"-a", "--address"})
            .description("Provide IP address")
            .required(false);

    parser.add_argument()
            .names({"-p", "--port"})
            .description("Provide port")
            .required(false);

    parser.enable_help();

    auto err = parser.parse(argc, argv);
    if (err) {
        std::cerr << err << std::endl;
        return 1;
    }

    if (parser.exists("help")) {
        parser.print_help();
        return 0;
    }

    std::string ip_addr;
    uint16_t port;
    bool is_server = false;

    if (parser.exists("a") && parser.exists("p")) {
        ip_addr = parser.get<std::string>("a");
        port = parser.get<uint16_t>("p");
    } else {
        std::cerr << "Expected IP address and port" << std::endl;
        return 1;
    }


    Session session(ip_addr, port);

    if (parser.exists("s")) {
        is_server = true;
        std::cout << "[MAIN] Waiting for connection..." << std::endl;
        session.serve();
        std::cout << "[MAIN] Proceeding with key-exchange protocol..." << std::endl;

    } else if (parser.exists("c")) {
        session.connect();
        std::cout << "[MAIN] Proceeding with key-exchange protocol..." << std::endl;
    } else {
        std::cerr << "Expected communication mode" << std::endl;
        return 1;
    }

    ProtocolEngine engine(session);

    engine.send_public_key();
    engine.create_shared_secret();

    std::cout << "[MAIN] Key-exchange protocol successful!" << std::endl;

    auto input_mutex = std::make_shared<std::mutex>();
    auto output_mutex = std::make_shared<std::mutex>();

    auto message_callback = [&output_mutex](const std::string &msg) {
        std::scoped_lock<std::mutex> lock(*output_mutex);
        std::cout << "[MESSAGE]: " << msg << std::endl;
    };

    auto file_callback = [&input_mutex, &output_mutex](const std::string &filename, const std::vector<uint8_t> &data) {
        std::scoped_lock<std::mutex, std::mutex> lock(*input_mutex, *output_mutex);
        std::cout << "[FILEDATA] " << filename << " - Confirm download (y/n): ";

        char c;
        std::cin >> c;

        if (c != 'y') {
            return;
        }

        std::cout << "[FILEDATA] " << "Enter file path: ";

        std::string target_path;
        std::cin >> target_path;

        if (std::filesystem::exists(target_path)) {
            std::cerr << "Target path is not empty" << std::endl;
            return;
        }

        auto file = std::fopen(target_path.c_str(), "wb");

        std::cout << "[FILEDATA] " << "Writing to " << target_path << std::endl;
        std::fwrite(data.data(), 1, data.size(), file);
        std::cout << "[FILEDATA] " << "File saved to " << target_path << " successfully" << std::endl;

        std::fclose(file);
    };

    if (!is_server) {
        MenuCLI menu_cli(output_mutex, input_mutex, engine);
        menu_cli.start();
    } else {
        std::cout << "[MAIN] Listening on address: " << ip_addr << ":" << port << std::endl;
        engine.start_listener(message_callback, file_callback);

        std::mutex dummy;
        dummy.lock();
        dummy.lock();
    }

    return 0;
}
