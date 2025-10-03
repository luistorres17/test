// Archivo: main.c
#include "pcd8544.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// --- Variables para la lógica de control ---
volatile uint8_t backlight_state = 1; // 1 = encendido (empezamos con luz)
uint8_t prev_btn_toggle = 1, prev_btn_up = 1, prev_btn_down = 1;
char display_buffer[12];

// Rango de Contraste (Vop): 0x00 a 0x7F.
// Un buen valor inicial suele ser 0x3A.
volatile uint8_t contrast_value = 0x3A; 

// Función de delay simple para anti-rebote
static void simple_delay(volatile uint32_t count) {
    while(count--);
}

// Función dedicada para actualizar el contraste
void update_contrast(uint8_t new_contrast) {
    PCD8544_SendCommand(0x21); // Entrar a modo extendido
    PCD8544_SendCommand(0x80 | new_contrast); // Enviar nuevo valor de Vop
    PCD8544_SendCommand(0x20); // Volver a modo básico
}

int main(void) {
    // --- Inicializaciones ---
    Clock_Init();
    SPI1_Init_nokia();
    Button_LED_Init(); 
    PCD8544_Init();

    // --- Estado Inicial ---
    update_contrast(contrast_value); // Establecer el contraste inicial
    GPIOB_BSRR = (1 << 0); // Encender backlight al inicio

    // --- Bucle Principal ---
    while (1) {
        // --- Actualizar la información en pantalla ---
        PCD8544_SetCursor(0, 0);
        PCD8544_WriteString("AsistenteStm32");
        PCD8544_SetCursor(0, 2);
        PCD8544_WriteString("Contraste:");
        sprintf(display_buffer, "0x%02X", contrast_value); // Formatear como hexadecimal
        PCD8544_SetCursor(0, 3);
        PCD8544_WriteString(display_buffer);

        // --- Leer estado actual de los botones ---
        uint8_t btn_toggle = (GPIOB_IDR & (1 << 1))  ? 1 : 0;
        uint8_t btn_up     = (GPIOB_IDR & (1 << 10)) ? 1 : 0;
        uint8_t btn_down   = (GPIOB_IDR & (1 << 11)) ? 1 : 0;

        // --- Lógica para el botón de Toggle Backlight (PB1) ---
        if (prev_btn_toggle == 1 && btn_toggle == 0) {
            backlight_state = !backlight_state;
            if (backlight_state) {
                GPIOB_BSRR = (1 << 0);   // Set PB0 (Encender)
            } else {
                GPIOB_BSRR = (1 << 16);  // Reset PB0 (Apagar)
            }
            simple_delay(200000); 
        }

        // --- Lógica para el botón de Aumentar Contraste (PB10) ---
        if (prev_btn_up == 1 && btn_up == 0) {
            if (contrast_value < 0x7F) {
                contrast_value++;
                update_contrast(contrast_value);
            }
            simple_delay(200000);
        }

        // --- Lógica para el botón de Disminuir Contraste (PB11) ---
        if (prev_btn_down == 1 && btn_down == 0) {
            if (contrast_value > 0x00) {
                contrast_value--;
                update_contrast(contrast_value);
            }
            simple_delay(200000);
        }

        // --- Actualizar estados anteriores de los botones ---
        prev_btn_toggle = btn_toggle;
        prev_btn_up = btn_up;
        prev_btn_down = btn_down;
    }
}