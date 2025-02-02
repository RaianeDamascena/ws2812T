#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

// Padrões para os números de 0 a 9 em uma matriz 5x5 (cada número é representado por um array de 5 valores)
const uint8_t number_patterns[10][5] = {
    {0x1F, 0x11, 0x11, 0x11, 0x1F},  // 0
    {0x04, 0x1C, 0x04, 0x04, 0x1F},  // 1
    {0x1F, 0x01, 0x1F, 0x10, 0x1F},  // 2
    {0x1F, 0x01, 0x1F, 0x01, 0x1F},  // 3
    {0x11, 0x11, 0x1F, 0x01, 0x01},  // 4
    {0x1F, 0x10, 0x1F, 0x01, 0x1F},  // 5
    {0x1F, 0x10, 0x1F, 0x11, 0x1F},  // 6
    {0x1F, 0x01, 0x04, 0x04, 0x04},  // 7
    {0x1F, 0x11, 0x1F, 0x11, 0x1F},  // 8
    {0x1F, 0x11, 0x1F, 0x01, 0x1F}   // 9
};

// Definindo pinos para os botões
#define BUTTON_A_PIN 5  // Botão A na GPIO 5
#define BUTTON_B_PIN 6  // Botão B na GPIO 6
#define LED_PIN 13      // LED onboard na GPIO 13

// Variáveis globais para controle do LED e cor
uint8_t selected_number = 0;  // Número selecionado (0 a 9)
uint8_t selected_r = 255;     // Intensidade do vermelho (0 a 255)
uint8_t selected_g = 0;       // Intensidade do verde (0 a 255)
uint8_t selected_b = 0;       // Intensidade do azul (0 a 255)

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

uint32_t led_buffer[NUM_PIXELS] = {0}; // Buffer para armazenar as cores de todos os LEDs

// Função para atualizar o buffer de LEDs com o número selecionado
void update_led_buffer() {
    // Apaga todos os LEDs
    for (int i = 0; i < NUM_PIXELS; i++) {
        led_buffer[i] = 0; // Desliga todos os LEDs
    }

    // Configura os LEDs de acordo com o padrão do número selecionado
    uint8_t pattern_row;
    for (int row = 0; row < 5; row++) {
        pattern_row = number_patterns[selected_number][row];
        for (int col = 0; col < 5; col++) {
            // Se o bit correspondente à coluna estiver ativo, acende o LED
            if (pattern_row & (1 << (4 - col))) {
                int led_index = row * 5 + col;
                led_buffer[led_index] = urgb_u32(selected_r, selected_g, selected_b);
            }
        }
    }
}

// Função para enviar o estado de todos os LEDs para a matriz
void set_leds_from_buffer() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(led_buffer[i]);
    }
}

int main() {
    stdio_init_all();
    printf("WS2812 5x5 Matrix - Display Numbers 0 to 9\n");

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Inicializando os pinos dos botões A e B
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);  // Configura o botão A com resistor de pull-up

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);  // Configura o botão B com resistor de pull-up

    // Inicializando o pino do LED onboard
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Variáveis para o controle do LED piscando
    bool red_led_on = false;
    uint32_t last_toggle_time = 0;
    const uint32_t toggle_interval = 100; // Intervalo de 100ms para piscar 5 vezes por segundo

    while (1) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        // Lógica para piscar o LED da GPIO 13 (LED onboard)
        if (current_time - last_toggle_time >= toggle_interval) {
            red_led_on = !red_led_on;  // Alterna o estado do LED
            gpio_put(LED_PIN, red_led_on); // Atualiza o estado do LED onboard
            last_toggle_time = current_time;
        }

        // Atualiza o buffer com o número selecionado
        update_led_buffer();

        // Envia o estado do buffer para a matriz de LEDs
        set_leds_from_buffer();

        // Verifica se o botão A foi pressionado para incrementar
        if (!gpio_get(BUTTON_A_PIN)) {
            selected_number = (selected_number + 1) % 10; // Incrementa o número (0 a 9)
            sleep_ms(200);  // Debounce simples
        }

        // Verifica se o botão B foi pressionado para decrementar
        if (!gpio_get(BUTTON_B_PIN)) {
            selected_number = (selected_number + 9) % 10; // Decrementa o número (0 a 9)
            sleep_ms(200);  // Debounce simples
        }

        sleep_ms(100); // Espera 100ms antes de fazer o próximo ciclo
    }

    return 0;
}
