#include "fumfun.h"


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
    digitalWrite(reles[i], HIGH); //verificar la logica porque esto esta en high y deberia apagar
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

//////////Deteccion del sensor de lluvia

//Funcion para habilitar interrupcion por cambio de pin
void initINT()
{
  PCICR|= (1<<PCIE0);
  PCMSK0|=(1<<PCINT3);
}

//Función para cambiar la variable que indica si se activo o desactivo el sensor
void rutinaINT(PCINT0_vect) 
{
  delay(500);  //se coloca un delay para asegurar que los rebotes no cambien el estado varias veces, ya que podrian generar que se desconfigure la logica y quede siempre en activo el sensor
  if(sensor_on==0){
    sensor_on=1;
  }else {
    sensor_on=0;
    }
  
}
