#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);


// SENSOR E BUZZER
const int sensorIR1 = 2;
const int sensorIR2 = 3;
const int sensorIR3 = 5;
const int buzzer = 8;

int pontos = 0;

int estadoAnterior1 = HIGH;
int estadoAnterior2 = HIGH;
int estadoAnterior3 = HIGH;

int nivel = 1;

const int vel = 11;
const int motorA = 4;
const int motorB = 7;

unsigned long tempoInicio = 0;
unsigned long tempoAnterior = 0;
unsigned long ultimoSegundo = 0;

bool sentido = false;
bool jogoAtivo = false;
bool jogoFinalizado = false;


// ----------- SOM DO JOGO --------------
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


// ------------------ CONTROLE DO MOTOR ----------------
void pararMotor() {
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
  analogWrite(vel, 0);
}

void controlarMotor() {

  if (nivel == 1) {
    pararMotor();
  }

  else {

    int velocidade;

    if (nivel == 2) {
      velocidade = 150;
    }
    else {
      velocidade = 255;
    }

    analogWrite(vel, velocidade);

    if (millis() - tempoAnterior >= 3000) {

      tempoAnterior = millis();

      if (sentido == false) {
        digitalWrite(motorA, HIGH);
        digitalWrite(motorB, LOW);
        sentido = true;
      }
      else {
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, HIGH);
        sentido = false;
      }
    }
  }
}


// ---------------- DISPLAY LCD ----------------

// INICIALIZAÇÃO DO DISPLAY
void iniciarDisplay() {

  Wire.begin();
  lcd.init();
  lcd.backlight();

  telaInicial();
}


// TELA INICIAL
void telaInicial() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("CESTINHAS CIN");

  lcd.setCursor(0, 1);
  lcd.print("APERTE PLAY!");
}


// ATUALIZAR DISPLAY DURANTE O JOGO
void atualizarDisplay() {

  unsigned long tempoAtual = millis();

  int tempoRestante = 45 - ((tempoAtual - tempoInicio) / 1000);

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

// MOSTRAR MUDANÇA DE NÍVEL
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


// TELA DE FIM DE JOGO
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
  tempoAnterior = millis();

  jogoAtivo = true;

  Serial.println("JOGO INICIADO!");
  Serial.println("Tempo: 45 segundos");

  // ATUALIZAR LCD AO INICIAR O JOGO
  atualizarDisplay();
}


// ----------------- FIM DO JOGO ------------------------
void finalizarJogo() {

  jogoAtivo = false;
  jogoFinalizado = true;

  pararMotor();

  somFimJogo();

  Serial.println();
  Serial.println("===== FIM DE JOGO =====");
  Serial.print("Pontuacao final: ");
  Serial.println(pontos);
  Serial.println("=======================");

  // MOSTRAR RESULTADO FINAL NO LCD
  finalizarDisplay();
}


// ---------------- CONFIGURACAO INICIAL ----------------
void setup() {

  Serial.begin(9600);

  pinMode(sensorIR1, INPUT);
  pinMode(sensorIR2, INPUT);
  pinMode(sensorIR3, INPUT);
  pinMode(vel, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);

  pararMotor();

  // INICIALIZAR LCD
  iniciarDisplay();

  iniciarJogo();
}


// ---------------- LOOP PRINCIPAL ----------------
void loop() {

  if (jogoFinalizado) {
    return;
  }

  if (jogoAtivo == true) {

    unsigned long tempoAtual = millis();

    // VERIFICAR TEMPO DO JOGO
    if (tempoAtual - tempoInicio >= 45000) {
      finalizarJogo();
      return;
    }

    // ATUALIZAR TEMPO NO MONITOR SERIAL
    unsigned long segundoAtual =
      (tempoAtual - tempoInicio) / 1000;

    if (segundoAtual != ultimoSegundo) {

      ultimoSegundo = segundoAtual;

      Serial.print("Tempo restante: ");
      Serial.print(45 - segundoAtual);
      Serial.println(" segundos");

      atualizarDisplay();
    }

    // LEITURA DO SENSOR IR
    int leitura1 = digitalRead(sensorIR1);
    int leitura2 = digitalRead(sensorIR2);
    int leitura3 = digitalRead(sensorIR3);


    if ((leitura1 == LOW && estadoAnterior1 == HIGH) or 
        (leitura2 == LOW && estadoAnterior2 == HIGH) or
        (leitura3 == LOW && estadoAnterior3 == HIGH)) {

      pontos++;

      Serial.print("CESTA! Pontos: ");
      Serial.println(pontos);

      somPonto();

      // ATUALIZAR LCD APÓS MARCAR CESTA
      atualizarDisplay();

      // SUBIR PARA O NIVEL 2
      if (pontos == 10) {

        nivel = 2;
        tempoAnterior = millis();

        Serial.println("NIVEL 2!");

        somSubidaNivel();

        // MOSTRAR NOVO NÍVEL NO LCD
        mostrarNovoNivel();
      }

      // SUBIR PARA O NIVEL 3
      else if (pontos == 20) {

        nivel = 3;
        tempoAnterior = millis();

        Serial.println("NIVEL 3!");

        somSubidaNivel();

        // MOSTRAR NOVO NÍVEL NO LCD
        mostrarNovoNivel();
      }
    }

    estadoAnterior1 = leitura1;
    estadoAnterior2 = leitura2;
    estadoAnterior3 = leitura3;

    controlarMotor();
  }
}
