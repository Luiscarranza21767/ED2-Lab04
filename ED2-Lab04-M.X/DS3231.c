#include "DS3231.h"
#include "I2C.h"


uint8_t convertir(uint8_t valor){
    uint8_t unidad;
    uint8_t decena;
    uint8_t conversion;
    unidad = valor & 0x0F;
    decena = ((valor & 0xF0) >> 4);
    conversion = unidad+(decena*10);
    return conversion;
}



uint8_t leer_sec(void){
    uint8_t sec;
    I2C_Master_Start();     // Inicia la comunicación I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x00);        //Posición donde va leer
    I2C_Master_RepeatedStart();    //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    sec = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    sec = convertir(sec);
    return sec;
}

uint8_t leer_min(void){
    uint8_t min;
    I2C_Master_Start();     // Inicia la comunicación I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x01);        //Posición donde va leer
    I2C_Master_RepeatedStart();    //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    min = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    min = convertir(min);
    return min;
}

uint8_t leer_hora(void){
    uint8_t hora;
    I2C_Master_Start();     // Inicia la comunicación I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x02);        //Posición donde va leer
    I2C_Master_RepeatedStart();    //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
     hora = I2C_Master_Read(1);     //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    return hora;
}

   