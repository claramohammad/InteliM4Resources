#include <Arduino.h>

const int ledPins[] = {7, 6, 5, 4};
const int buttonPins[] = {13, 12, 11, 10};
const int resetButtonPin = 8;
const int buzzerPin = 3;

int sequence[100]; // Array para armazenar a sequência gerada aleatoriamente
int playerSequence[100]; // Array para armazenar a sequência inserida pelo jogador
int sequenceLength = 0; // Comprimento da sequência gerada
int playerTurn = false; // Indica se é a vez do jogador
int score = 0;
bool playerMadeMistake = false;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Inicializa a semente para números aleatórios
  randomSeed(analogRead(0));

  // Aguarde o botão de reset ser pressionado para iniciar
  while (digitalRead(resetButtonPin) == HIGH) {
    // Aguarde
  }
}

void loop() {
  // Verifique o estado do botão de reset
  if (digitalRead(resetButtonPin) == LOW) {
    delay(50); // Pequeno atraso para debouncing
    if (digitalRead(resetButtonPin) == LOW) { // Confirma que o botão ainda está pressionado
      restartGame(); // Reinicie o jogo quando o botão de reset for pressionado
      while (digitalRead(resetButtonPin) == LOW) {
        // Aguarde até que o botão seja liberado para evitar reinícios repetidos
      }
      delay(50); // Outro pequeno atraso para debouncing
    }
  }

  if (!playerTurn) {
    addRandomColorToSequence();
    showSequence();
    playerTurn = true;
  }

  for (int i = 0; i < sequenceLength; i++) {
    int buttonPressed = waitForButtonPress();
    if (buttonPressed == sequence[i]) {
      // Botão correto pressionado
      playTone(500, 200);
      delay(300);
    } else {
      // Botão errado pressionado
      playerTurn = false;
      playerMadeMistake = true;
      playMistakeTone();
      score = 0;
      sequenceLength = 0;
      playerSequence[0] = -1;
      break;
    }

    if (i == sequenceLength - 1) {
      // O jogador completou com sucesso a sequência
      score++;
      delay(500);
      playerTurn = false;
      playerMadeMistake = false;
    }
  }
}

void addRandomColorToSequence() {
  int randomNumber = random(0, 4);
  sequence[sequenceLength] = ledPins[randomNumber];
  sequenceLength++;
}

void showSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(sequence[i], HIGH);
    playTone(500, 200);
    delay(500);
    digitalWrite(sequence[i], LOW);
    delay(200);
  }
}

int waitForButtonPress() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        while (digitalRead(buttonPins[i]) == LOW) {
          // Aguarde o botão ser liberado
        }
        return ledPins[i];
      }
    }
  }
}

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}

void playMistakeTone() {
  if (playerMadeMistake) {
    tone(buzzerPin, 200, 500); // Tocar um som diferente para erros
    delay(500);
    noTone(buzzerPin);
  }
}

void restartGame() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  sequenceLength = 0;
  playerTurn = false;
  score = 0;
  playerMadeMistake = false;
  playerSequence[0] = -1;
}
