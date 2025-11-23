// server.cpp
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#include "token_utils.h"

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

std::string trim(const std::string& s) {
    const char* ws = " \t\n\r";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    const int PORT = 5000;

    // Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error: no se pudo crear el socket\n";
        return 1;
    }

    // Reusar puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)) < 0) {
        std::cerr << "Error: setsockopt\n";
        close(server_fd);
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error: bind\n";
        close(server_fd);
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error: listen\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Servidor escuchando en puerto " << PORT << "..." << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            std::cerr << "Error al aceptar conexion\n";
            continue;
        }

        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, sizeof(buffer) - 1);
        if (valread <= 0) {
            close(new_socket);
            continue;
        }

        std::string received(buffer, valread);
        received = trim(received);

        std::cout << "\n==== Nueva transaccion recibida ====\n";
        std::cout << "Raw: " << received << std::endl;

        auto parts = split(received, '|');
        if (parts.size() != 6) {
            std::cerr << "Error: formato de transaccion invalido\n";
            std::string msg = "ERROR_FORMATO";
            send(new_socket, msg.c_str(), msg.size(), 0);
            close(new_socket);
            continue;
        }

        std::string transaction_id = parts[0];
        std::string datetime = parts[1];
        std::string from_acc = parts[2];
        std::string to_acc = parts[3];
        std::string amount = parts[4];
        std::string token_recibido = trim(parts[5]);

        std::string token_esperado = generarTokenActual();

        std::cout << "ID: " << transaction_id << "\n";
        std::cout << "Fecha/Hora: " << datetime << "\n";
        std::cout << "Cuenta origen: " << from_acc << "\n";
        std::cout << "Cuenta destino: " << to_acc << "\n";
        std::cout << "Valor: " << amount << "\n";
        std::cout << "Token recibido: " << token_recibido << "\n";
        std::cout << "Token esperado: " << token_esperado << "\n";

        std::string respuesta;
        if (token_recibido == token_esperado) {
            respuesta = "OK";
            std::cout << "Resultado: TOKEN VALIDO, transaccion aceptada.\n";
        } else {
            respuesta = "ERROR_TOKEN";
            std::cout << "Resultado: TOKEN INVALIDO, transaccion rechazada.\n";
        }

        send(new_socket, respuesta.c_str(), respuesta.size(), 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
