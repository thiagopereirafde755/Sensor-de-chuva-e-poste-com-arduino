#include "arduino_secrets.h"

//desenvolvido por Thiago Pereira
#include <Servo.h>
#include <LiquidCrystal.h>

// POSTE
int ldrPin = A1; // Sensor pino LDR
int posteLed = 8; // Pino LED do poste de luz
int ldrThreshold = 800; // Limiar para acionar o poste de luz
  

// LED COM PALMA
int sensorSom = A2; // Pino do sensor de som
int ledSom = 10; // Pino do LED
int contPalmas = 0; // Contador de Palmas
unsigned long tempMaxSom = 1000; // Tempo mÃ¡ximo entre o pulso seguinte
unsigned long tempMinSom = 300; // Tempo mÃ­nimo entre o pulso seguinte
unsigned long compriSom = 100; // Comprimento do som esperado
unsigned long time;
unsigned long startTime = 0;

// ELEVADOR
const int buttonPin = A4;
const int motorPin1 = 6;
const int motorPin2 = 7;
int buttonState = 0;
int lastButtonState = 0;
int motorDirection = 0;

// UMIDADE
const int pinoSensorUmidade = A0; // Pino do sensor de umidade
const int bomba = 11;
unsigned long ultimaAcaoBomba = 0; // Armazena o Ãºltimo tempo que a bomba foi ligada ou desligada
const unsigned long atrasoBomba = 10000; // 10 segundos de atraso para ligar a bomba

// CHUVA
const int pinoSensorChuva = A3; // Pino do sensor de chuva
const int pinoServo = 9; // Pino do servo
Servo meuServo; // Servo

// RIO
const int triggerPin = 12;
const int echoPin = 13;
const int lcdRs = 5;
const int lcdEn = 4;
const int lcdD4 = 3;
const int lcdD5 = 2;
const int lcdD6 = 1;
const int lcdD7 = 0;
LiquidCrystal lcd(lcdRs, lcdEn, lcdD4, lcdD5, lcdD6, lcdD7);
int buzzer = A5;

void setup() {
    // Inicializa o monitor serial
    

    // POSTE
    pinMode(ldrPin, INPUT);
    pinMode(posteLed, OUTPUT);


    // LED COM PALMA
    pinMode(sensorSom, INPUT);
    pinMode(ledSom, OUTPUT);

    // ELEVADOR
    pinMode(buttonPin, INPUT);
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);

    // UMIDADE
    pinMode(pinoSensorUmidade, INPUT);
    pinMode(bomba, OUTPUT);

    // CHUVA
    pinMode(pinoSensorChuva, INPUT);
    meuServo.attach(pinoServo);

    // RIO
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzer, OUTPUT);

    lcd.begin(16, 2);
}

void loop() {
    controlarLuzComPalmas();
    controlarUmidade();
    controlarPosteDeLuz();
    ControlarElevador();
    controlarSensorChuva();
    medirDistanciaRio();
}

// LED COM PALMA
void controlarLuzComPalmas() {
    time = millis();
    unsigned long tempoAposPalma = time - startTime;
    int sensorValue = analogRead(sensorSom);

    // Serial.print("Valor do Sensor de Som: ");
    // Serial.println(sensorValue);

    if (tempoAposPalma >= compriSom && sensorValue < 200) {
        if (tempoAposPalma < tempMinSom || tempoAposPalma > tempMaxSom) {
            contPalmas = 0;
            startTime = millis();
        } else {
            contPalmas++;
            startTime = millis();
        }

        if (contPalmas >= 1) {
            digitalWrite(ledSom, !digitalRead(ledSom));
            contPalmas = 0;
        }
    }
}

// ELEVADOR
void ControlarElevador() {
    buttonState = digitalRead(buttonPin);

    if (buttonState != lastButtonState) {
        if (buttonState == LOW) {
            if (motorDirection == 0) {
                digitalWrite(motorPin1, HIGH);
                digitalWrite(motorPin2, LOW);
                delay(11000);
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                motorDirection = 1;
            } else {
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, HIGH);
                delay(2000);
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                motorDirection = 0;
            }
        }
        delay(50); // Atraso para evitar eventos mÃºltiplos devido ao salto
    }

    lastButtonState = buttonState;
}

// UMIDADE
void controlarUmidade() {
    int leituraUmidade = analogRead(pinoSensorUmidade);
    unsigned long tempoAtual = millis();

    if (leituraUmidade > 800) {
        if (tempoAtual - ultimaAcaoBomba >= atrasoBomba) {
            digitalWrite(bomba, LOW);
        }
    } else {
        digitalWrite(bomba, HIGH);
        ultimaAcaoBomba = tempoAtual; // Atualiza o tempo da Ãºltima aÃ§Ã£o da bomba
    }


}

// POSTE
void controlarPosteDeLuz() {
    int ldrValue = analogRead(ldrPin);
    digitalWrite(posteLed, ldrValue > ldrThreshold ? HIGH : LOW);
}

// CHUVA
void controlarSensorChuva() {
    int valorSensorChuva = analogRead(pinoSensorChuva);
    meuServo.write(valorSensorChuva < 800 ? 180 : 0);
}

// RIO
void medirDistanciaRio() {
    long duracao;
    float distancia;

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    duracao = pulseIn(echoPin, HIGH);
    distancia = duracao * 0.034 / 2;

     if (distancia >= 13) {
    lcd.setCursor(0, 0);
    lcd.print("     NORMAL   ");
    noTone(buzzer);
  } else if (distancia > 11) {
    lcd.setCursor(0, 0);
    lcd.print("     RISCO    ");
    noTone(buzzer);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("   ALAGAMENTO ");
    tone(buzzer, 1000);
  }
}
