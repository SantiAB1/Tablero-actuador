#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

                                                    //RECEPTOR 
// Pines del módulo NRF24L01
#define CE 9
#define CSN 10

// Configuración del radio
RF24 radio(CE, CSN);

// Dirección de los canales (debe coincidir con la dirección del transmisor)
const byte direccion[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, direccion);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  if (radio.available()) { //radio.available solo en receptor
    char texto[32] = "";
    radio.read(&texto, sizeof(texto));
    Serial.println("Mensaje recibido: ");
    Serial.println(texto);
    delay(500);
  }
}