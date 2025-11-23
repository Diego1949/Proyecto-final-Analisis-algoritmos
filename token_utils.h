// token_utils.h
#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <string>

// Clave secreta compartida entre cliente y servidor
extern const std::string SECRET_KEY;

// Hash sencillo (djb2) para generar el código base del token
int simpleHash(const std::string& s);

// Genera el token dinámico actual (6 dígitos) basado en SECRET_KEY + tiempo
std::string generarTokenActual();

// Devuelve la fecha/hora actual como string "YYYY-MM-DD HH:MM:SS"
std::string obtenerFechaHoraActual();

#endif // TOKEN_UTILS_H
