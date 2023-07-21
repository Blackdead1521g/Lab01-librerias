/* 
 * File:   Displays.h
 * Author: Black
 *
 * Created on 20 de julio de 2023, 09:01 PM
 */

#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic16f887.h>

#ifndef DISPLAYS_H
#define	DISPLAYS_H

uint8_t preparar_display1(int unidades);
uint8_t preparar_display2(int decenas);
uint8_t table (int X);

#endif	/* DISPLAYS_H */

