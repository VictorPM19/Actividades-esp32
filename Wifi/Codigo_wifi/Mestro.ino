#include <WiFi.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

int led1 = 18;  // Foco 1
int led2 = 22;  // Foco 2

// Ingresar la red a la cual se va a conectar
const char* ssid = "Orco";
const char* password = "20052005";

// Se crea un servidor HTTP en el puerto 80
WiFiServer server(80);

void setup() {
  // Se inicializa el monitor serial
  Serial.begin(115200);
  SerialBT.begin("Orco");
  Serial.println("El Bluetooth ha iniciado y está listo para emparejarse");

  // Declaramos los pines como salida
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Conexión a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a WiFi");
  // Imprime la dirección IP asignada
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciando servidor web
  server.begin();
}

void loop() {
  // Manejo de conexión Bluetooth
  if (SerialBT.available()) {
    char DatoRecibo = SerialBT.read();
    if (DatoRecibo == 'A') {
      digitalWrite(led1, HIGH);
      Serial.println("LED 1 Encendido desde Bluetooth");
    } else if (DatoRecibo == 'B') {
      digitalWrite(led1, LOW);
      Serial.println("LED 1 Apagado desde Bluetooth");
    } else if (DatoRecibo == 'C') {
      digitalWrite(led2, HIGH);
      Serial.println("LED 2 Encendido desde Bluetooth");
    } else if (DatoRecibo == 'D') {
      digitalWrite(led2, LOW);
      Serial.println("LED 2 Apagado desde Bluetooth");
    }
  }

  // Manejo de conexión WiFi
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Nueva solicitud");

    // Lee la solicitud HTTP
    String request = client.readStringUntil('\r');
    Serial.println(request);

    // Control de los focos
    if (request.indexOf("/LED1-ON") != -1) {
      digitalWrite(led1, HIGH);
    }
    if (request.indexOf("/LED1-OFF") != -1) {
      digitalWrite(led1, LOW);
    }
    if (request.indexOf("/LED2-ON") != -1) {
      digitalWrite(led2, HIGH);
    }
    if (request.indexOf("/LED2-OFF") != -1) {
      digitalWrite(led2, LOW);
    }

    client.flush();

    // Enviar respuesta HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<style>.center{text-align:center;}</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1 class='center' style='font-size: 5em;'>Control de Iluminación</h1>");

    // Controles para los focos
    client.println("<p class='center'><FONT SIZE='6' COLOR='#3498DB'>Control para Foco 1</p>");
    client.println("<p class='center'> <a href=\"/LED1-ON\"><button style='width:150px; height: 50px; background-color: green; border: none; color: white; font-size: 20px; cursor: pointer;'>Encender</button></a></p>");
    client.println("<p class='center'> <a href=\"/LED1-OFF\"><button style='width:150px; height: 50px; background-color: red; border: none; color: white; font-size: 20px; cursor: pointer;'>Apagar</button></a></p>");

    client.println("<p class='center'><FONT SIZE='6' COLOR='#3498DB'>Control para Foco 2</p>");
    client.println("<p class='center'> <a href=\"/LED2-ON\"><button style='width:150px; height: 50px; background-color: green; border: none; color: white; font-size: 20px; cursor: pointer;'>Encender</button></a></p>");
    client.println("<p class='center'> <a href=\"/LED2-OFF\"><button style='width:150px; height: 50px; background-color: red; border: none; color: white; font-size: 20px; cursor: pointer;'>Apagar</button></a></p>");

    client.println("</body>");
    client.println("</html>");
  }
}