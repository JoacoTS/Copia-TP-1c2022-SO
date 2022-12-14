/*
 ============================================================================
 Name        : MóduloA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../include/main.h"

int main(int argc, char** argv) {

    if(argc < 3) {
        printf("Cantidad de argumentos insuficientes \n");
        return EXIT_FAILURE;
    }

	printf("Path del archivo: %s \n", argv[1]);
    printf("Tamaño del proceso: %s \n", argv[2]); 

    iniciar_config();
    iniciar_logger();

    int conexion_kernel = crear_conexion(logger_consola, "KERNEL", ip_kernel, puerto_kernel);

    t_paquete *codigo = crear_paquete();
    codigo->codigo_operacion = MENSAJE;

    codigo = leer_archivo(argv[1]);
    agregar_a_paquete(codigo, (void*)(argv[2]), sizeof(int));

    enviar_paquete(codigo, conexion_kernel, logger_consola);

    eliminar_paquete(codigo);

    close(conexion_kernel);

    /*--------------------------------------*/

    log_destroy(logger_consola);
}
