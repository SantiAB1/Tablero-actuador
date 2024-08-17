#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Pines del módulo NRF24L01
#define CE 9
#define CSN 10
#define LEDVENENO 4
#define LEDR 5
#define LEDG 6
#define LEDB 7
// Configuración del radio
RF24 radio(CE, CSN);

// Dirección de los canales (debe coincidir con la dirección del transmisor)
const byte direccion[6] = "juan";

void interpretarmensaje(char texto);


char mensaje[32];
char confirmacion='S';