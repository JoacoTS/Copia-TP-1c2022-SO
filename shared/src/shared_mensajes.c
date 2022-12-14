#include "../include/shared_mensajes.h"

//NOTA:
//Los enteros que se envian estan en formato 'int', por ahi hay que cambiarlos a 'uint32_t'

int enviar_num(int coneccion, int num, t_log *logger)
{
    //send() ENVIA POR coneccion EL num Y RETORNA LA CANTIDAD DE BYTES ENVIADOS
	int ret = send(coneccion, &num, sizeof(uint32_t), 0);
	if(ret < sizeof(uint32_t))
		log_error(logger, "Error al enviar numero");

	return ret;
}

int recibir_num(int coneccion, t_log* logger)
{
    //recv() RETORNA -1 SI EN CASO DE ERROR
	int num;
	if(recv(coneccion, &num, sizeof(int), MSG_WAITALL) > 0)
		return num;
	else
	{
        log_error(logger, "Error al recibir numero");
		return -1;
	}
}

/*-------------------------------*/
void* serializar_paquete(t_paquete* paquete, int bytes)
{
	//Inicia puntero vacio
	void * total_paquete = malloc(bytes);
	int desplazamiento = 0;

	//Copia codigo de operacion (la primer parte del mensaje que va a llegar)
	memcpy(total_paquete + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	
	//Copia tamanio del paquete
	memcpy(total_paquete + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	
	//Copia contenido del paquete
	memcpy(total_paquete + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return total_paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	//'Agranda' el paquete para agregar un valor
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	//Copia el tamanio del valor
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	
	//Copia el valor
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	//Actualiza el tamanio del paquete (se agrego dato y tamanio del dato)
	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente, t_log *logger)
{
	//Total mensaje = buffer + tamanio buffer + codigo operacion
	int bytes = paquete->buffer->size + 2*sizeof(int);
	
	//Transforma de t_paquete a *void
	void* a_enviar = serializar_paquete(paquete, bytes);

	//Envia datos
	if(send(socket_cliente, a_enviar, bytes, 0) <= 0)
		log_error(logger, "Datos no se enviaron correctamente");

	//Liberar datos
	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	//Primer eliminar *void del paquete
	free(paquete->buffer->stream);

	//Despues el puntero que guardaba la estructura t_buffer
	free(paquete->buffer);

	//Despues el puntero que guardaba a t_paquete
	free(paquete);
}

void enviar_mensaje(char* mensaje, int socket_cliente, t_log *logger)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//Definir codigo de operacion
	paquete->codigo_operacion = MENSAJE;
	
	//Iniciar estructura buffer del paquete
	paquete->buffer = malloc(sizeof(t_buffer));
	
	//Tamanio = largo mensaje + caracter '\0'
	paquete->buffer->size = strlen(mensaje) + 1;
	
	//Reservar espacio para mensaje
	paquete->buffer->stream = malloc(paquete->buffer->size);
	
	//Copiar mensaje en buffer
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	//Mensaje total = mensaje + tamanio + codigo operacion
	int bytes = paquete->buffer->size + 2*sizeof(int);

	//De paquete a *void
	void* a_enviar = serializar_paquete(paquete, bytes);
	printf("Msg %s \n", (char*)(a_enviar + 2*sizeof(int)));

	int check;
	if( (check = send(socket_cliente, a_enviar, bytes, 0)) <= 0)
		log_error(logger, "Mensaje no se envio correctamente");

	log_info(logger, "Se enviaron %d bytes", check);
	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

/*-----------------------------------*/

void* recibir_buffer(int* size, int socket_cliente, t_log *logger)
{
	void * buffer;

	//Recibir tamanio de buffer
	if(recv(socket_cliente, size, sizeof(int), MSG_WAITALL) <= 0)
		log_error(logger, "Error al recibir tamanio buffer");
	
	//Asignar espacio en base a tamanio
	buffer = malloc(*size);
	
	//Recibir buffer
	if(recv(socket_cliente, buffer, *size, MSG_WAITALL) <= 0)
		log_error(logger, "Error al recibir buffer");

	return buffer;
}

char* recibir_mensaje(int socket_cliente, t_log *logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente, logger);
	return buffer;
}

t_list* recibir_paquete(int socket_cliente, t_log *logger)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	//Recibe tamanio del paquete y el paquete mismo
	buffer = recibir_buffer(&size, socket_cliente, logger);
	
	//Mientras: cantidad datos < tamanio paquete
	while(desplazamiento < size)
	{
		//Copia tamanio de dato
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		
		//Actualiza: se recibio un entero
		desplazamiento+=sizeof(int);
		
		//Asigna espacio segun tamanio recibido
		char* valor = malloc(tamanio); //NOTA: CAMBIAR DE char* A void*
		
		//Copia valor recibido
		memcpy(valor, buffer+desplazamiento, tamanio);
		
		//Actualiza: recibio un dato
		desplazamiento+=tamanio;

		//Enlistar dato
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}