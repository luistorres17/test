// Archivo: main.c
#include "pcd8544.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h> 



int main(void) {
  //uint64_t full_count = 0;
  //char display_buffer[21]; 

  // Inicialización de periféricos
  Clock_Init();
  SPI1_Init_nokia();
  TIM1_Timer_Init();
  PCD8544_Init();
	p_pullup();

  // Escribir un texto estático inicial
  PCD8544_WriteString("hola");
	PCD8544_SetCursor(0,1);
	PCD8544_WriteString("oty:");
	
	while(1) {
        // Leemos el registro de entrada del puerto B (GPIOB_IDR)
        // y comprobamos el estado del bit 3.
        if (GPIOB_IDR & 0x00000008) { // Si el bit 3 es '1' (botón NO presionado por el pull-up)
            // Encendemos el LED (poniendo PC13 en BAJO, común en placas "Blue Pill")
            // Para poner en BAJO (reset) el pin 13, escribimos '1' en el bit 29 (BR13) de BSRR.
            // Valor: 0x20000000
            GPIOC_BSRR = 0x20000000;
        } else { // Si el bit 3 es '0' (botón SÍ presionado, conectado a GND)
            // Apagamos el LED (poniendo PC13 en ALTO)
            // Para poner en ALTO (set) el pin 13, escribimos '1' en el bit 13 (BS13) de BSRR.
            // Valor: 0x00002000
            GPIOC_BSRR = 0x00002000;
        }
    }
	
    
    
}