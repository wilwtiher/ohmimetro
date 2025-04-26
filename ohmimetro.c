#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define ADC_PIN 28 // GPIO para o ohmimetro
#define Botao_A 5  // GPIO para botão A

int R_conhecido = 10000;   // Resistor de 10k ohm
float R_x = 0.0;           // Resistor desconhecido
float ADC_VREF = 3.31;     // Tensão de referência do ADC
int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)
const char* cores[] = { // Variavel para código de cores (5 caracteres cada)
    "Preto",   // 0
    "Marro",  // 1
    "Verme",// 2
    "Laran", // 3
    "Amare", // 4
    "Verde",   // 5
    "Azul.",    // 6
    "Viole", // 7
    "Cinza",   // 8
    "Branc"   // 9
};

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    if(gpio == botaoB){
        reset_usb_boot(0, 0);
    }
}

int main()
{
  // Para ser utilizado o modo BOOTSEL com botão B
  gpio_init(botaoB);
  gpio_set_dir(botaoB, GPIO_IN);
  gpio_pull_up(botaoB);
  gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  // Aqui termina o trecho para modo BOOTSEL com botão B

  
  gpio_init(Botao_A);
  gpio_set_dir(Botao_A, GPIO_IN);
  gpio_pull_up(Botao_A);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_t ssd;                                                // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  adc_init();
  adc_gpio_init(ADC_PIN); // GPIO 28 como entrada analógica

  float tensao;
  char str_x[5]; // Buffer para armazenar a string
  char str_y[5]; // Buffer para armazenar a string

  bool cor = true;
  while (true)
  {
    adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica

    float soma = 0.0f;
    for (int i = 0; i < 500; i++)
    {
      soma += adc_read();
      sleep_ms(1);
    }
    float media = soma / 500.0f;

      // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_RESOLUTION - adc_encontrado)
      R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

      // Conta para saber codigo de cor
      int ordem = 0;
      float resistencia = R_x;
      while (resistencia >= 100) {
          resistencia /= 10;
          ordem++;
      }
      while (resistencia < 10) {
          resistencia *= 10;
          ordem--;
      }
  
      int digito1 = (int)resistencia / 10;
      int digito2 = (int)resistencia % 10;
  
    sprintf(str_x, "%1.0f", media); // Converte o inteiro em string
    sprintf(str_y, "%1.0f", R_x);   // Converte o float em string

    // cor = !cor;
    //  Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
    ssd1306_line(&ssd, 3, 25, 123, 25, cor);           // Desenha uma linha
    ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
    ssd1306_draw_string(&ssd, "Ohmimetro", 8, 6); // Desenha uma string
    ssd1306_draw_string(&ssd, "Resistenc.", 8, 16);  // Desenha uma string
    ssd1306_draw_string(&ssd, str_y, 8, 26);  // Desenha uma string
    ssd1306_draw_string(&ssd, "Codig. de cor", 8, 36);          // Desenha uma string
    // codigo de cores
    ssd1306_draw_string(&ssd, cores[digito1], 8, 46);    // Desenha uma string
    ssd1306_draw_string(&ssd, ",", 48, 46);    // Desenha uma string
    ssd1306_draw_string(&ssd, cores[digito2], 54, 46);    // Desenha uma string
    ssd1306_draw_string(&ssd, ",", 94, 46);    // Desenha uma string
    ssd1306_draw_string(&ssd, cores[ordem], 8, 56);    // Desenha uma string
    ssd1306_draw_string(&ssd, ",", 48, 46);    // Desenha uma string
    ssd1306_draw_string(&ssd, "dourado.", 54, 46);    // Desenha uma string
    /*
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);           // Desenha uma linha vertical
    ssd1306_draw_string(&ssd, str_x, 8, 52);           // Desenha uma string
    ssd1306_draw_string(&ssd, str_y, 59, 52);          // Desenha uma string
    */
    ssd1306_send_data(&ssd);                           // Atualiza o display
    sleep_ms(700);
  }
}
