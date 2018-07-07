#include <SoftwareSerial.h>
#include <HttpClient.h>
#include "WiFiEsp.h"

SoftwareSerial Serial1(2,3); // RX, TX
String content;
float humedad = 333.0;
float temperatura=333.0;
//boolean ventilador= false;
//char ssid[] = "NLRICVAL";            // your network SSID (name)
//char pass[] = "oracionparamami";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
///char server[] = "luibasantes.pythonanywhere.com";

#define ventilador false
#define pass "oracionparamami"
#define ssid "NLRICVAL"
#define server "luibasantes.pythonanywhere.com"
WiFiEspClient client;

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);  
  WiFi.init(&Serial1);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  Serial.println();
  Serial.println("Starting connection to server...");
  }


void loop()
{

 




 
 while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  printWifiStatus();
  Serial.println();
  Serial.println("Starting connection to server...");
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    String value = "pruebaSabado";  // an arbitrary value for testing
    String content = "{\"nombre\": \"" + value + "\",\"temperatura\": " + String(temperatura) +",\"humedad\": " + String(humedad) + ",\"ventilador\": " + String(ventilador) +"}";
    client.println("POST /api/plantas/ HTTP/1.1");
    client.println("Host: luibasantes.pythonanywhere.com");
    client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);
    Serial.println(content);
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  delay(30000);
  client.stop();
  delay(60000);
  delay(60000);
}


