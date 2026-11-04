const int buzzer = 8; // Pino usado para controlar o buzzer

void setup() {
  pinMode(buzzer, OUTPUT); // Define o pino como saída
  digitalWrite(buzzer, LOW); // Começa com o buzzer desligado
}

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

void loop() {
  // No código do jogo, chame cada função no momento correspondente:
  // somInicioJogo();
  // somPonto();
  // somSubidaNivel();
  // somFimJogo();
}