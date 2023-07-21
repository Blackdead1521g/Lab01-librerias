/* Archivo: Lab01.c
 * Dispositivo: PIC16F887
 * Autor: Kevin Alarcón
 * Compilador: XC8(v2.40), MPLABX V6.05
 * 
 * 
 * Programa: Realizar dos contadores, uno con botones y otro con ADC a través de librerías
 * Hardware: Potenciometros, botones y leds
 * 
 * Creado: 18 de julio, 2023
 * Última modificación: 21 de julio, 2023
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//---------------------Librerías----------------------------------
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "Configuracion.h"
#include "Displays.h"


#define _tmr0_value 255 // valor de tmr0 para que la interrupción sea cada 16.38ms 
#define _XTAL_FREQ 8000000 //definimos la frecuencia del oscilador

//---------------------Variables---------------------------------
int canal = 0;
int banderas = 0;
int numero = 0;
int contador = 0;
int decenas = 0;
int unidades = 0;
int display1 = 0;
int display2 = 0;

//-------------------Prototipos de funciones------------------
void setup(void);
void displays (int numero);
void preparar_displays(void);
uint8_t table (int X);

//----------------------Interrupciones---------------------------
void __interrupt() isr(void) {
    //Interrupción del TMR0
    if (INTCONbits.T0IF) { // si se produce overflow en el TMR0 y se activa la bandera
        PORTE = 0; //Inicialmente limpiamos el puerto donde estará el multiplexado
        if (banderas == 0){ //Verificamos si bandera es igual a cero
            PORTC = display1; //Si si es igual a cero le asignamos al puerto C el valor de display1 (unidades)
            PORTE = 2; //Encendemos el segundo bit del puerto de multiplexado
            banderas = 1; //Igualamos a uno la variable de banderas
        }
        else if (banderas == 1){ //Verificamos si bandera es igual a uno
            PORTC = display2;  //Si si es igual a uno le asignamos al puerto C el valor de display2 (decenas)
            PORTE = 4;  //Encendemos el tercer bit del puerto de multiplexado
            banderas = 0; //Igualamos a uno la variable de banderas
        }
        TMR0 = _tmr0_value; //Reestablecer el valor necesario para el TMR0
        INTCONbits.T0IF = 0; //Limpiar bandera de interrupcion del TMR0
    }
    
    //Interrupción del Puerto B
    if (INTCONbits.RBIF) //Revisamos si la bandera de interrupción del Puerto B se enciende
    {   //RB0 --> Incrementa puertoD
        if (PORTBbits.RB0 == 0){
            PORTD++;
        }
        //RB1 --> Decrementa puerto D
        else if (PORTBbits.RB1 == 0){
            PORTD--;
        }
        //RB2 --> Cambiar de canal en el ADC
        else if (PORTBbits.RB2 == 0){
            canal++;
            if(canal == 5){
                canal = 0;
            }
            adc_change_channel(canal);
        }
        INTCONbits.RBIF = 0; //Apagamos la bandera del puerto B
    }
    
    //Interrupción del ADC
    if (PIR1bits.ADIF) {
        numero = adc_read(); //Le asignamos a número el valor del ADC
        PIR1bits.ADIF = 0; //Limpiar la bandera de la interrupcion del ADC
    }
    
    return;
}

void main(void) {
    setup();
    while(1){//loop forever
        contador = PORTD;
        
        if(numero > contador){
            PORTBbits.RB7 = 1;
        }
        else{
            PORTBbits.RB7 = 0;
        }
        
        displays(numero);
        display1 = preparar_display1(unidades); //Llamamos a nuestra función que prepara los valores de los displays
        display2 = preparar_display2(decenas); //Llamamos a nuestra función que prepara los valores de los displays
        
        if (ADCON0bits.GO == 0) { // Si la lectura del ADC se desactiva
            
            __delay_us(1000); //Este es el tiempo que se dará cada vez que se desactiva la lectura
            ADCON0bits.GO = 1; //Activamos la lectura del ADC
        }  
    }
}

void displays (int numero){ //Esta función nos permite dividir la variable de nuestro contador en unidades, decenas y centenas.
    unidades = numero & 0x0F; //Para unidades solo la igualamos al residuo que calculamos antetriormente
    decenas = (numero & 0xf0)>> 4; //Para decenas tomamos el residuo anterior y los dividimos dentro de 10 para que nos de solo las decenas
    return;
}

void setup(void){
    config_puertos(); //Configuramos los puertos
    ioc_init(0xff); //Inicializamos las interrupciones en el puertoB
    frec_osc(8); //Escogemos la frecuencia de oscilación
    config_banderas(); //Configuramos las banderas e interrupciones
    
    adc_init(0); //Inicializamos el canal del ADC
    
    /////////////// tmr0
    OPTION_REGbits.T0CS = 0; //Usar Timer0 con Fosc/4
    OPTION_REGbits.PSA = 0; //Prescaler con el Timer0
    OPTION_REGbits.PS2 = 1; //Prescaler de 256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;  
    TMR0 = _tmr0_value; ///VALOR INICIAL DEL TMR0
    return;        
}    
