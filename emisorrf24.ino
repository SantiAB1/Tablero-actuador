#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
                                      //TRANSMISOR
// Pines del módulo NRF24L01
#define CE 9
#define CSN 10

// Configuración del radio
RF24 radio(CE, CSN);

// Dirección de los canales
const byte direccion[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(direccion);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  //radio.startWrite(const void *buf, uint8_t len, const bool multicast)
}

void loop() {
  const char texto[] = "Hola Mundo";
  radio.write(&texto, sizeof(texto));
  Serial.println("Mensaje enviado: Hola Mundo");
  delay(1000);  // Envía el mensaje cada segundo

}
