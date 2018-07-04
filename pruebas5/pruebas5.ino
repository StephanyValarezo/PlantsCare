#include <SoftwareSerial.h>
#include <HttpClient.h>
#include "WiFiEsp.h"


SoftwareSerial Serial1(2,3); // RX, TX
/*
 WiFiEsp example: WebClient
 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.
 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/



String content;
float humedad = 333.0;
float temperatura=333.0;
boolean ventilador= false;
char ssid[] = "NLRICVAL";            // your network SSID (name)
char pass[] = "oracionparamami";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "luibasantes.pythonanywhere.com";
// Initialize the Ethernet client object
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
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    //GET AQUI NO BORRAR CHCH
    /*
    client.println("GET /api/plantas/ HTTP/1.1");
    client.println("Host: luibasantes.pythonanywhere.com");
    client.println("User-Agent: Mozilla/5.0");
    client.println("Connection: close");
    client.println();
*/
  // Make the HTTP request
  String value = "Arduino9";  // an arbitrary value for testing
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
  
}



void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  //if (!client.connected()) {
   // Serial.println();
    //Serial.println("Disconnecting from server...");
    //client.stop();
    
    // do nothing forevermore
    while (true);
  
  delay(60000);
  delay(60000);

  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    //GET AQUI NO BORRAR CHCH
    /*
    client.println("GET /api/plantas/ HTTP/1.1");
    client.println("Host: luibasantes.pythonanywhere.com");
    client.println("User-Agent: Mozilla/5.0");
    client.println("Connection: close");
    client.println();
*/
  // Make the HTTP request
  String value = "Arduino10";  // an arbitrary value for testing
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

}


