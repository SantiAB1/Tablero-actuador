#include "TA.h"

int t_carga = 5; // todos en seg
int t_veneno = 8;
int t_limpieza =17; //en total son 30 segundos el disparo

char confirmacion='S', lluvia='L', mensajeF = 'F';
const byte direccion[5]="juan";
const byte direccion2[5]="pepe";
bool flag=0;
bool sensor_on=0;
char mensaje;
bool disparo_forzado = false;


RF24 radio(CE, CSN);

void Rele(int n, bool estado){ 
//INICIO LA FUNCION RELE, NECESITO EL RELE QUE QUIERO PCERRAR (n) Y EL ESTADO es 1 o 0 
  if(n > 4 || n < 1) return;            //SI HAY ERROR VUELVE
  digitalWrite(reles[n - 1], !estado);  //SI NO HAY ERROR
  Serial.print("Relé ");
  Serial.print(n+0);
  Serial.println(estado?" encendido":" apagado");
}
void STOP(){
  for(char i = 0; i < 4; i++){
    digitalWrite(reles[i], HIGH);
  }
  Serial.println("STOP");
}

void disparo(bool flag){
  // Cuando se dispara el dispositivo se prende una luz amarilla en el led RGB
  digitalWrite(LEDR,LOW);
  digitalWrite(LEDG,HIGH);
  digitalWrite(LEDB,LOW);

  if(flag==1){
    Rele(1,HIGH); //Bomba de agua
    Rele(2,HIGH); //electrovalvula
    delay(t_carga*1000);
    Rele(3,HIGH); //Inyecto Veneno
    delay(t_veneno*1000);
    Rele(3,LOW);//Corto veneno
    delay(t_limpieza*1000);
    Rele(1,LOW); //Apago Bomba de agua
    Rele(2,LOW); //Cierro electrovalvula
    delay(3000);
  }
  else{
    STOP();
  }
}

void interpretarmensaje(char texto)
{

  switch (texto)
  {
    case 'S':
      noveneno=false;

      radio.stopListening();                             //se pone en modo 

      if(disparo_forzado){
        Serial.println("Mensaje de disparo forzado");
        radio.write(&mensajeF, sizeof(char));
        disparo_forzado = false;

      }else if(sensor_on==1 && sensor_activo){
        radio.write(&lluvia, sizeof(char));            //envia el mensaje "L" para confirmar correcta recepción pero avisar que el sensor de lluvia está activo
      }else {
        radio.write(&confirmacion, sizeof(char));  // envia el mensaje "S" para confirmar que se está recibiendo correctamente
      
      }
    //.openReadingPipe(1,direccion); 
      radio.startListening();                           //vuelve al modo receptor
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
      noveneno=true;
       // Cuando la pagina web nos dice que esta vacio
    break;

    default:
      
    break;
  }

}

void leds(){
  if(noveneno){
    // no hay veneno luz roja
    digitalWrite(LEDB,HIGH);
    digitalWrite(LEDG,HIGH);
    digitalWrite(LEDR,LOW);

  } else if(!sensor_activo){
    // sensor desactivado (bypass) luz azul
    digitalWrite(LEDG,HIGH);
    digitalWrite(LEDB,LOW);
    digitalWrite(LEDR,HIGH);

  } else if(sensor_on){
    // lluvia luz blanca
    digitalWrite(LEDR,LOW);
    digitalWrite(LEDG,LOW);
    digitalWrite(LEDB,LOW);

  } else {
    // listoooo
    digitalWrite(LEDG,LOW);
    digitalWrite(LEDR,HIGH);
    digitalWrite(LEDB,HIGH);


  }

}


void SecuenciaRGB(){
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  delay(300);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);

  delay(300);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);

  delay(300);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);

  delay(800);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

}

