//SENSOR TEMPERATURA
#include "DHT.h" //cargamos la librería DHT
#define DHTPIN 2 //Seleccionamos el pin en el que se //conectará el sensor
#define DHTTYPE DHT11 //Se selecciona el DHT11 (hay //otros DHT)
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

//LIBRERIA LCD
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//LIBRERIA DEL CLOCK
#include <Wire.h>
#include "DS3231.h"
DS3231 Clock;
RTClib RTC;

//TARJETA MICRO SD
#include <SD.h> //Load SD library
int chipSelect = 4; //chip select pin for the MicroSD Card Adapter
File Datos; // Datos object that is used to read and write data


//COMUNICACION SERIAL
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2,3); // RX, TX

//wifi

#include <HttpClient.h>
#include "WiFiEsp.h"
//Variables wifi
String content;
float humedad = 333.0;
float temperatura=333.0;
boolean ventilador= false;
char ssid[] = "NETLIFE-Basantes";            // your network SSID (name)
char pass[] = "65432101";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "luibasantes.pythonanywhere.com";
WiFiEspClient client; // Initialize the Ethernet client object


//VARIABLES
float h,t;
String aviso;


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



void digitalDisplay(float h, float t)
{
  lcd.home ();
  lcd.print("Temp:");
  lcd.setCursor ( 7, 0);
  lcd.print(t);
  
  lcd.setCursor ( 0, 1 ); // go to the 2nd line
  lcd.print("Humd:");
  lcd.setCursor ( 7, 1);
  lcd.print(aviso);
}







void setup() {
   
  Serial.begin(9600); //Se inicia la comunicación serial
  
  //---------------------------------------------------------------------------------------
   //wifi
  Serial1.begin(9600);     // initialize serial for ESP module
  WiFi.init(&Serial1);    // initialize ESP module
  
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
  
  //------------------------------------------------------------------------------------------------------------------------


   
  //SENSOR TEMPERATURA 
  dht.begin(); //Se inicia el sensor

  //lcd
  lcd.begin(16, 2);
  lcd.setCursor ( 0, 1);

  //CLOCK
  Wire.begin();
  Clock.setClockMode(false); 

  //TARJETA MICRO SD
  pinMode(chipSelect, OUTPUT); // chip select pin must be set to OUTPUT mode
}


void loop() {
     //SENSOR TEMPERATURA
    h = dht.readHumidity(); //Se lee la humedad
    t = dht.readTemperature(); //Se lee la temperatura
    
    //CLOCK
    DateTime now = RTC.now();
    
  Datos= SD.open("datalog2.txt", FILE_WRITE); // open "Datos.txt" to write data
  if (Datos) {
      
    h = dht.readHumidity(); //Se lee la humedad
    t = dht.readTemperature(); //Se lee la temperatura
    
    //CLOCK
    DateTime now = RTC.now();
    //GUARADAR EN TARJETA
    Datos.print("temp:");
    Datos.print(t);
    Datos.print(",");
    Datos.print("hum:");
    Datos.print(h);
    Datos.print(",");

    Datos.print(" Fecha:");
    Datos.print(now.day());  // Dia del mes
    Datos.print("/"); 
    Datos.print(now.month());
    Datos.print("/"); 
    Datos.print(now.year());
    Datos.print(",");  // Hora en formato 0-23.
   
   Datos.print(" Hora:");
   Datos.print(now.hour());
   Datos.print(":");   // Minutos.
   Datos.print(now.minute());
   Datos.print(":");  // Segundos.
   Datos.print(now.second());
   Datos.println(",");

    
   Datos.close(); // close Datos
   aviso="si yala";
     
  } else {
    Serial.println("Could not open Datos (writing).");
    aviso="no salio";
  }
  
  //pantalla LCD
  digitalDisplay(h,t);


  //enviar a servidor
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
    
    // do nothing forevermore
    while (true);
  }
  
  delay(60000); 
  delay(60000);
  delay(60000); 
  delay(60000);
}




