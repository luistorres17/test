#include "max7219.h"

void mostrarnumero(uint32_t number) {
    // Inicia un bucle que se repite 8 veces, una para cada dígito del display.
    // La variable 'i' representa la posición del dígito en el MAX7219,
    // donde i=1 es el primer dígito (extremo derecho) y i=8 es el último (extremo izquierdo).
    for (int i = 1; i <= 8; i++) {
        
        // 1. Extrae el dígito menos significativo (el de la derecha) del número.
        // La operación módulo (%) 10 calcula el resto de la división por 10.
        // Ejemplo: si 'number' es 123, 'number % 10' resulta en 3.
        uint8_t digit = number % 10;
        
        // 2. Envía el dígito extraído al display.
        // Llama a la función de comunicación SPI para enviar el dígito ('digit')
        // a la posición correspondiente ('i').
        MAX7219_SendCommand(i, digit);
        
        // 3. Prepara el número para la siguiente iteración.
        // Se realiza una división entera por 10 para eliminar el dígito que ya se mostró.
        // Ejemplo: si 'number' era 123, después de esta operación será 12.
        number /= 10;
    }
}

void MAX7219_SendCommand(uint8_t command, uint8_t value) {
    uint16_t data = (command << 8) | value;
    
    // Poner la línea CS (PA4) en bajo
    GPIOA_ODR &= ~0x00000010; 
    
    // Esperar a que el búfer de transmisión (TXE) esté vacío (TXE = bit 1)
    while (!(SPI1_SR & 0x0002));
    
    // Enviar los 16 bits de datos
    SPI1_DR = data;
    
    // Esperar a que la transmisión finalice (BSY = bit 7)
    while (SPI1_SR & 0x0080);
    
    // Poner la línea CS (PA4) en alto para latch
    GPIOA_ODR |= 0x00000010; // (1 << 4)
}