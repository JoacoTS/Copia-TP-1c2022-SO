#ifndef HEADERS_PLANIFICADOR_CORTO_PLAZO_H_
#define HEADERS_PLANIFICADOR_CORTO_PLAZO_H_

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "./config.h"
#include "../../shared/include/shared_log.h"
#include "../../shared/include/shared_utils.h"
#include <commons/collections/list.h>

void iniciar_planificador_corto_plazo();
void planificar_procesos();

#endif /* HEADERS_PLANIFICADOR_CORTO_PLAZO_H_ */