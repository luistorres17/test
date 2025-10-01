// Archivo: main.c
#include "pcd8544.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>   // <-- 1. LIBRERÍA AÑADIDA

// Variable global para la parte alta del contador
volatile uint32_t high_part_counter = 0;

// Rutina de Servicio de Interrupción (ISR)
void TIM1_UP_IRQHandler(void) {
    high_part_counter++;
    TIM1_SR &= ~0x0001;
}

int main(void) {
    uint64_t full_count = 0;
    char display_buffer[21]; 

    // Inicialización de periféricos
    Clock_Init();
    SPI1_Init_nokia();
    TIM1_Timer_Init();
    PCD8544_Init();

    // Escribir un texto estático inicial
    PCD8544_WriteString("Contador:");
    
    // Bucle principal
    while (1) {
        full_count = ((uint64_t)high_part_counter << 16) | TIM1_CNT;
        
        sprintf(display_buffer, "%llu", full_count);
        
        PCD8544_SetCursor(0, 2);
        PCD8544_WriteString(display_buffer);
        
        // Rellenar con espacios para borrar dígitos antiguos
        for (int i = strlen(display_buffer); i < 14; i++) { // <-- 2. FUNCIÓN CORREGIDA
             PCD8544_DrawChar(' ');
        }
    }
}