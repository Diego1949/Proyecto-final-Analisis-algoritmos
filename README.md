# Sistema de Transacciones Bancarias Seguras con Token Dinámico (TOTP)



Este proyecto implementa un sistema de validación de transacciones distribuidas utilizando **contenedores Docker ligeros** y **C++**. El sistema simula la seguridad bancaria mediante claves dinámicas (Tokens) que cambian cada 30 segundos, eliminando la necesidad de enviar contraseñas a través de la red.



## Características Principales



* **Arquitectura Microservicios:** Cliente y Servidor totalmente desacoplados.

* **Seguridad:** Implementación de algoritmo TOTP (Time-based One-Time Password).

* **Eficiencia:** Contenedores optimizados de **~10MB** gracias a la compilación estática en C++ (Alpine Linux).

* **Despliegue:** Imágenes alojadas en Docker Hub para ejecución inmediata.



## Arquitectura y Seguridad



El sistema consta de dos contenedores comunicados vía Sockets TCP.



**Diagrama de Secuencia:**



```mermaid

sequenceDiagram

    participant Usuario

    participant Cliente (C++)

    participant Servidor (C++)



    Note over Cliente,Servidor: Comparten SECRET_KEY (Nunca viaja por la red)



    Usuario->>Cliente: Ingresa ID, Cuentas, Monto

    Cliente->>Cliente: Calcula Hash(SECRET_KEY + Tiempo/30s)

    Cliente->>Servidor: Envía Transacción + Token (ej. 540923)

    

    Note right of Servidor: Recibe solicitud

    

    Servidor->>Servidor: Genera su propio Token Local

    Servidor->>Servidor: Compara (Token Recibido == Token Local)

    

    alt Tokens Coinciden

        Servidor-->>Cliente: Responde "OK"

    else No Coinciden

        Servidor-->>Cliente: Responde "ERROR_TOKEN"

    end
