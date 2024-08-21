#include "TA.h"

//char mensaje;


void setup() {
  Serial.begin(9600);
//inicio la comunicacion por radio
  radio.begin();
  radio.openReadingPipe(1, direccion); //se setea una pipe y una direccion 
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();             //El tablero actuador se pone como Receptor en principio
  
//Declaro los leds a usar y el pin del sensor, el RGB tiene 3 patas que usan 3 pines distintos, el led del veneno es violeta
  pinMode(LEDVENENO,OUTPUT);          
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
  pinMode(Sensor,INPUT_PULLUP);

//Configuración de los relés
  for(char i = 0; i < 4; i++)
   {
    pinMode(reles[i], OUTPUT);
    digitalWrite(reles[i], HIGH);
  }
  
  attachInterrupt(digitalPinToInterrupt(11), miISR, CHANGE);

}

void loop() {
  if (radio.available()) //Detecta si hay algo para leer (si el emisor le mando algo)
   {
    radio.read(&mensaje, sizeof(mensaje));
    Serial.println(mensaje);
    interpretarmensaje(mensaje); 
    
    
   }

}
