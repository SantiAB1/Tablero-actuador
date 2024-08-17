#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Tablero.h"



void setup() {
  Serial.begin(9600);
//inicio la comunicacion por radio
  radio.begin();
  radio.openReadingPipe(1, direccion);//se setea una pipe y una direccion 
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();             //El tablero actuador se pone como emisor en principio
  
  //Declaro los leds a usar, el RGB tiene 3 patas que usan 3 pines distintos, el led del veneno es violeta
  pinMode(LEDVENENO,OUTPUT);          
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
  
}

void loop() {

  if (radio.available()) //Detecta si hay algo para leer (si el emisor le mando algo)
  { 
    radio.read(&mensaje, sizeof(mensaje);
    interpretarmensaje(mensaje);
  }
}

void interpretarmensaje(char texto)
{

  switch (texto)
  {
    case 'S':
      analog.Write(LEDR,0);
      analog.Write(LEDG,255);                            //Si se recibe una S se prende el LED EN VERDE y se apaga el rojo por si se habia prendido
      radio.openWritingPipe(direccion);
      radio.stopListening;                               //se pone en modo emisor
      radio.write(&confirmacion, sizeof(confirmacion));  // envia el mensaje "S" para confirmar que se está recibiendo correctamente
      radio.openReadingPipe(1,direccion); 
      radio.startListening;                              //vuelve al modo receptor
    break;

    case 'D':
      bool flag=0
      //hacer un archivo aparte con la funcion de los reles 
    break;
  
    case 'P':
      bool flag=1;
      // en la misma funcion del rele, si recibe un 1 o un 0 hace dos operaciones distintas
    break;
    
    case 'V':
      digital.Write(LEDVENENO,HIGH);
    break;

    default:
      analog.Write(LEDG,0);
      analog.Write (LEDR,255); //Pongo el rgb en rojo para indicar que no se está recibiendo "S" ni ningún otro mensaje válido
    break;
  }




}