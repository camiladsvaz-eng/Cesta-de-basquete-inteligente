const int sensorIR = 2;
// Conectei o nosso sensorIR (infravermelho) no pino 2 do arduino. Defini como const por ser algo que não muda ao longo do projeto e int por ser inteiro

int pontos = 0; // Variável que contará os pontos da partida (quantas vezes a bola passará pela cesta)
int estadoAnterior = HIGH; // Nenhum objeto conectado = HIGH e Objeto detectado = LOW; Essa variável guarda a leitura anterior do sensor para que a mesma bolinha não seja contada mais de uma vez

const int vel = 11;
const int motorA = 4;
const int motorB = 7;

unsigned long tempoAnterior = 0;
bool sentido = false;

void setup() {
  pinMode(sensorIR, INPUT);
  pinMode(vel, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int leitura = digitalRead(sensorIR);

  if (leitura == LOW && estadoAnterior == HIGH) {
 // verificamos as duas condições simultaneamente
 // objeto detectado && sem objeto detectado
    pontos++;

    Serial.print("Pontos: ");
    Serial.println(pontos);
  }
  estadoAnterior = leitura;

  // Controle do motor
  if (pontos < 10) {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    analogWrite(vel, 0);
  }

  else {

    int velocidade;

    if (pontos >= 10 && pontos < 20) {
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