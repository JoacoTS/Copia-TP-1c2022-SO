#include "../include/main.h"

#define CONFIG_FILE_PATH "./cfg/Consola.config"
#define LOG_FILE_PATH "./cfg/Consola.log"
#define NOMBRE_MODULO "Consola"

int main(int argc, char** argv) {

    t_config* config_consola;
    t_log* logger = iniciar_logger(LOG_FILE_PATH, NOMBRE_MODULO);

    if(argc < 3) {
        printf("Cantidad de argumentos insuficientes \n");
        return EXIT_FAILURE;
    }
    
    // Path del archivo: argv[1] && Tamaño del proceso: argv[2]
    log_info(logger, "Módulo Consola iniciado");

    config_consola = iniciar_config(CONFIG_FILE_PATH);
    procesar_archivo_config_consola(config_consola);

    int conexion_kernel = crear_conexion(logger, "KERNEL", ip_kernel, puerto_kernel);

    t_paquete* codigo = leer_archivo(argv[1]);
    agregar_a_paquete(codigo, (void*)(argv[2]), sizeof(int) + 1);

    enviar_paquete(codigo, conexion_kernel, logger);

    eliminar_paquete(codigo);

    int op_code = recibir_operacion(conexion_kernel);
    recibir_mensaje(conexion_kernel, logger);

    close(conexion_kernel);

    /*--------------------------------------*/

    finalizar_programa(logger, config_consola);
}
