#include "arduino_secrets.h"


//Desenvolvido por Thiago Pereira
// Define o pino do sensor de chuva
const int pinoSensorChuva = A0;

int ledverde = 2;
int ledvermelho = 3;
int buzzer =4; 

int LED = 8; // O LED esta¡
 conectado ao pino 9
int LDR = A5; // O LDR esta conectado ao pino A0

int valor_sensor = 0;       
int valor_limite = 1000;

void setup() {
  

  // Inicializa o pino do sensor de chuva como entrada
  pinMode(pinoSensorChuva, INPUT);
pinMode(ledverde, OUTPUT);
pinMode(ledvermelho, OUTPUT);
pinMode(buzzer, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  // Inicializa a comunicação serial
  Serial.begin(9600);
}

void loop() {
  chuva();
  poste();

 

}
void chuva(){
 // LÃª o valor do sensor de chuva
  int valorSensorChuva = analogRead(pinoSensorChuva);

  // Envia o valor do sensor de chuva para a porta serial
  Serial.print("Valor do sensor de chuva: ");
  Serial.println(valorSensorChuva);


  if (valorSensorChuva < 900) {
digitalWrite(ledvermelho, HIGH);
digitalWrite(ledverde, LOW);
tone(buzzer, 500);
  } else {
digitalWrite(ledverde, HIGH);
digitalWrite(ledvermelho, LOW);
  noTone(buzzer);
  }
}
void poste(){
    valor_sensor = analogRead(LDR);
  
                  
  
  // Controle do LED com base no valor do sensor
  if (valor_sensor <= valor_limite) { // Se o valor do sensor for menor ou igual ao valor limite
    digitalWrite(LED, LOW); // O LED APAGA (estÃ¡ claro)
   
  } else { // Se for maior que o valor limite
    digitalWrite(LED, HIGH); // O LED acende (estÃ¡ escuro)

  }
}