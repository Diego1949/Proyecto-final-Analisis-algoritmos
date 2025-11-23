# Proyecto-final-Analisis-algoritmos
# Sistema de Transacciones con Token Dinámico (Entrega Final)

Este proyecto implementa un sistema simple de transacciones entre un **cliente** y un **servidor** bancario usando **C++** y **Docker**.  
La seguridad se basa en un **token dinámico de 6 dígitos**, similar a los tokens de bancos.

## Arquitectura

La solución se compone de **dos contenedores Docker**:

- `token_server`: Servidor bancario que recibe transacciones y **valida el token**.
- `token_client`: Cliente que genera y envía transacciones con el **token dinámico**.

Los contenedores se comunican por **sockets TCP** sobre una red interna de Docker.

Diagrama conceptual:

Cliente (`token_client`) → [socket TCP puerto 5000] → Servidor (`token_server`)

## Formato de la transacción

Las transacciones se envían como una línea de texto con campos separados por `|`:

```txt
transaction_id|date_time|from_account|to_account|amount|token
