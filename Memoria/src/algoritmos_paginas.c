#include "../include/algoritmos_paginas.h"

int criterio_clock(entrada_tabla_N2 *e)
{
    if(e->bit_uso == 1)
    {
        //CAMBIAR BIT DE USO Y SIGUE BUSCANDO
        e->bit_uso = 0;
        return 0;
    }
    //SINO TERMINA DE BUSCAR
    else
        return 1;
}

int criterio_clock_mejorado(entrada_tabla_N2 *e, int vuelta)
{
    switch(vuelta)
    {
        case 0:
        //PRIMER VUELTA SOLO VERIFICA POR MEJOR CASO
        return (e->bit_uso == 0 && e->bit_modificacion == 0);
        break;
        
        case 1:
        //SEGUNDA VUELTA VERIFICA POR SEGUNDO CASO ACEPTABLE
        //MODIFICA BIT USO
        if(e->bit_uso == 0 && e->bit_modificacion == 1)
            return 1;
        else
        {
            e->bit_uso = 0;
            return 0;
        }
        break;

        case 2:
        //TERCER VUELTA VUELVE A VERIFICAR MEJOR CASO
        return (e->bit_uso == 0 && e->bit_modificacion == 0);
        break;

        case 3:
        //CUARTA VUELTA VUELVE A VERIFICAR POR SEGUNDO CASO ACEPTABLE
        return (e->bit_uso == 0 && e->bit_modificacion == 1);
        break;

        default:
        return 0;
        break;
    }
}

entrada_tabla_N2* aplicar_busqueda_clock(int dir_tablaN1)
{
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);

    //NOTA: revisar logica en primer vuelta de for()
    //se toma valor en linea 55 para no empezar el for con valor nulo en ret
    entrada_tabla_N2 *ret = list_get(marcos_proceso, 0);
    for(int i = 1; criterio_clock(ret); i++)
    {
        //DIO UNA VUELTA
        if(i == list_size(marcos_proceso))
            i = 0;
        
        ret = list_get(marcos_proceso, i);
    }

    list_destroy_and_destroy_elements(marcos_proceso, free);
    //NOTA: podria eliminar los elementos en la lista principal (chequear despues)
    return ret;
}

entrada_tabla_N2* aplicar_busqueda_clock_mejorado(int dir_tablaN1)
{
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);

    entrada_tabla_N2 *ret = list_get(marcos_proceso, 0);
    int vuelta = 0;
    for(int i = 1; criterio_clock_mejorado(ret, vuelta); i++)
    {
        //DIO UNA VUELTA
        if(i == list_size(marcos_proceso))
        {
            i = 0;
            vuelta++;
        }

        ret = list_get(marcos_proceso, i);
    }

    list_destroy_and_destroy_elements(marcos_proceso, free);
    //NOTA: podria eliminar los elementos en la lista principal (chequear despues)
    return ret;
}