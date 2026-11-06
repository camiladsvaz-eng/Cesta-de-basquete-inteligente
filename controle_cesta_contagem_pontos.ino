#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DISPLAY LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);


// ---------------- SENSOR E BUZZER ----------------
const int sensorIR1 = 2;
const int sensorIR2 = 3;
const int sensorIR3 = 5;
const int buzzer = 8;

const int botao = 14; // A0 no Arduino Uno


// ---------------- MOTOR DE PASSO ----------------
// Entradas da placa ULN2003

const int motorIN1 = 4;
const int motorIN2 = 6;
const int motorIN3 = 9;
const int motorIN4 = 10;


// Sequencia de meio passo do motor 28BYJ-48
const int sequencia[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};


// Aproximadamente 4096 meios passos por volta
const int passosPorVolta = 4096;

// Limites angulares
const int limiteNivel2 = 341; // Aproximadamente 30 graus
const int limiteNivel3 = 512; // Aproximadamente 45 graus

int indicePasso = 0;

long posicaoMotor = 0;
long alvoMotor = 0;

int sentidoMotor = 1;

unsigned long ultimoPasso = 0;
const unsigned long intervaloPasso = 3;


// ---------------- VARIAVEIS DO JOGO ----------------

int pontos = 0;
int nivel = 1;

int estadoAnterior1 = HIGH;
int estadoAnterior2 = HIGH;
int estadoAnterior3 = HIGH;

int estadoBotaoAnterior = HIGH;

unsigned long tempoInicio = 0;
unsigned long ultimoSegundo = 0;

bool jogoAtivo = false;
bool jogoFinalizado = false;


// ---------------- SOM DO JOGO ----------------

void bip(int duracao) {
  digitalWrite(buzzer, HIGH);
  delay(duracao);
  digitalWrite(buzzer, LOW);
}

void somInicioJogo() {
  bip(200);
  delay(350);

  bip(200);
  delay(350);

  bip(350);
}

void somPonto() {
  bip(100);
}

void somSubidaNivel() {
  bip(180);
  delay(150);
  bip(180);
}

void somFimJogo() {
  bip(1500);
}


// ---------------- CONTROLE DO MOTOR ----------------

// Aplica uma determinada etapa da sequencia
void aplicarPasso(int indice) {

  digitalWrite(motorIN1, sequencia[indice][0]);
  digitalWrite(motorIN2, sequencia[indice][1]);
  digitalWrite(motorIN3, sequencia[indice][2]);
  digitalWrite(motorIN4, sequencia[indice][3]);
}


// Desliga as quatro bobinas
void desligarMotor() {

  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
}


// Define o limite angular de acordo com o nivel
void atualizarAlvoMotor() {

  if (nivel == 1) {
    alvoMotor = 0;
  }

  else if (nivel == 2) {

    if (sentidoMotor == 1) {
      alvoMotor = limiteNivel2;
    }
    else {
      alvoMotor = -limiteNivel2;
    }
  }

  else if (nivel == 3) {

    if (sentidoMotor == 1) {
      alvoMotor = limiteNivel3;
    }
    else {
      alvoMotor = -limiteNivel3;
    }
  }
}


// Controla o movimento lateral da cesta
void controlarMotor() {

  // No nivel 1, a cesta permanece centralizada
  if (nivel == 1) {

    alvoMotor = 0;

    if (posicaoMotor == 0) {
      desligarMotor();
      return;
    }
  }

  else {

    // Quando chega ao limite, inverte o sentido
    if (posicaoMotor == alvoMotor) {

      sentidoMotor = -sentidoMotor;

      atualizarAlvoMotor();
    }
  }


  // Verifica se chegou o momento de executar outro passo
  if (millis() - ultimoPasso >= intervaloPasso) {

    ultimoPasso = millis();

    // Movimento para a direita
    if (posicaoMotor < alvoMotor) {

      indicePasso++;

      if (indicePasso > 7) {
        indicePasso = 0;
      }

      posicaoMotor++;
    }

    // Movimento para a esquerda
    else if (posicaoMotor > alvoMotor) {

      indicePasso--;

      if (indicePasso < 0) {
        indicePasso = 7;
      }

      posicaoMotor--;
    }

    else {
      return;
    }

    aplicarPasso(indicePasso);
  }
}


// ---------------- DISPLAY LCD ----------------

// Inicializacao do display
void iniciarDisplay() {

  Wire.begin();

  lcd.init();
  lcd.backlight();

  telaInicial();
}


// Tela inicial
void telaInicial() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("CESTINHAS CIN");

  lcd.setCursor(0, 1);
  lcd.print("APERTE PLAY!");
}


// Atualizacao durante o jogo
void atualizarDisplay() {

  unsigned long tempoAtual = millis();

  int tempoRestante =
    45 - ((tempoAtual - tempoInicio) / 1000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("PONTOS: ");
  lcd.print(pontos);

  lcd.setCursor(0, 1);
  lcd.print("NIVEL: ");
  lcd.print(nivel);

  lcd.setCursor(10, 1);
  lcd.print(tempoRestante);
  lcd.print("s");
}


// Mostrar mudanca de nivel
void mostrarNovoNivel() {

  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("NOVO NIVEL!");

  lcd.setCursor(3, 1);
  lcd.print("NIVEL ");
  lcd.print(nivel);

  delay(1000);

  atualizarDisplay();
}


// Tela final
void finalizarDisplay() {

  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("FIM DE JOGO!");

  lcd.setCursor(0, 1);
  lcd.print("PONTOS: ");
  lcd.print(pontos);
}


// ---------------- INICIO DO JOGO -----------------

void iniciarJogo() {

  Serial.println("Preparando partida...");

  somInicioJogo();

  pontos = 0;
  nivel = 1;

  estadoAnterior1 = digitalRead(sensorIR1);
  estadoAnterior2 = digitalRead(sensorIR2);
  estadoAnterior3 = digitalRead(sensorIR3);

  tempoInicio = millis();
  ultimoSegundo = 0;

  sentidoMotor = 1;
  atualizarAlvoMotor();

  jogoAtivo = true;

  Serial.println("JOGO INICIADO!");
  Serial.println("Tempo: 45 segundos");

  atualizarDisplay();
}


// ---------------- FIM DO JOGO ----------------

void finalizarJogo() {

  jogoAtivo = false;
  jogoFinalizado = true;

  desligarMotor();

  somFimJogo();

  Serial.println();
  Serial.println("===== FIM DE JOGO =====");
  Serial.print("Pontuacao final: ");
  Serial.println(pontos);
  Serial.println("=======================");

  finalizarDisplay();
}


// ---------------- CONFIGURACAO INICIAL ----------------

void setup() {

  Serial.begin(9600);

  pinMode(sensorIR1, INPUT);
  pinMode(sensorIR2, INPUT);
  pinMode(sensorIR3, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(botao, INPUT_PULLUP);

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorIN3, OUTPUT);
  pinMode(motorIN4, OUTPUT);

  digitalWrite(buzzer, LOW);

  desligarMotor();

  iniciarDisplay();
}


// ---------------- LOOP PRINCIPAL ----------------

void loop() {

  // Leitura do botao PLAY
  int leituraBotao = digitalRead(botao);

  if (leituraBotao == LOW &&
      estadoBotaoAnterior == HIGH) {

    if (jogoAtivo == false &&
        jogoFinalizado == false) {

      iniciarJogo();
    }
  }

  estadoBotaoAnterior = leituraBotao;


  // Se o jogo acabou, nao executa mais nada
  if (jogoFinalizado) {
    return;
  }


  if (jogoAtivo == true) {

    unsigned long tempoAtual = millis();


    // Verificar tempo da partida
    if (tempoAtual - tempoInicio >= 45000) {

      finalizarJogo();
      return;
    }


    // Atualizar cronometro
    unsigned long segundoAtual =
      (tempoAtual - tempoInicio) / 1000;

    if (segundoAtual != ultimoSegundo) {

      ultimoSegundo = segundoAtual;

      Serial.print("Tempo restante: ");
      Serial.print(45 - segundoAtual);
      Serial.println(" segundos");

      atualizarDisplay();
    }


    // Leitura dos tres sensores IR
    int leitura1 = digitalRead(sensorIR1);
    int leitura2 = digitalRead(sensorIR2);
    int leitura3 = digitalRead(sensorIR3);


    // Detectar uma nova cesta
    if ((leitura1 == LOW && estadoAnterior1 == HIGH) ||
        (leitura2 == LOW && estadoAnterior2 == HIGH) ||
        (leitura3 == LOW && estadoAnterior3 == HIGH)) {

      pontos++;

      Serial.print("CESTA! Pontos: ");
      Serial.println(pontos);

      somPonto();

      atualizarDisplay();


      // SUBIDA PARA O NIVEL 2
      if (pontos == 10) {

        nivel = 2;

        sentidoMotor = 1;
        atualizarAlvoMotor();

        Serial.println("NIVEL 2!");

        somSubidaNivel();

        mostrarNovoNivel();
      }


      // SUBIDA PARA O NIVEL 3
      else if (pontos == 20) {

        nivel = 3;

        atualizarAlvoMotor();

        Serial.println("NIVEL 3!");

        somSubidaNivel();

        mostrarNovoNivel();
      }
    }


    // Atualizar estados anteriores dos sensores
    estadoAnterior1 = leitura1;
    estadoAnterior2 = leitura2;
    estadoAnterior3 = leitura3;


    // Atualizar movimentacao do motor
    controlarMotor();
  }
}
