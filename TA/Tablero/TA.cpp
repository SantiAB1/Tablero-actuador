#include "TA.h"

int t_carga = 5; // todos en seg
int t_veneno = 8;
int t_limpieza =17; //en total son 30 segundos el disparo

char confirmacion='S';
char lluvia='L';
const byte direccion[5]="juan";
bool flag=0;
bool sensor_on=0;
char mensaje;


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

void miISR()
{
  delay(500); //se coloca un delay para asegurar que los rebotes no cambien el estado varias veces, ya que podrian generar que se desconfigure la logica y quede siempre en activo el sensor
  if(sensor_on==0){
    sensor_on=1;
  }else {
    sensor_on=0;
    }
}
