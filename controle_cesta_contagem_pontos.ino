const int sensorIR = 2; // Conectei o nosso sensorIR (infravermelho) no pino 2 do arduino. Defini como const por ser algo que não muda ao longo do projeto e int por ser inteiro
const int buzzer = 8;// Pino usado para controlar o buzzer

int pontos = 0; // Variável que contará os pontos da partida (quantas vezes a bola passará pela cesta)
int estadoAnterior = HIGH; // Nenhum objeto conectado = HIGH e Objeto detectado = LOW; Essa variável guarda a leitura anterior do sensor para que a mesma bolinha não seja contada mais de uma vez

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
// Produz um bip com a duração recebida, em milissegundos
void bip(int duracao) {
  digitalWrite(buzzer, HIGH); // Liga o buzzer
  delay(duracao);             // Mantém o som pelo tempo indicado
  digitalWrite(buzzer, LOW);  // Desliga o buzzer
}

// 1) Início do jogo: três bips, sendo o último mais longo
void somInicioJogo() {
  bip(200);
  delay(350); // Pausa entre os bips

  bip(200);
  delay(350);

  bip(350);
}

// 2) Ponto marcado: um bip curto
void somPonto() {
  bip(100);
}

// 3) Subida de nível: dois bips
void somSubidaNivel() {
  bip(180);
  delay(150); // Pausa entre os dois bips
  bip(180);
}

// 4) Fim do jogo: um bip longo de 1,5 segundo
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

    // Alterna o sentido a cada 3 segundos
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
// ---------------- INICIO DO JOGO -----------------
void iniciarJogo() {

  Serial.println("Preparando partida...");

  somInicioJogo();

  pontos = 0;
  nivel = 1;

  estadoAnterior = digitalRead(sensorIR);

  tempoInicio = millis();
  ultimoSegundo = 0;
  tempoAnterior = millis();

  jogoAtivo = true;

  Serial.println("JOGO INICIADO!");
  Serial.println("Tempo: 45 segundos");
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
}


void setup() {
  Serial.begin(9600);
  
  pinMode(sensorIR, INPUT);
  pinMode(vel, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(buzzer, OUTPUT); // Define o pino como saída
  digitalWrite(buzzer, LOW); // Começa com o buzzer desligado

  pararMotor();
  iniciarJogo();
}


void loop() {

  // Se o jogo acabou, nao executa mais nada
  if (jogoFinalizado) {
    return;
  }

  if (jogoAtivo == true) {

    unsigned long tempoAtual = millis();

    // Verifica se os 45 segundos acabaram
    if (tempoAtual - tempoInicio >= 45000) {
      finalizarJogo();
      return;
    }

    // Atualiza o tempo no Monitor Serial
    unsigned long segundoAtual =
      (tempoAtual - tempoInicio) / 1000;

    if (segundoAtual != ultimoSegundo) {

      ultimoSegundo = segundoAtual;

      Serial.print("Tempo restante: ");
      Serial.print(45 - segundoAtual);
      Serial.println(" segundos");
    }

    // Leitura do sensor IR
    int leitura = digitalRead(sensorIR);

    // Detecta uma nova cesta
    if (leitura == LOW && estadoAnterior == HIGH) {

      pontos++;

      Serial.print("CESTA! Pontos: ");
      Serial.println(pontos);

      somPonto();

      // A cada 10 pontos, aumenta o nivel
      if (pontos == 10) {

        nivel = 2;
        tempoAnterior = millis();

        Serial.println("NIVEL 2!");
        somSubidaNivel();
      }

      else if (pontos == 20) {

        nivel = 3;
        tempoAnterior = millis();

        Serial.println("NIVEL 3!");
        somSubidaNivel();
      }
    }

    estadoAnterior = leitura;

    // Atualiza o motor
    controlarMotor();
  }
}
