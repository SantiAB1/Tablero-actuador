#include <SPI.h>
#include <nRF24L01.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BearSSLHelpers.h>
#include <ArduinoJson.h>
//#include <RTClib.h>
#include <RF24.h>
#include <String.h>
#include <AceCommon.h>
#include <Wire.h>
//TRANSMISOR
// Pines del módulo NRF24L01
#define CE 2
#define CSN 15

// Configuración del radio
RF24 radio(CE, CSN);
char mensaje;
// Dirección de los canales
const byte direccion[5] = "juan";
const byte direccion2[5] = "pepe";
const char* ssid = "Marina y Nacho";                                                                     // Tu red WiFi
const char* password = "leticia15";                                                                              // Tu contraseña WiFi
const char* geturl1 = "https://cessna-api.onrender.com/api/fumigations/user/b1ea78ba-5394-4110-b6c5-f9cd68b85fb5/today";
const char* geturl2 = "https://cessna-api.onrender.com/api/fumigationEquipments/user/b1ea78ba-5394-4110-b6c5-f9cd68b85fb5/getConfig";  // URL para el get de horarios
const char* posturl = "https://cessna-api.onrender.com/api/fumigationEquipments/user/b1ea78ba-5394-4110-b6c5-f9cd68b85fb5/changeConfig";
BearSSL::WiFiClientSecure client;                                                                                 // Crear un nuevo cliente para transmitir datos
HTTPClient http;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //delay(5000);
  Serial.println("");
  Serial.print("Conectado al WiFi, IP: ");
  Serial.println(WiFi.localIP());
  radio.begin();
  radio.openWritingPipe(direccion);
  radio.openReadingPipe(1, direccion2);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}


char texto = 'S', disparo = 'D',noveneno='V';
String payload;
const char* horario1;
const char* horario2;
const char* horario3;
const char* horarioact;
const char* ultimoDisparo;  // horario del último disparo
int disparos_restantes;      //Contador de la cantidad de disparos
bool estado_motor;           //Estado del BOTON de parar del server
StaticJsonDocument<256> doc;  // Ajusta el tamaño según sea necesario
bool llueve = false;



void loop() {

//  while (Serial.available() == 0);
//  texto = Serial.read();
//  if (texto > 60 && texto < 122) {
    radio.stopListening();  // dejo de escuchar para poder transmitir

    radio.write(&texto, sizeof(texto));

    Serial.println("Enviado:");
    Serial.println(texto);

    radio.startListening();  // vuelvo a escuchar
//  }
  //Si esta recibiendo lee el mensaje y se comunica con la pagina
  if (radio.available()) {
    radio.read(&mensaje, sizeof(mensaje));
    //if(mensaje!=NULL)
    Serial.println("Recibido:");
    Serial.println(mensaje);

    ManejoPagina();

    if((horarioact == horario1 || horarioact == horario2 || horarioact == horario3)
    && horarioact != ultimoDisparo && disparos_restantes != 0 && estado_motor == 0) {
      ultimoDisparo = horarioact;
      ManejoDisparo();
    }else if (disparos_restantes==0){
      radio.stopListening();
      radio.write(&noveneno, sizeof(noveneno));
    }

  }  
  delay(20000);  // Envía el mensaje cada segundo

}





///////////////////////////////FUNCIONES///////////////////

void ManejoPagina(){
    /*  Comunicación con la página  */

  if (mensaje == 'S' || mensaje == 'F') {
    if(mensaje == 'S') llueve = false;

        /*  Primer GET  */

    client.setInsecure();  // Deshabilitar la validación del certificado SSL
    http.begin(client, geturl1);  // Especificar la URL del GET
    int httpCode = http.GET();   // Realizar la solicitud GET

    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Datos recibidos");
      Serial.println(payload);

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
        return;
      }

      horario1 = doc["horario1"];
      horario2 = doc["horario2"];
      horario3 = doc["horario3"];
      horarioact = doc["horaActual"];

    } else {
      Serial.print("Error en la solicitud GET, código: ");
      Serial.println(httpCode);
    }

    http.end();  // Cerrar la conexión GET

    Serial.println("Datos Parseados");
    Serial.println(horario1);
    Serial.println(horario2);
    Serial.println(horario3);
    Serial.println(horarioact);



      /*  Segundo GET  */

    client.setInsecure();  // Deshabilitar la validación del certificado SSL
    http.begin(client, geturl2);  // Especificar la URL del GET
    httpCode = http.GET();   // Realizar la solicitud GET

    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Datos recibidos");
      Serial.println(payload);

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
        return;
      }

      disparos_restantes = doc["disparos_restantes"];
      estado_motor = doc["estado_motor"];

    } else {
      Serial.print("Error en la solicitud GET, código: ");
      Serial.println(httpCode);
    }

    http.end();  // Cerrar la conexión GET

    Serial.println("Datos Parseados 2");
    Serial.println(disparos_restantes);
    Serial.println(estado_motor);


    /*  Post de NO-LLUVIA  */

    client.setInsecure();
    http.begin(client, posturl);
        http.addHeader("Content-Type", "application/json");
        doc["disparosRestantes"] = (mensaje == 'S')?0:1; // si el mensaje fue F restame un disparo del server
        doc["estadoSensor"] = llueve;
        serializeJson(doc, payload);
        Serial.println("JSON para enviar:");
        Serial.println(payload);
        int httpResponseCode = http.POST(payload);
        if (httpResponseCode > 0) {
            String response = http.getString();  // Obtiene la respuesta del POST
            Serial.println(httpResponseCode);    // Código de respuesta
            Serial.println(response);            // Respuesta del servidor
        } else {
            Serial.print("Error en la solicitud POST: ");
            Serial.println(httpResponseCode);
        }

      http.end();  // Finaliza la conexión después del POST


  } else if(mensaje == 'L'){
    Serial.printf("Llueve loco");
    llueve = true;

    /*  Post de LLUVIA  */

    client.setInsecure();
    http.begin(client, posturl);
        http.addHeader("Content-Type", "application/json");
        doc["disparosRestantes"] = 0;
        doc["estadoSensor"] = true;
        serializeJson(doc, payload);
        Serial.println("JSON para enviar:");
        Serial.println(payload);
        int httpResponseCode = http.POST(payload);
        if (httpResponseCode > 0) {
            String response = http.getString();  // Obtiene la respuesta del POST
            Serial.println(httpResponseCode);    // Código de respuesta
            Serial.println(response);            // Respuesta del servidor
        } else {
            Serial.print("Error en la solicitud POST: ");
            Serial.println(httpResponseCode);
        }

      http.end();  // Finaliza la conexión después del POST
  }
}





void ManejoDisparo(){
  if(llueve) return;

  radio.stopListening();  // dejo de escuchar para poder transmitir

  radio.write(&disparo, sizeof(texto));

  Serial.println("Enviado:");
  Serial.println(disparo);

  radio.startListening();  // vuelvo a escuchar


      /*  Post de DISPARO  */
  
  client.setInsecure();
  http.begin(client, posturl);
      http.addHeader("Content-Type", "application/json");
      doc["disparosRestantes"] = 1;
      doc["estadoSensor"] = false;
      serializeJson(doc, payload);
      Serial.println("JSON para enviar:");
      Serial.println(payload);
      int httpResponseCode = http.POST(payload);
      if (httpResponseCode > 0) {
          String response = http.getString();  // Obtiene la respuesta del POST
          Serial.println(httpResponseCode);    // Código de respuesta
          Serial.println(response);            // Respuesta del servidor
      } else {
          Serial.print("Error en la solicitud POST: ");
          Serial.println(httpResponseCode);
      }

    http.end();  // Finaliza la conexión después del POST

}
