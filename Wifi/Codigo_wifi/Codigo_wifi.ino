#include <WiFi.h>

#include <HTTPClient.h>

const char *ssid = "";
const char *password = "";


void WiFi_Setup() 
{
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int contador = 0;
  while (WiFi.status() != WL_CONNECTED && contador < 20 ) {
    delay(500);
    Serial.print(".");
    contador += 1;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi conectado");
  }else {
    ESP.restart();
  }
 
}
void setup() 
{
  Serial.begin(115200);
  WiFi_Setup();
}
void loop() 
{
  
    HTTPClient http;
    String RFID = "valor del sensor";
    String datos_a_enviar = "rfid=" + RFID;
    Serial.println(datos_a_enviar);
    http.begin("https://ip/xxxx.php");       
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int codigo_respuesta = http.POST(datos_a_enviar);   
    if (codigo_respuesta > 0) 
    {
      if (codigo_respuesta == 200) 
      {
        String cuerpo_respuesta = http.getString();
        Serial.println(cuerpo_respuesta);
      }
    } 
    else 
    {
      Serial.println(codigo_respuesta);
    }
    http.end();  
  
}