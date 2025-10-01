#include "max7219.h"

void mostrarnumero(uint32_t number) {
    // Inicia un bucle que se repite 8 veces, una para cada d�gito del display.
    // La variable 'i' representa la posici�n del d�gito en el MAX7219,
    // donde i=1 es el primer d�gito (extremo derecho) y i=8 es el �ltimo (extremo izquierdo).
    for (int i = 1; i <= 8; i++) {
        
        // 1. Extrae el d�gito menos significativo (el de la derecha) del n�mero.
        // La operaci�n m�dulo (%) 10 calcula el resto de la divisi�n por 10.
        // Ejemplo: si 'number' es 123, 'number % 10' resulta en 3.
        uint8_t digit = number % 10;
        
        // 2. Env�a el d�gito extra�do al display.
        // Llama a la funci�n de comunicaci�n SPI para enviar el d�gito ('digit')
        // a la posici�n correspondiente ('i').
        MAX7219_SendCommand(i, digit);
        
        // 3. Prepara el n�mero para la siguiente iteraci�n.
        // Se realiza una divisi�n entera por 10 para eliminar el d�gito que ya se mostr�.
        // Ejemplo: si 'number' era 123, despu�s de esta operaci�n ser� 12.
        number /= 10;
    }
}

void MAX7219_SendCommand(uint8_t command, uint8_t value) {
    uint16_t data = (command << 8) | value;
    
    // Poner la l�nea CS (PA4) en bajo
    GPIOA_ODR &= ~0x00000010; 
    
    // Esperar a que el b�fer de transmisi�n (TXE) est� vac�o (TXE = bit 1)
    while (!(SPI1_SR & 0x0002));
    
    // Enviar los 16 bits de datos
    SPI1_DR = data;
    
    // Esperar a que la transmisi�n finalice (BSY = bit 7)
    while (SPI1_SR & 0x0080);
    
    // Poner la l�nea CS (PA4) en alto para latch
    GPIOA_ODR |= 0x00000010; // (1 << 4)
}