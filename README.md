# Controle de LEDs com Botão e Temporizadores – RP2040  

## Descrição  
Este repositório contém um programa para o **Raspberry Pi Pico (RP2040)** que aciona três **LEDs (azul, vermelho e verde)** quando um botão é pressionado. Os LEDs se desligam automaticamente após tempos diferentes, utilizando **temporizadores (alarms)**.  

O funcionamento é o seguinte:  
- Quando o botão é pressionado, os **três LEDs** acendem simultaneamente.  
- Cada LED é desligado após um tempo específico:  
  - **LED azul**: apaga após **3 segundos**.  
  - **LED vermelho**: apaga após **6 segundos**.  
  - **LED verde**: apaga após **9 segundos**.  
- O botão só pode acende os LEDs novamente após todos os LEDs serem desligados.  
