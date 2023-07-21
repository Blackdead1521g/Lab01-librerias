/* Archivo: Configuracion.c
 * Dispositivo: PIC16F887
 * Autor: Kevin Alarcón
 * Compilador: XC8(v2.40), MPLABX V6.05
 * 
 * 
 * Programa: Librería de configuracion
 * Hardware: NA
 * 
 * Creado: 17 de julio, 2023
 * Última modificación: 21 de julio, 2023
 */

#include "Configuracion.h"

//Función para la configuración de puertos 
void config_puertos(void){
    //definir digitales
    ANSELbits.ANS0 = 1; //Seleccionamos solo los dos pines que utilizaremos como analógicos
    ANSELbits.ANS1 = 1;
    ANSELbits.ANS2 = 1;
    ANSELbits.ANS3 = 1;
    ANSELbits.ANS4 = 1;
    ANSELH = 0; 
    
    //Definimos puertos que serán entradas
    TRISB = 0b01111111;
    TRISA = 0b11111111;
    
    //Definimos puertos que serán salidas
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
    //Limpiamos los puertos
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

//Función para configurar pullups y cuantos pines queremos con interrupción
void ioc_init(char pin){
    OPTION_REGbits.nRBPU = 0; //Habilitamos los PULLUPS
    IOCB = pin; //Habilitamos las interrupciones al cambiar de estado el puerto B
}

//Función para elegir la frecuencia del oscilador
void frec_osc(unsigned int freq){
    if (freq == 8){
        OSCCONbits.IRCF = 0b111; //8 MHz
    }
    if (freq == 4){
        OSCCONbits.IRCF = 0b110; //4 MHz
    }
    if (freq == 2){
        OSCCONbits.IRCF = 0b101; //2 MHz
    }
    if (freq == 1){
        OSCCONbits.IRCF = 0b100; //1 MHz
    }
    if (freq == 500){
        OSCCONbits.IRCF = 0b011; //500 KHz
    }
    if (freq == 250){
        OSCCONbits.IRCF = 0b010; //250 KHz
    }
    if (freq == 125){
        OSCCONbits.IRCF = 0b001; //125 KHz
    }
    if (freq == 31){
        OSCCONbits.IRCF = 0b000; //31 KHz
    }
    OSCCONbits.SCS = 1; //Configurar el reloj interno como Fosc sistema
    
}

//Función para configurar las banderas e interrupciones
void config_banderas(void){
    INTCONbits.RBIF = 0; //Apagamos la bandera del puerto B
    INTCONbits.RBIE = 1; //Habilitamos la interrupción en el puerto B
    INTCONbits.GIE = 1; //Habilitar interrupciones globales
    INTCONbits.T0IF = 0; //interrupcion del tmr0
    INTCONbits.T0IE = 1; //Hbilitar interrupción del TMR0
}

//Función para elegir en que canal iniciar el ADC
void adc_init(int channel){
    if(channel == 0){
        ADCON0bits.CHS = 0b0000; //Canal 0
    }
    else if(channel == 1){
        ADCON0bits.CHS = 0b0001; //Canal 1
    }
    else if(channel == 2){
        ADCON0bits.CHS = 0b0010; //Canal 2
    }
    else if(channel == 3){
        ADCON0bits.CHS = 0b0011; //Canal 3
    }
    else if(channel == 4){
        ADCON0bits.CHS = 0b0100; //Canal 4
    }
    
    //Configuración ADC
    ADCON1bits.VCFG0 = 0; //Voltaje referenciado de 0V
    ADCON1bits.VCFG1 = 0; //Voltaje referenciado de 5V
    ADCON0bits.ADCS = 0b10; // Fosc/32
     
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON0bits.ADON = 1;//Encendemos el módulo del ADC
     
    //Banderas e interrupciones
    PIR1bits.ADIF = 0; //Apagamos la bandera del ADC
    INTCONbits.PEIE = 1; //Habilitar interrupciones periféricas
    PIE1bits.ADIE = 1; //Habilitar interrupciones del ADC
    INTCONbits.GIE = 1; //Habilitar interrupciones globales
    ADCON0bits.GO = 1; //Activamos la lectura del ADC
}

int adc_read(){
    return ADRESH;
}

void adc_change_channel(int channel){
    if(channel <= 4){ 
        ADCON0bits.CHS = channel;
    }
    else {
        ADCON0bits.CHS = 0b0000;
    }
    return;
    
}

int adc_get_channel(){
    char canal = ADCON0bits.CHS; //Le asignamos a la variable canal el número de canal del ADC
    return canal;
}