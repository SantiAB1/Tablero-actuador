#include "TA.h"


//char mensaje;
bool sensor_latch = false;
bool sensor_activo = true;
bool noveneno=false;


void setup() {

    /* Secuencia de encendido del LED RGB */
  pinMode(LEDENCENDIDO,OUTPUT);          
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
  SecuenciaRGB();

    /* Configuración de Serial y Radio */
  Serial.begin(9600);
//inicio la comunicacion por radio
  radio.begin();
  
  radio.openWritingPipe(direccion2);
  radio.openReadingPipe(1, direccion); //se setea una pipe y una direccion 
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();             //El tablero actuador se pone como Receptor en principio
  
//Declaro los leds a usar y el pin del sensor, el RGB tiene 3 patas que usan 3 pines distintos, el led del veneno es violeta
  pinMode(Sensor,INPUT_PULLUP);
  pinMode(botondisparo,INPUT_PULLUP);
  pinMode(botonsens,INPUT_PULLUP);


//Configuración de los relés
  for(char i = 0; i < 4; i++)
   {
    pinMode(reles[i], OUTPUT);
    digitalWrite(reles[i], HIGH);
  }
  digitalWrite(LEDENCENDIDO,LOW); //Prende el led para saber que el equipo esta encendido
  digitalWrite(LEDR,HIGH);
  digitalWrite(LEDG,HIGH);
  digitalWrite(LEDB,HIGH);

}

void loop() {
  

  if(!digitalRead(botondisparo)){ // Este es un disparo manual (forzado)
    disparo(1);
    Serial.println("Disparo manual activado");
    disparo_forzado = true;
  }
    // Boton para inhabilitar el sensor de lluvia
  if(!digitalRead(botonsens)){
    if(!sensor_latch){
      sensor_latch = true;
      sensor_activo = !sensor_activo;
      Serial.println("Estado de sensor cambiado a:");
      if(sensor_activo){
        Serial.println("Sensor activo");
      }
      else
        Serial.println("Sensor inactivo");

      leds(); // actualizo los leds
      delay(40); // rebotes
    }
  } else if(sensor_latch){
    sensor_latch = false;
    delay(40); // rebotes 
  }



  sensor_on = digitalRead(Sensor); // está en 1 (lluvia) cuando el pin 7 va a tierra

  //Apago todos los leds hasta que llegue un mensaje 
  
  if (radio.available()) //Detecta si hay algo para leer (si el emisor le mando algo)
   {
    radio.read(&mensaje, sizeof(char));
    Serial.println(mensaje);
    interpretarmensaje(mensaje);
    leds(); // actualiza los LEDs
   }
  
    
}
