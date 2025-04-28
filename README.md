# Aluno: Wilton Lacerda Silva Júnior
## Matrícula: TIC370100193
# Video explicativo: https://youtu.be/VHi8iiUdq1w
# Ohmimetro
O objetivo do projeto é desenvolver um Ohmimetro funcional o qual analisa a resistencia e escreve o código de cor do resistor, na placa de desenvolvimento BitDogLab.
## Funcionalidades

- **Display OLED**
  - O display mostrará o valor da resistencia analisada e o código de cor.
- **1 Botão**
  - O botão utilizado tem a função de reiniciar a placa no modo de bootload.
- **UART**
  - A UART será responsavel por passar as informações para a usb conectada, para poder analisar sem o display OLED os valores.
- **1 ADC**
  - O ADC será responsavel para analisar a resistência.

# Requisitos
## Hardware:

- Raspberry Pi Pico W.
- 1 display ssd1306 com o sda na porta 14 e o scl na porta 15.
- 1 circuito para medir resistencia, com 2 resistores em serie, um que sabe a resistencia, com uma ponta conectada no pino 28, e outra perna no gnd, e outro resistor em serie com a outra perna no 3.3V

## Software:

- Ambiente de desenvolvimento VS Code com extensão Pico SDK.

# Instruções de uso
## Configure o ambiente:
- Certifique-se de que o Pico SDK está instalado e configurado no VS Code.
- Compile o código utilizando a extensão do Pico SDK.
## Teste:
- Utilize a placa BitDogLab para o teste. Caso não tenha, conecte os hardwares informados acima nos pinos correspondentes.

# Explicação do projeto:
## Contém:
- O projeto terá 1 meio de entrada: O resistor.
- Também contará com saída visual,  1 display de led 128x64.

## Funcionalidades:
- O programa medira a resistencia do resistor conectado a gpio 28.
- O valor da resistência sera mostrada tanto no display OLED quanto na UART USB.
- O código de cor do resistor será mostrado tanto no display OLED quanto na UART USB.
