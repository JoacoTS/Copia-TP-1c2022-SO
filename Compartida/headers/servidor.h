/*
 * servidor.h
 *
 *  Created on: 9 abr. 2022
 *      Author: utnso
 */

#ifndef HEADERS_SERVIDOR_H_
#define HEADERS_SERVIDOR_H_

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <string.h>
#include <stdlib.h>

int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);
int esperar_cliente(t_log* logger, const char* name, int socket_servidor);
int crear_conexion(t_log* logger, const char* server_name, char* ip, char* puerto);
void liberar_conexion(int* socket_cliente);

#endif /* HEADERS_SERVIDOR_H_ */
