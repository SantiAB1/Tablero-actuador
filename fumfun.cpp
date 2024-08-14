#include "fumfun.h"

void Rele(int n, bool estado){ 
//INICIO LA FUNCION RELE, NECESITO EL RELE QUE QUIERO PCERRAR (n) Y EL ESTADO es 1 o 0 
const char reles[] = {2, 3, 4, 5};      //Se elijieron estos pines se pueden cambiar 
  if(n > 4 || n < 1) return;            //SI HAY ERROR VUELVE
  digitalWrite(reles[n - 1], !estado);  //SI NO HAY ERROR
  Serial.print("Relé ");
  Serial.print(n+0);
  Serial.println(estado?" encendido":" apagado");
}