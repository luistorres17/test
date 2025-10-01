#ifndef PCD8544_H
#define PCD8544_H

#include "config.h"
#include <stdint.h>

// --- Prototipos de Funciones ---

// Inicializa el controlador PCD8544
void PCD8544_Init(void);

// Envía un comando al LCD
void PCD8544_SendCommand(uint8_t cmd);

// Envía un dato (píxeles) al LCD
void PCD8544_SendData(uint8_t data);

// Limpia el buffer de la pantalla y la pantalla física
void PCD8544_Clear(void);

// Establece la posición del cursor (columna x, fila y)
void PCD8544_SetCursor(uint8_t x, uint8_t y);

// Dibuja un carácter en la posición actual del cursor
void PCD8544_DrawChar(char c);

// Escribe una cadena de texto en la posición actual del cursor
void PCD8544_WriteString(const char* str);

#endif // PCD8544_H