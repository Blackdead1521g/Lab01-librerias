/* 
 * File:   Interrupcion.h
 * Author: Black
 *
 * Created on 17 de julio de 2023, 01:45 PM
 */

#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic16f887.h>

#ifndef CONFIGURACION_H
#define	CONFIGURACION_H

void config_puertos(void);
void ioc_init(char pin);
void frec_osc(unsigned int freq);
void config_banderas(void);

void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();

#endif	/* CONFIGURACION_H */

