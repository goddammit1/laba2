#include <iostream>
#include "boost/asio.hpp"

using namespace boost::asio;
using ip::tcp;

const std::string RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 5\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "pong";

int main() {
    const int PORT = 8080;

    try {
        io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT));
        std::cout << "Server started on port " << PORT << std::endl;

        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            // Читаем запрос (минимально)
            boost::asio::streambuf buffer;
            read_until(socket, buffer, "\r\n\r\n");

            // Отправляем ответ
            write(socket, boost::asio::buffer(RESPONSE));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}



