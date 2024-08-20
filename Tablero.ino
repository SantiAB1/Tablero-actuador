#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Tablero.h"
#include "fumfun.h"


void setup() {
  Serial.begin(9600);
//inicio la comunicacion por radio
  radio.begin();
  radio.openReadingPipe(1, direccion); //se setea una pipe y una direccion 
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();             //El tablero actuador se pone como Receptor en principio
  
//Declaro los leds a usar, el RGB tiene 3 patas que usan 3 pines distintos, el led del veneno es violeta
  pinMode(LEDVENENO,OUTPUT);          
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);

//Configuración de los relés
  for(char i = 0; i < 4; i++)
   {
    pinMode(reles[i], OUTPUT);
    digitalWrite(reles[i], HIGH);
  }
//Configuracion para sensor de lluvia
  pinMode(Sensor,INPUT_PULLUP);
  initINT(); //interrupción
  
}

void loop() {

  if (radio.available()) //Detecta si hay algo para leer (si el emisor le mando algo)
  {
    radio.read(&mensaje, sizeof(mensaje));
    //Serial.println(mensaje);
    interpretarmensaje(mensaje); 
    
    }
}

void interpretarmensaje(char texto)
{

  switch (texto)
  {
    case 'S':
      digitalWrite(LEDR,LOW);                               
      digitalWrite(LEDG,HIGH);                           //Si se recibe una S se prende el LED EN VERDE y se apaga el rojo por si se habia prendido
      radio.openWritingPipe(direccion);
      radio.stopListening();                             //se pone en modo emisor

      if(sensor_on==1){
        radio.write(&lluvia, sizeof(lluvia));            //envia el mensaje "L" para confirmar correcta recepción pero avisar que el sensor de lluvia está activo
      }else {
        radio.write(&confirmacion, sizeof(confirmacion));  // envia el mensaje "S" para confirmar que se está recibiendo correctamente
      }

      radio.openReadingPipe(1,direccion); 
      radio.startListening();                            //vuelve al modo receptor
      //delay(10000);


    break;

    case 'D':
      flag=1;           //flag: 1 dispara, y 0 SOP
      disparo(flag);
    break;  
    
    case 'P':
      flag=0;
      disparo(flag);
    break;
    
    case 'V':
      digitalWrite(LEDVENENO,HIGH);
       // Cuando la pagina web nos dice que esta vacio
    break;

    default:
      digitalWrite(LEDG,LOW);
      digitalWrite (LEDR,HIGH); //Pongo el rgb en rojo para indicar que no se está recibiendo "S" ni ningún otro mensaje válido
    break;
  }




}
