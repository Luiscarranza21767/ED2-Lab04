#include "DS3231.h"
#include "I2C.h"


uint8_t convertir(uint8_t valor, uint8_t tipo){
    uint8_t unidad;
    uint8_t decena;
    uint8_t conversion;
    unidad = valor & 0x0F;
    
    if (tipo == 0){
        decena = ((valor & 0xF0) >> 4);
        conversion = unidad+(decena*10);}
    else if (tipo == 1){
        decena = ((valor & 0x30) >> 4);
        conversion = unidad+(decena*10);
    }
    else if (tipo == 2){
        decena = ((valor & 0x10) >> 4);
        conversion = unidad+(decena*10);
    }
    return conversion;
}

uint8_t desconvertir(uint8_t valor){
    uint8_t decena;
    uint8_t unidad;
    decena = (valor/10);
    decena = (decena << 4);
    unidad = (valor%10);
    return (decena + unidad);
}
uint8_t leer_x(uint8_t address){
    uint8_t valor;
    I2C_Master_Start();     // Inicia la comunicación I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(address);        //Posición donde va leer
    I2C_Master_RepeatedStart();    //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    valor = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    if (address == 0x02){
        valor = convertir(valor, 1);
    }
    else if (address == 0x05){
        valor = convertir(valor, 2);
    }
    else {
        valor = convertir(valor, 0);
    }
    
    return valor;
}

void enviar_x(uint8_t val1, uint8_t val2, uint8_t val3, uint8_t val4, uint8_t val5){
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0);
    I2C_Master_Write(0);
    I2C_Master_Write(val1);        // Escribir minutos
    I2C_Master_Write(val2);        // Escribir hora
    I2C_Master_Write(1);
    I2C_Master_Write(val3);        //Escribir fecha
    I2C_Master_Write(val4);        //Escribir mes
    I2C_Master_Write(val5);        //Escribir año
    I2C_Master_Stop(); 
    
}


   