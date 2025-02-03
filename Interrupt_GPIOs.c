#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "include/ws2812.pio.h"
#include "include/ws2812.h"

// Configurações dos pinos 
const uint8_t led_r_pin = 13;    //Red=13, Blue=12, Green=11
const uint8_t led_b_pin = 12;    
const uint8_t led_g_pin = 11;    
const uint8_t button_a = 5;        // Botão A = 5, Botão B = 6 , BotãoJoy = 22
const uint8_t button_b = 6;        
const uint8_t intensidade = 100;
// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 1; // Intensidade do azul
// Definições para Matriz LED
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define tempo 2000
// Buffer para armazenar quais LEDs estão ligados matriz 5x5
bool led_buffer[NUM_PIXELS] = {
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
};
// Variáveis globais
static volatile uint state = 1;
static volatile int number = -1;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);
// Prototipo da função de inicialização dos pinos
void init_all_pins();
void count_number(int number);
void clear_buffer();

int main() {
    // inicialização de todos os pinos
    init_all_pins();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // limpa o buffer 
    //count_number(0);
    while (1) {
        // coloca o led red para piscar num periodo de 0.2 segundos, frequencia de 5hz
        gpio_put(led_r_pin, true);
        sleep_ms(100);
        gpio_put(led_r_pin, false);
        sleep_ms(100);
        
    }

}

void init_all_pins() {
    // Inicializações
    stdio_init_all();
    // Inicializa o LED
    gpio_init(led_r_pin);              // Inicializa o pino do LED RED
    gpio_set_dir(led_r_pin, GPIO_OUT); // Configura o pino como saída
    gpio_init(led_g_pin);              // Inicializa o pino do LED GREEN
    gpio_set_dir(led_g_pin, GPIO_OUT); // Configura o pino como saída
    gpio_init(led_b_pin);              // Inicializa o pino do LED BLUE
    gpio_set_dir(led_b_pin, GPIO_OUT); // Configura o pino como saída
    // Inicializa o botão
    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(button_a);          // Habilita o pull-up interno
    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(button_b);          // Habilita o pull-up interno
}

// Função contador
void count_number(int number) {
    if (number < 0 || number > 9) return;
    // vetor com indices dos numeros de 0 a 9
    int indices0[] = {1,2,3,6,8,11,13,16,18,21,22,23};
    int indices1[] = {1,2,3,7,12,17,22,16};
    int indices2[] = {1,2,3,6,13,12,11,18,21,22,23};
    int indices3[] = {1,2,3,8,11,12,13,18,21,22,23};
    int indices4[] = {1,8,11,12,13,16,18,21,23};
    int indices5[] = {1,2,3,8,11,12,13,16,21,22,23};
    int indices6[] = {1,2,3,6,8,11,12,13,16,21,22,23};
    int indices7[] = {1,8,11,18,21,22,23};
    int indices8[] = {1,2,3,6,8,11,12,13,16,18,21,22,23};
    int indices9[] = {1,2,3,8,11,12,13,16,18,21,22,23};
    int *indices_arr[] = {indices0, indices1, indices2, indices3, indices4, indices5, indices6, indices7, indices8, indices9};
    int tam_arr[] = {12,8,11,11,9,11,12,7,13,12};


    int tam = tam_arr[number];
    int *indices = indices_arr[number];   
    // atualiza buffer nos indices dos leds que vão ser acessos na matriz de leds
    for(int i = 0; i < tam_arr[number]; i++){
        led_buffer[indices[i]] = 1;
    }
}
// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d\n", state);
    
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) { // 200 ms de debouncing
        last_time = current_time; // Atualiza o tempo do último evento
        printf("Mudanca de Estado do Led. Estado = %d\n", state);
        if(gpio == button_a) {
            // incrementa numero exibido na matriz de leds  
            if(number<9) number+=1;
        } else if (gpio == button_b) {
            // decrementa numero exibido na matriz de leds
            if(number>0) number-=1;
        }
        // limpa o buffer 
        clear_buffer();
        // atualiza o buffer com novo numero
        count_number(number);
        // Alterna o estado do buffer
        set_one_led(led_r, led_g, led_b, intensidade); 
        state++;   // incrementa a variavel de verificação
    }
    

}

// Matriz de LEDs
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_one_led(uint8_t r, uint8_t g, uint8_t b, uint8_t intensidade) {
    r = r * intensidade;
    g = g * intensidade;
    b = b * intensidade;
    uint32_t color = urgb_u32(r,b,g);
    for(int i = 0; i < NUM_PIXELS; i++) {
        if (led_buffer[i]) {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void clear_buffer() {
    for(int i = 0; i < NUM_PIXELS; i++) {
        led_buffer[i] = 0;
    }
}
// end