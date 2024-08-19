#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "fumfun.h"

// Pines del módulo NRF24L01
#define CE 9
#define CSN 10

#define LEDVENENO A0
#define LEDR A1
#define LEDG A2
#define LEDB A3
// Configuración del radio
RF24 radio(CE, CSN);

// Dirección de los canales (debe coincidir con la dirección del transmisor)
const byte direccion[6] = "juan";  //por que tamaño 6 y no 5?
bool flag=0;
void interpretarmensaje(char texto);


char mensaje;
char confirmacion='S';