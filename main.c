// Archivo: main.c
#include "pcd8544.h"
#include <stdint.h>

// --- Variables para la lógica de Toggle ---
// Almacena el estado actual del LED (0 = apagado, 1 = encendido)
volatile uint8_t led_state = 0; 
// Almacena el estado anterior del botón para detectar solo el flanco de bajada
uint8_t previous_button_state = 1; 

// Función de delay simple para anti-rebote
static void simple_delay(volatile uint32_t count) {
    while(count--);
}

int main(void) {
    // Inicialización de periféricos
    Clock_Init();
    SPI1_Init_nokia();
    PCD8544_Init();
    Button_LED_Init(); // Usamos la nueva función de inicialización

    // Escribir un texto estático inicial
    PCD8544_WriteString("Hola");
    PCD8544_SetCursor(0, 1);
    PCD8544_WriteString("oty");
    
    // Apagar el LED/Backlight al inicio (poniendo PB0 en bajo)
    GPIOB_BSRR = (1 << 16); // Escribe 1 en el bit de reset para PB0

    while (1) {
        // Leemos el estado actual del botón en PB1
        uint8_t current_button_state = (GPIOB_IDR & 0x00000002) ? 1 : 0;

        // Comprobamos si ha habido un flanco de bajada (de no presionado a presionado)
        if (previous_button_state == 1 && current_button_state == 0) {
            
            // Invertimos el estado del LED
            led_state = !led_state;

            if (led_state) {
                // Encender el LED (poniendo PB0 en ALTO)
                GPIOB_BSRR = (1 << 0); // Escribe 1 en el bit de set para PB0
            } else {
                // Apagar el LED (poniendo PB0 en BAJO)
                GPIOB_BSRR = (1 << 16); // Escribe 1 en el bit de reset para PB0
            }
            
            // Pequeña pausa para evitar el rebote del botón (debounce)
            simple_delay(100000); 
        }

        // Actualizamos el estado anterior del botón
        previous_button_state = current_button_state;
    }
}