// Archivo: main.c
#include "pcd8544.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>   // <-- 1. LIBRER�A A�ADIDA

// Variable global para la parte alta del contador
volatile uint32_t high_part_counter = 0;

// Rutina de Servicio de Interrupci�n (ISR)
void TIM1_UP_IRQHandler(void) {
    high_part_counter++;
    TIM1_SR &= ~0x0001;
}

int main(void) {
    uint64_t full_count = 0;
    char display_buffer[21]; 

    // Inicializaci�n de perif�ricos
    Clock_Init();
    SPI1_Init_nokia();
    TIM1_Timer_Init();
    PCD8544_Init();

    // Escribir un texto est�tico inicial
    PCD8544_WriteString("Contador:");
    
    // Bucle principal
    while (1) {
        full_count = ((uint64_t)high_part_counter << 16) | TIM1_CNT;
        
        sprintf(display_buffer, "%llu", full_count);
        
        PCD8544_SetCursor(0, 2);
        PCD8544_WriteString(display_buffer);
        
        // Rellenar con espacios para borrar d�gitos antiguos
        for (int i = strlen(display_buffer); i < 14; i++) { // <-- 2. FUNCI�N CORREGIDA
             PCD8544_DrawChar(' ');
        }
    }
}