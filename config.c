#include "config.h"

void Clock_Init(void) {
    // 1. Habilitar el Oscilador Externo (HSE)
    RCC_CR |= 0x10000; // Pone a 1 el bit 16 (HSEON) para encender el oscilador externo.
    while (!(RCC_CR & 0x20000)); // Espera en un bucle hasta que el bit 17 (HSERDY) se ponga a 1, indicando que el HSE es estable.

    // 2. Seleccionar HSE como Reloj del Sistema
    RCC_CFGR &= ~0x3;      // Limpia los bits 0 y 1 (SW), que seleccionan la fuente del reloj del sistema.
    RCC_CFGR |= 0x1;       // Escribe '01' en los bits SW para seleccionar HSE como SYSCLK.
    while ((RCC_CFGR & 0xC) != 0x4); // Espera hasta que los bits 3:2 (SWS) sean '01', confirmando que el HSE está en uso.
}

void SPI1_Init(void) {
    // 1. Habilitar relojes para GPIOA y SPI1
    RCC_APB2ENR |= 0x00001004;

    // 2. Configurar pines GPIOA (PA4, PA5, PA7) de forma atómica
    // Se limpia y se escribe la nueva configuración en una sola operación
    GPIOA_CRL = (GPIOA_CRL & 0x0F00FFFF) | 0xB0B30000;

    // 3. Configurar y habilitar el periférico SPI1 en una sola escritura
    SPI1_CR1 = 0x0B5C;
}

void SPI1_Init_nokia(void) {
    // 1. Habilitar relojes para GPIOA y SPI1
    RCC_APB2ENR |= 0x00001004;

    // 2. Configurar pines GPIOA (PA2, PA3, PA4, PA5, PA7)
    // PA2 (RST), PA3 (D/C), PA4 (CS) -> Salidas Push-Pull
    // PA5 (SCK), PA7 (MOSI) -> Salidas de Función Alternativa Push-Pull
    // Se limpia y se escribe la nueva configuración en una sola operación
    GPIOA_CRL = (GPIOA_CRL & 0x0F0000FF) | 0xB0B33300;

    // 3. Configurar y habilitar el periférico SPI1 en una sola escritura
    SPI1_CR1 = 0x035C; // Similar al anterior, pero con DFF=0 (8 bits) y habilitado.
}

void TIM1_Counter_Init(void) {
    // 1. Habilitar relojes para GPIOA y TIM1
    RCC_APB2ENR |= 0x00000804; // Habilita reloj para GPIOA (bit 2) y TIM1 (bit 11)
    // 2. Configurar pin PA9 (TIM1_CH2) como entrada con pull-up
    GPIOA_CRH &= 0xFFFFFF0F;      // Limpia la configuración actual de PA9
    GPIOA_CRH |= 0x00000080;      // MODE9=00 (Input), CNF9=10 (Pull-up/down)
    GPIOA_ODR |= 0x00000200;      // Activa la resistencia de pull-up en PA9 (ODR9=1)
    // 3. Limpiar registros de TIM1 antes de configurar
    TIM1_CR1   = 0x0000;
    TIM1_SMCR  = 0x0000;
    TIM1_CCMR1 = 0x0000;
    TIM1_CCER  = 0x0000;
    // 4. Configurar TIM1 en Modo Reloj Externo 1 usando el CANAL 2
    // a. Configurar canal 2 como entrada mapeada a TI2
    TIM1_CCMR1 |= 0x00000100; // CC2S[1:0] = 01
    // b. Aplicar el filtro anti-rebotes más fuerte
    TIM1_CCMR1 |= 0x0000F000; // IC2F[3:0] = 1111
    // c. Configurar la polaridad del trigger (flanco de bajada)
    TIM1_CCER |= 0x00000020;  // CC2P = 1
    // d. Configurar el modo esclavo para usar el reloj externo del Canal 2
    TIM1_SMCR |= 0x00000060; // TS[2:0] = 110 (Selecciona TI2FP2)
    TIM1_SMCR |= 0x00000007; // SMS[2:0] = 111 (Modo Reloj Externo 1)
    // e. Establecer el valor máximo del contador
    TIM1_ARR = 0xFFFF;
    // f. Habilitar el contador
    TIM1_CR1 |= 0x0001;      // CEN = 1
}



void TIM1_Timer_Init(void) {
    // 1. Habilitar el reloj para el periférico TIM1.
    // El bit 11 en el registro RCC_APB2ENR corresponde a TIM1.
    RCC_APB2ENR |= 0x00000800; 

    // 2. Configurar TIM1 para contar automáticamente.
    
    // Limpiamos los registros para asegurar una configuración limpia.
    TIM1_CR1  = 0x0000;
    TIM1_SMCR = 0x0000; // Poner a 0 deshabilita el modo esclavo. El timer usará el reloj interno.
    
    // a. Configurar el Preescalador (PSC - Prescaler).
    
    TIM1_PSC = 36535; 
    
    // b. Configurar el Valor de Auto-Recarga (ARR - Auto-Reload Register).
    // Este es el valor máximo hasta el que contará el timer antes de desbordarse y volver a 0.
    // 0xFFFF es el valor máximo para un contador de 16 bits (65535).
    TIM1_ARR = 0xFFFF;
    
    // 3. Habilitar el contador.
    // Se establece el bit 0 (CEN - Counter Enable) en el registro de control 1.
    TIM1_CR1 |= 0x0001; 
}



void Button_LED_Init(void) {
    // 1. Habilitar el reloj para el puerto GPIOB.
    RCC_APB2ENR |= 0x00000008; // Bit 3 (IOPBEN) para GPIOB.

    // 2. Configurar el pin PB0 como salida push-pull (para el LED/Backlight).
    // Se usa GPIOB_CRL (pines 0-7). Bits [3:0] para PB0.
    GPIOB_CRL &= ~0x0000000F; // Limpiamos los bits de configuración de PB0.
    GPIOB_CRL |=  0x00000002; // Configuramos como salida push-pull 2MHz ('0010' binario).

    // 3. Configurar el pin PB1 como entrada con pull-up (para el botón).
    // Se usa GPIOB_CRL. Bits [7:4] para PB1.
    GPIOB_CRL &= ~0x000000F0; // Limpiamos los bits de configuración de PB1.
    GPIOB_CRL |=  0x00000080; // Configuramos como entrada pull-up/down ('1000' binario).
    
    // Activamos la resistencia de PULL-UP poniendo el bit 1 en '1'.
    GPIOB_ODR |= 0x00000002;
}