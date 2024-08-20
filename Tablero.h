#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "fumfun.h"

// Pines del módulo NRF24L01
#define CE 9
#define CSN 10
//Pines de LEDS
#define LEDVENENO A0
#define LEDR A1
#define LEDG A2
#define LEDB A3
//Pin sensor de lluvia
#define Sensor 11

// Configuración del radio
RF24 radio(CE, CSN);


//Declaraciones para funciones del módulo de radio
const byte direccion[5] = "juan";  // Dirección de los canales (debe coincidir con la dirección del transmisor)
bool flag=0;
void interpretarmensaje(char texto);
unsigned char mensaje;
unsigned char confirmacion='S'; //mensaje default a enviar cuando el sensor de lluvia NO esta activo
unsigned char lluvia='L';       //mensaje a enviar cuando el sensor de lluvia esta activo


//Sensor de lluvia
//DECLARO FUNCION PARA INTERRUPCION
void initINT();
//DECLARO RUTINA DE SERVICIO
void rutinaINT(PCINT0_vect);

bool sensor_on=0;//Variable que sirve como flag para saber si el sensor está encendido