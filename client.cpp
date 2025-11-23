// client.cpp
#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "token_utils.h"

int main() {
    std::string transaction_id, from_acc, to_acc;
    double amount = 0.0;

    std::cout << "=== Cliente de Transaccion ===\n";

    std::cout << "ID de transaccion: ";
    std::cin >> transaction_id;

    std::cout << "Cuenta origen: ";
    std::cin >> from_acc;

    std::cout << "Cuenta destino: ";
    std::cin >> to_acc;

    std::cout << "Valor: ";
    std::cin >> amount;

    std::string datetime = obtenerFechaHoraActual();
    std::string token = generarTokenActual();

    std::string mensaje = transaction_id + "|" + datetime + "|" +
                          from_acc + "|" + to_acc + "|" +
                          std::to_string(amount) + "|" + token;

    std::cout << "\nTransaccion a enviar:\n" << mensaje << "\n\n";

    int sock = 0;
    struct sockaddr_in serv_addr{};
    const int PORT = 5000;

    // Buscar al servidor por nombre de servicio Docker: "token_server"
    struct hostent* server = gethostbyname("token_server");
    if (!server) {
        std::cerr << "Error: no se pudo resolver host 'token_server'\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creando socket\n";
        return 1;
    }

    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error: conexion fallida al servidor\n";
        close(sock);
        return 1;
    }

    // Enviar la transaccion
    if (send(sock, mensaje.c_str(), mensaje.size(), 0) < 0) {
        std::cerr << "Error enviando datos\n";
        close(sock);
        return 1;
    }

    char buffer[1024] = {0};
    int valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread < 0) {
        std::cerr << "Error leyendo respuesta\n";
        close(sock);
        return 1;
    }

    std::string respuesta(buffer, valread);
    std::cout << "Respuesta del servidor: " << respuesta << std::endl;

    close(sock);
    return 0;
}
