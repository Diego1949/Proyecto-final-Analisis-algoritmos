// token_utils.cpp
#include "token_utils.h"

#include <ctime>
#include <cstdio>

const std::string SECRET_KEY = "CLAVE_SUPER_SECRETA_123";

int simpleHash(const std::string& s) {
    unsigned long hash = 5381; // djb2
    for (char c : s) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash & 0x7FFFFFFF);
}

std::string generarTokenActual() {
    std::time_t now = std::time(nullptr);

    // Ventanas de 30 segundos
    long ventana = static_cast<long>(now / 30);

    std::string data = SECRET_KEY + std::to_string(ventana);
    int h = simpleHash(data);

    if (h < 0) h = -h;
    int codigo = h % 1000000; // 6 dígitos

    char buffer[7];
    std::snprintf(buffer, sizeof(buffer), "%06d", codigo);
    return std::string(buffer);
}

std::string obtenerFechaHoraActual() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);

    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    return std::string(buf);
}
