/* Universidad del Valle de Guatemala
 IE3054 Electrónica Digital 2
 Autor: Luis Pablo Carranza
 Compilador: XC8, MPLAB X IDE (v6.00)
 Proyecto: Laboratorio No.04
 Hardware PIC16F887
 Creado: 09/02/23
 Última Modificación: 09/02/23*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSC 
//oscillator without clock out)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
//can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
//pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
//protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
//protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/
//External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-
//Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
//has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
//(Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
//(Write protection off)

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include "LCD.h"
#include "oscilador.h"
#include "conversiones.h"
#include "I2C.h"
#include <stdint.h>
#include <stdio.h>
#include "DS3231.h"


#define _XTAL_FREQ 8000000
#define DS232_write 0xD0
#define DS232_read 0xD1

uint8_t lecADC;
float conver;
char valADC[3];
char unidad;
char decena;

uint8_t sec;
uint8_t min;
uint8_t hor;

void portsetup(void);

void main(void) {
    setupINTOSC(7);     //Oscilador a 8MHz
    portsetup();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("S1: ");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("    V");
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String(":  :");
    
    while(1){
        
        I2C_Master_Start();
        I2C_Master_Write(0x11);
        lecADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        conver = (lecADC*5.0)/255;
        sprintf(valADC, "%.2f", conver);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valADC);
        
        sec = leer_sec();
        Lcd_Set_Cursor(1, 15);
        unidad = inttochar(descomponer(0, sec));
        Lcd_Write_Char(unidad);
        Lcd_Set_Cursor(1, 14);
        decena = inttochar(descomponer(1, sec));
        Lcd_Write_Char(decena);
        
        min = leer_min();
        Lcd_Set_Cursor(1, 12);
        unidad = inttochar(descomponer(0, min));
        Lcd_Write_Char(unidad);
        Lcd_Set_Cursor(1, 11);
        decena = inttochar(descomponer(1, min));
        Lcd_Write_Char(decena);
        
//        hor = leer_hora();
//        Lcd_Set_Cursor(1, 9);
//        unidad = inttochar(descomponer(0, hor));
//        Lcd_Write_Char(unidad);
//        Lcd_Set_Cursor(1, 8);
//        decena = inttochar(descomponer(1, hor));
//        Lcd_Write_Char(decena);
        
        
    }
}
void portsetup(){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    PORTA = 0; 
    TRISD = 0;
    PORTD = 0;
    TRISB = 0;
    PORTB = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C

}
