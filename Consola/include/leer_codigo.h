#ifndef LEER_CODIGO
#define LEER_CODIGO

#include <stdlib.h>
#include <commons/string.h>
#include "../../shared/include/shared_mensajes.h"

#define LARGO_LINEA_MAX 20

char* leer_instruccion(FILE *f);
t_paquete* leer_archivo(char *path);

#endif