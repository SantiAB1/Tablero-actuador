#ifndef FUMFUN_H
#define FUMFUN_H
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const char reles[] = {2, 3, 4, 5};      //Se elijieron estos pines se pueden cambiar 
const int t_carga = 5; // todos en seg
const int t_veneno = 8;
const int t_limpieza =17; //en total son 30 segundos el disparo
void Rele(int n, bool estado); 
// RELE, cierra y abre los reles, se le da "n" el numero del rele y el estado 1 es cerrado

void STOP();
// Abre todos los reles

void disparo(bool flag);
//Hace un disparo utilizando las funciones anteriores

#endif