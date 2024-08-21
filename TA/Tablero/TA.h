#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#define CE 9
#define CSN 10
//Pines de LEDS
#define LEDVENENO A0
#define LEDR A1
#define LEDG A2
#define LEDB A3
//Pin sensor de lluvia
#define Sensor 7
static char reles[] = {2, 3, 4, 5};

extern RF24 radio;
extern char mensaje;
extern int t_carga; // todos en seg
extern int t_veneno;
extern int t_limpieza; //en total son 30 segundos el disparo
extern const byte direccion[5];  // Dirección de los canales (debe coincidir con la dirección del transmisor)
extern bool flag;
extern bool sensor_on;

extern char confirmacion; //mensaje default a enviar cuando el sensor de lluvia NO esta activo
extern char lluvia;       //mensaje a enviar cuando el sensor de lluvia esta activo

void interpretarmensaje(char texto);
void miISR();
void Rele(int n, bool estado);
void STOP();
void disparo(bool flag);

