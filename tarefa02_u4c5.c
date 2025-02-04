#include <stdio.h>       // Biblioteca padrão do C para entrada e saída de dados (printf, scanf, etc.).
#include "pico/stdlib.h" // Biblioteca para controle de GPIO, temporização e comunicação serial no Raspberry Pi Pico.
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes.

// Definição dos pinos GPIO para os LEDs.
const uint blue = 11;      // Pino GPIO 11 para o LED azul.
const uint red = 12;       // Pino GPIO 12 para o LED vermelho.
const uint green = 13;     // Pino GPIO 13 para o LED verde.
const uint BUTTON_PIN = 5; // Pino GPIO 5 para o botão de ativação.

// Variável global que indica se o LED verde está aceso.
// Essa variável evita que o botão ative os LEDs repetidamente enquanto já estão acesos.
bool green_led_active = false;

// Variável para armazenar o tempo em que o LED deve ser desligado (não utilizada diretamente neste código).
absolute_time_t turn_off_time; 

// Função de callback que será chamada para desligar o LED verde após um tempo programado.
int64_t turn_off_green_callback(alarm_id_t id, void *user_data)
{
        printf("Desligando LED verde\n");
        gpio_put(green, false);  // Define o pino do LED verde como nível baixo (desligado).
        green_led_active = false; // Permite que o botão possa ser pressionado novamente para acender os LEDs.
        return 0; // Retorna 0 para indicar que o alarme não deve ser reagendado.
}

// Função de callback para desligar o LED azul após um tempo programado.
int64_t turn_off_blue_callback(alarm_id_t id, void *user_data)
{
        printf("Desligando LED azul\n");
        gpio_put(blue, false); // Define o pino do LED azul como nível baixo (desligado).
        return 0; // Retorna 0 para indicar que o alarme não será reagendado.
}

// Função de callback para desligar o LED vermelho após um tempo programado.
int64_t turn_off_red_callback(alarm_id_t id, void *user_data)
{
        printf("Desligando LED vermelho\n");
        gpio_put(red, false); // Define o pino do LED vermelho como nível baixo (desligado).
        return 0; // Retorna 0 para indicar que o alarme não será reagendado.
}

int main()
{
    // Inicializa a comunicação serial para permitir a saída de dados no terminal (depuração com printf).
    stdio_init_all();

    // Configuração dos pinos dos LEDs como saída digital.
    gpio_init(blue);
    gpio_init(red);
    gpio_init(green);
    gpio_set_dir(blue, GPIO_OUT);
    gpio_set_dir(red, GPIO_OUT);
    gpio_set_dir(green, GPIO_OUT);

    // Configuração do pino do botão como entrada digital.
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Habilita o resistor pull-up interno no botão para garantir que ele tenha um nível lógico alto quando não pressionado.
    gpio_pull_up(BUTTON_PIN);

    // Loop principal do programa: verifica continuamente o estado do botão.
    while (true)
    {
        // Verifica se o botão foi pressionado (nível baixo no pino) e se os LEDs não estão ativos.
        if (gpio_get(BUTTON_PIN) == 0 && !green_led_active)
        {
            // Aguarda 50 ms para debounce, evitando leituras errôneas causadas por oscilações mecânicas no botão.
            sleep_ms(50);

            // Confirma se o botão ainda está pressionado após o debounce.
            if (gpio_get(BUTTON_PIN) == 0)
            {
                // Liga todos os LEDs (configura os pinos como nível alto).
                gpio_put(blue, true);
                gpio_put(red, true);
                gpio_put(green, true);

                // Marca que os LEDs estão ativos, impedindo novas ativações até que sejam desligados.
                green_led_active = true;

                // Configura temporizadores para desligar os LEDs em momentos diferentes.
                add_alarm_in_ms(3000, turn_off_blue_callback, NULL, true);  // Desliga o LED azul após 3 segundos.
                add_alarm_in_ms(6000, turn_off_red_callback, NULL, true);   // Desliga o LED vermelho após 6 segundos.
                add_alarm_in_ms(9000, turn_off_green_callback, NULL, true); // Desliga o LED verde após 9 segundos.
            }
        }

        // Aguarda 10 ms antes da próxima verificação do botão, reduzindo o uso da CPU.
        sleep_ms(10);
    }

    return 0; // O programa nunca chegará aqui devido ao loop infinito.
}
