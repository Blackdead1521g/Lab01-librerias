/* Archivo: Displays.c
 * Dispositivo: PIC16F887
 * Autor: Kevin Alarcón
 * Compilador: XC8(v2.40), MPLABX V6.05
 * 
 * 
 * Programa: Librería de cconfiguración para display
 * Hardware: NA
 * 
 * Creado: 17 de julio, 2023
 * Última modificación: 21 de julio, 2023
 */

#include "Displays.h"

//---------------------Tablas------------------------------------
uint8_t table (int X){ //Función que contiene los valores que se le ingresarán a los displays
    switch(X){ //Realizamos un switch case para ir verificando en qué número está el parámetro (dependiendo el número le asignamos su respecto valor al display)
        case 0: 
            return 0b00111111; //(0)
        case 1:
            return 0b00000110; //(1)
        case 2:
            return 0b01011011; //(2)
        case 3:
            return 0b01001111; //(3)
        case 4:
            return 0b01100110; //(4)
        case 5:
            return 0b01101101; //(5)
        case 6:
            return 0b01111101; //(6)
        case 7:
            return 0b00000111; //(7)
        case 8:
            return 0b01111111; //(8)
        case 9:
            return 0b01101111; //(9)
        case 10:
            return 0b01110111; //(A)
        case 11:
            return 0b01111100; //(B)
        case 12:
            return 0b00111001; //(C)
        case 13:
            return 0b01011110; //(D)
        case 14:
            return 0b01111001; //(E)
        case 15:
            return 0b01110001; //(F)
    }
}

uint8_t preparar_display1(int unidades){ //En esta función le asignaremos a cada variable de display su respectiva unidad
    int display1 = table(unidades); //Al display1 le asignamos las unidades
    return display1;
}

uint8_t preparar_display2(int decenas){ //En esta función le asignaremos a cada variable de display su respectiva unidad
    int display2 = table(decenas); //Al display2 le asignamos las decenas
    return display2;
}
