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

uint8_t lecADC;
float conver;
char valADC[3];
char unidad;
char decena;

uint8_t sec, segundo;
uint8_t min, minuto;
uint8_t hor, hora;
uint8_t fecha, fecha1;
uint8_t mes, mes1;
uint8_t year, year1;
uint8_t modo;

void portsetup(void);
void Escribir_dato(uint8_t dato, uint8_t posx, uint8_t posy);

//void __interrupt() isr (void){
//    if(INTCONbits.RBIF){
//        __delay_ms(20);
//        if(PORTBbits.RB7 == 0){ // Botón que selecciona m, h, d, m, a
//            if (modo <4){
//                modo += 1;
//            }
//            else {
//                modo = 0;
//            }
//        }
//        else if(PORTBbits.RB6 == 0){
//            segundo = sec;
//            minuto = min;
//            hora = hor;
//            fecha1 = fecha;
//            mes1 = mes;
//            year1 = year;
//            if (modo == 0){
//                if (minuto<60){
//                    minuto ++;   
//                } 
//                else {
//                    minuto = 0;
//                }
//            }
//            else if (modo == 1){
//                if (hora<24){
//                    hora ++;
//                }
//                else {
//                    hora = 0;
//                }
//            }
//            else if (modo == 2){
//                fecha1 ++;
//            }
//            else if (modo == 3){
//                mes1 ++;
//            }
//            else if (modo == 4){
//                year1 ++;
//            }
//        }
//        else if(PORTBbits.RB5 == 0){
//            segundo = sec;
//            minuto = min;
//            hora = hor;
//            fecha1 = fecha;
//            mes1 = mes;
//            year1 = year;
//            if (modo == 0){
//                if (minuto > 0){
//                    minuto --;
//                }
//                else {
//                    minuto = 60;
//                }
//            }
//            else if (modo == 1){
//                if (hora>0){
//                    hora --;
//                }
//                else {
//                    hora = 23;
//                }
//            }
//            else if (modo == 2){
//                fecha1 --;
//            }
//            else if (modo == 3){
//                mes1 --;
//            }
//            else if (modo == 4){
//                year1 --;
//            }
//        }
//        
//        minuto = desconvertir(minuto);
//        hora = desconvertir(hora);
//        fecha1 = desconvertir(fecha1);
//        mes1 = desconvertir(mes1);
//        year1 = desconvertir(year1);
//        enviar_x(0, minuto, hora, 0);
//        enviar_x(1, fecha1, mes1, year1);
//        
//        INTCONbits.RBIF = 0;
//        
//    }
//}

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
    Lcd_Set_Cursor(2,10);
    Lcd_Write_String("/  /");
    modo = 0;
    
    enviar_x(0, 0, 1, 1, 0);
    
    while(1){
        PORTA = modo;
        // Comunicación con pic esclavo
        I2C_Master_Start();
        I2C_Master_Write(0x11);
        lecADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        conver = (lecADC*5.0)/255;
        sprintf(valADC, "%.2f", conver);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valADC);
        
        // Comunicación con DS3231
        sec = leer_x(0x00);     // Leer segundo
        Escribir_dato(sec, 14, 1);
        
        min = leer_x(0x01);     // Leer minuto
        Escribir_dato(min, 11, 1);
        
        hor = leer_x(0x02);     // Leer hora
        Escribir_dato(hor, 8, 1);
        
        fecha = leer_x(0x04);   // Leer fecha
        Escribir_dato(fecha, 8, 2);
        
        mes = leer_x(0x05);     // Leer mes
        Escribir_dato(mes, 11, 2);
        
        year = leer_x(0x06);    // Leer año
        Escribir_dato(year, 14, 2);
        
        
        if(PORTBbits.RB7 == 0){ // Botón que selecciona m, h, d, m, a
            __delay_ms(20);
            if (modo <4){
                modo += 1;
            }
            else {
                modo = 0;
            }
        }
        
        if(PORTBbits.RB6 == 0){
            __delay_ms(20);
            segundo = sec;
            minuto = min;
            hora = hor;
            fecha1 = fecha;
            mes1 = mes;
            year1 = year;
            
            if (modo == 0){
                if (minuto<59){
                    minuto ++;   
                } 
                else {
                    minuto = 0;
                }
            }
            else if (modo == 1){
                if (hora<23){
                    hora ++;
                }
                else {
                    hora = 0;
                }
            }
            else if (modo == 2){
                if((mes == 1) || (mes == 3) || (mes == 5) || (mes == 7) || (mes == 8) || (mes == 10) || (mes == 12)){
                    if (fecha1 < 31){
                        fecha1 ++;
                    }
                    else {
                        fecha1 = 1;
                    }
                }
                else if(mes == 2){
                    if (fecha1 < 28){
                        fecha1 ++;
                    }
                    else {
                        fecha1 = 1;
                    }
                }
                else{
                    if (fecha1 < 30){
                        fecha1 ++;
                    }
                    else {
                        fecha1 = 1;
                    }
                }
            }
            else if (modo == 3){
                if (mes<12){
                    mes1 ++;
                }
                else {
                    mes1 = 1;
                }
            }
            else if (modo == 4){
                if (year <99){
                    year1 ++;
                }
                else {
                    year1 = 0;
                }
            }
            minuto = desconvertir(minuto);
            hora = desconvertir(hora);
            fecha1 = desconvertir(fecha1);
            mes1 = desconvertir(mes1);
            year1 = desconvertir(year1);
            enviar_x(minuto, hora, fecha1, mes1, year1);
        }
        if(PORTBbits.RB5 == 0){
            __delay_ms(20);
            segundo = sec;
            minuto = min;
            hora = hor;
            fecha1 = fecha;
            mes1 = mes;
            year1 = year;
            if (modo == 0){
                if (minuto > 0){
                    minuto --;
                }
                else {
                    minuto = 59;
                }
            }
            else if (modo == 1){
                if (hora>0){
                    hora --;
                }
                else {
                    hora = 23;
                }
            }
            else if (modo == 2){
                if((mes == 1) || (mes == 3) || (mes == 5) || (mes == 7) || (mes == 8) || (mes == 10) || (mes == 12)){
                    if (fecha1 > 1){
                        fecha1 --;
                    }
                    else {
                        fecha1 = 31;
                    }
                }
                else if(mes == 2){
                    if (fecha1 > 1){
                        fecha1 --;
                    }
                    else {
                        fecha1 = 28;
                    }
                }
                else{
                    if (fecha1 > 1){
                        fecha1 --;
                    }
                    else {
                        fecha1 = 30;
                    }
                }
            }
            else if (modo == 3){
                if (mes1 > 1){
                    mes1 --;
                }
                else {
                    mes1 = 12;
                }
            }
            else if (modo == 4){
                if (year1 > 0){
                    year1 --;
                }
                else {
                    year1 = 99;
                }
            }
            minuto = desconvertir(minuto);
            hora = desconvertir(hora);
            fecha1 = desconvertir(fecha1);
            mes1 = desconvertir(mes1);
            year1 = desconvertir(year1);
            enviar_x(minuto, hora, fecha1, mes1, year1);
        }
        
        
    }
}
void portsetup(){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    PORTA = 0; 
    TRISD = 0;
    PORTD = 0;
    
    // Configuración del puerto B (interrupciones)
    TRISB = 0b11100000;
    PORTB = 0b11100000;
//    INTCONbits.GIE = 1;     // Habilitar interrupciones globales
//    INTCONbits.RBIE = 1;    // Habilita interrupción del puerto B
//    INTCONbits.RBIF = 0;    // Apaga la bandera de interrupción del puerto B
//    IOCB = 0b11100000;      // Habilita la interrupción en cambio (IoC)
    WPUB = 0b11100000;      // Habilita el Weak Pull-Up en el puerto B
    OPTION_REGbits.nRBPU = 0;   // Deshabilita el bit de RBPU
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C

}

void Escribir_dato(uint8_t dato, uint8_t posx, uint8_t posy){
    Lcd_Set_Cursor(posy, posx+1);
    unidad = inttochar(descomponer(0, dato));
    Lcd_Write_Char(unidad);
    Lcd_Set_Cursor(posy, posx);
    decena = inttochar(descomponer(1, dato));
    Lcd_Write_Char(decena);
}