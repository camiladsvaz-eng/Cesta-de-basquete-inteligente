# 🏀 Cesta de Basquete Inteligente

Um jogo interativo de basquete em miniatura, desenvolvido com Arduino, que combina programação, eletrônica e mecânica para proporcionar uma experiência inspirada nas máquinas de basquete arcade.

## 📌 Sobre o projeto

A **Cesta de Basquete Inteligente** é um projeto desenvolvido para a disciplina de Introdução à Computação, com o objetivo de construir uma mini máquina de basquete interativa.

Inspirado nas tradicionais máquinas de basquete arcade, o jogo desafia os participantes a acertarem o maior número possível de arremessos dentro de um tempo limitado.

O diferencial está na evolução da dificuldade: conforme o jogador acumula pontos, a cesta começa a se movimentar lateralmente, exigindo maior precisão nos arremessos.

Todo o funcionamento é controlado por um Arduino, responsável pela leitura dos sensores, contagem de pontos, controle do tempo, movimentação da cesta e interação com o jogador.

## 🎮 Como funciona?

O jogo possui uma partida de **45 segundos**, durante a qual o jogador deve realizar o maior número possível de cestas.

A experiência é dividida em três níveis:

| Nível | Pontuação | Funcionamento |
|:---:|:---:|---|
| 🟢 1 | 0 a 9 pontos | A cesta permanece centralizada. |
| 🟡 2 | 10 a 19 pontos | A cesta começa a se movimentar lateralmente. |
| 🔴 3 | A partir de 20 pontos | A cesta aumenta sua amplitude de movimentação, tornando o desafio mais difícil. |

### Funcionalidades

- **Contagem automática de pontos:** três sensores infravermelhos identificam a passagem da bola pela cesta.
- **Cronômetro:** cada partida possui duração de 45 segundos.
- **Dificuldade progressiva:** o jogo possui três níveis, com aumento gradual da dificuldade.
- **Movimentação automática:** um motor de passo controla o movimento lateral da cesta nos níveis superiores.
- **Display LCD:** apresenta a pontuação, o nível atual, o tempo restante e as informações da partida.
- **Efeitos sonoros:** um buzzer emite sons para sinalizar o início do jogo, a pontuação, a mudança de nível e o encerramento.
- **Sistema de recordes:** o jogo armazena a maior pontuação na memória EEPROM do Arduino, preservando o recorde mesmo após o desligamento.

## 🔧 Componentes utilizados

| Componente | Função |
|---|---|
| Arduino Mega 260 | Controlador principal do jogo. |
| 3 sensores infravermelhos (IR) | Detectam a passagem da bola e permitem a contagem de pontos. |
| Motor de passo 28BYJ-48 | Realiza a movimentação lateral da cesta. |
| Driver ULN2003 | Controla o acionamento do motor de passo. |
| Display LCD 16x2 com módulo I2C | Exibe as informações do jogo. |
| Buzzer | Produz os efeitos sonoros. |
| Botão push button | Inicia e reinicia as partidas. |
| Fonte externa de 5V | Alimenta o motor de passo. |

## ⚙️ Tecnologias e bibliotecas

**Tecnologias:**
- Arduino;
- C/C++ (Arduino);
- Eletrônica e sistemas embarcados.

**Bibliotecas utilizadas:**

- `Wire.h`: comunicação I2C com o display.
- `LiquidCrystal_I2C.h`: controle do display LCD.
- `EEPROM.h`: armazenamento persistente do recorde.

As bibliotecas `Wire.h` e `EEPROM.h` fazem parte do ambiente Arduino. A biblioteca `LiquidCrystal_I2C.h` deve estar instalada na IDE.

## 🔌 Conexões do Arduino

A tabela apresenta as conexões definidas no código do projeto.

| Componente | Pino do Arduino Uno |
|---|:---:|
| Sensor IR 1 | 2 |
| Sensor IR 2 | 3 |
| Sensor IR 3 | 5 |
| Buzzer | 8 |
| Botão PLAY | A0 |
| Motor IN1 | 4 |
| Motor IN2 | 6 |
| Motor IN3 | 9 |
| Motor IN4 | 10 |
| Display LCD | A4 (SDA) / A5 (SCL) |

**Atenção:** o motor de passo deve ser alimentado por uma fonte externa de 5V, com o GND da fonte conectado ao GND do Arduino para garantir uma referência elétrica comum.

O display LCD utiliza comunicação I2C e está configurado no endereço `0x27`.

## 🚀 Como executar o projeto

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software).
2. Instale a biblioteca `LiquidCrystal_I2C`.
3. Conecte os componentes ao Arduino conforme a tabela de conexões.
4. Abra o arquivo `controle_cesta_contagem_pontos.ino` na Arduino IDE.
5. Conecte o Arduino Uno ao computador.
6. Selecione a placa **Arduino Uno** e a porta correspondente.
7. Faça o upload do código.
8. Aguarde a inicialização do display e pressione o botão PLAY para começar!

## 🧠 Conceitos de computação aplicados

O desenvolvimento do projeto envolve diferentes conceitos de programação e computação:

- **Estruturas condicionais:** utilizadas para controlar os níveis, identificar pontuações e gerenciar os estados do jogo.
- **Estruturas de repetição:** utilizadas no monitoramento contínuo dos sensores e no controle do funcionamento do jogo.
- **Funções:** organização do código em diferentes responsabilidades, como controle do motor, atualização do display e gerenciamento das partidas.
- **Manipulação de variáveis:** controle da pontuação, tempo, nível e estados do jogo.
- **Programação de hardware:** interação entre o software e os componentes eletrônicos.
- **Controle de tempo:** utilização da função `millis()` para gerenciar a duração da partida e a movimentação do motor.
- **Armazenamento de dados:** utilização da memória EEPROM para preservar o recorde.

## 🎯 Objetivo acadêmico

O projeto busca aplicar, de maneira prática e interativa, os conhecimentos de programação e eletrônica desenvolvidos na disciplina de Introdução à Computação.

Além da implementação de um jogo funcional, a proposta envolve a integração entre diferentes componentes de hardware e software, explorando a resolução de problemas e o desenvolvimento de sistemas interativos.

## 👥 Equipe

**Desenvolvido por estudantes da Universidade Federal de Pernambuco (UFPE).**

Projeto acadêmico da disciplina de Introdução à Computação.

---

<div align="center">

**🏀 CESTINHAS DO CIn**

*Prepare sua mira, desafie seus limites e bata o recorde!*

</div>
