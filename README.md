# Projeto de Interrupções e Controle de LEDs com RP2040 - EmbarcaTech

## Descrição

Este projeto foi desenvolvido para consolidar a compreensão dos conceitos relacionados ao uso de interrupções no microcontrolador RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab. O objetivo é implementar um sistema funcional que manipule LEDs e botões utilizando interrupções, debouncing e controle de LEDs endereçáveis WS2812.

## Objetivos

- Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
- Identificar e corrigir o fenômeno do bouncing em botões por meio de debouncing via software.
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Fixar o estudo do uso de resistores de pull-up internos em botões de acionamento.
- Desenvolver um projeto funcional que combine hardware e software.

## Componentes

O projeto utiliza os seguintes componentes conectados à placa BitDogLab:

- **Matriz 5x5 de LEDs (endereçáveis) WS2812**: conectada à GPIO 7.
- **LED RGB**: com os pinos conectados às GPIOs 11, 12 e 13.
- **Botão A**: conectado à GPIO 5.
- **Botão B**: conectado à GPIO 6.

## Funcionalidades

O projeto implementa as seguintes funcionalidades:

1. **LED RGB**: O LED vermelho pisca continuamente 5 vezes por segundo.
2. **Botão A**: Incrementa o número exibido na matriz de LEDs a cada pressionamento.
3. **Botão B**: Decrementa o número exibido na matriz de LEDs a cada pressionamento.
4. **Matriz de LEDs WS2812**: Exibe números de 0 a 9 com efeitos visuais. O formato dos números pode ser estilo digital (segmentos iluminados), mas pode ser uma abordagem criativa, desde que o número seja claramente identificável.

## Requisitos

- **Uso de interrupções**: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
- **Debouncing**: O tratamento do bouncing dos botões deve ser feito via software.
- **Controle de LEDs**: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle.
- **Organização do código**: O código deve estar bem estruturado e comentado para facilitar o entendimento.

## Como Executar

1. Conecte a placa BitDogLab ao computador via cabo micro-
2. Compile e faça o upload do código para o microcontrolador.
3. Ao pressionar os botões A a matriz de LEDs exibirá o número "0" iniciando a contagem, e a partir disso os números podem ser incrementados ou decrementados entre 0 e 9. O LED RGB piscará conforme o esperado com período de 200 ms.

## Simulação
É possível testar o projeto no ambiente de simulação wokwi, ao clicar no arquivo "diagram.json" e iniciar a simulação.

