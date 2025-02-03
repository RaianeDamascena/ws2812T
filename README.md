## **Funcionalidade**
Este projeto implementa um sistema de controle para uma matriz de LEDs WS2812 (NeoPixel) em uma grade de 5x5, exibindo os números de 0 a 9. O sistema é interativo, permitindo ao usuário navegar entre os números através de dois botões físicos conectados ao Raspberry Pi Pico. Um LED na GPIO 13 também pisca continuamente para indicar o funcionamento do sistema.

---

## **Objetivo**
O código serve para demonstrar o controle eficiente de LEDs WS2812 em projetos de sistemas embarcados, utilizando microcontroladores. Além de controlar os LEDs, permite interação com o usuário através de botões, mostrando aplicações práticas de eletrônica digital.

---

## **Como Utilizar**

### **Materiais Necessários:**
- Raspberry Pi Pico
- Matriz WS2812 (NeoPixel) com 25 LEDs
- 2 Botões push-button
- LED comum conectado à GPIO 13 (opcional)
- Protoboard e jumpers para conexões

### **Montagem:**
1. Conecte a matriz de LEDs WS2812 ao pino 7 (GPIO) do Raspberry Pi Pico.
2. Conecte o Botão A ao pino 5 (GPIO) com resistor de pull-up.
3. Conecte o Botão B ao pino 6 (GPIO) com resistor de pull-up.
4. Conecte o LED indicador (opcional) ao pino 13 (GPIO).

### **Execução:**
1. Carregue o código no Raspberry Pi Pico.
2. Alimente o microcontrolador.
3. Pressione o Botão A para incrementar o número exibido na matriz de LEDs.
4. Pressione o Botão B para decrementar o número exibido.

---

## **Descrição do Código**
- **Definição de Padrões Numéricos:** Cada número de 0 a 9 é representado por uma matriz binária, onde cada bit indica se um LED está aceso ou apagado.
- **Funções Auxiliares:**
  - `put_pixel()`: Envia dados para um LED.
  - `urgb_u32()`: Converte valores RGB para o formato aceito pelo WS2812.
  - `update_led_buffer()`: Atualiza o buffer de LEDs com base no número selecionado.
  - `set_leds_from_buffer()`: Envia os dados do buffer para a matriz.
- **Loop Principal:**
  - Pisca o LED indicador na GPIO 13.
  - Atualiza os LEDs da matriz com o número selecionado.
  - Detecta pressionamentos dos botões para alterar o número exibido.

---

 **Demonstração**
Confira o vídeo de demonstração do projeto no seguinte link: [Clique aqui para acessar o vídeo](https://drive.google.com/file/d/1WsfoxS9Y688fGdihCx_7IPVyKcbc0TXI/view?usp=drive_link)

---

 **Desenvolvedor**
- **Nome:** Raiane Damascena da Paixão  
- **E-mail:** [rai.paixao2012@gmail.com](mailto:rai.paixao2012@gmail.com)

---
Este README documenta todas as informações necessárias para compreensão e execução deste projeto de controle de LEDs com um sistema embarcado.

