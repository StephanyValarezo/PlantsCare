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


//VARIABLES
float h,t;
String aviso;


void setup() {
   Serial.begin(9600); //Se inicia la comunicación serial
   
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
  
  //borrar
  
  //fin de borrar

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
  delay(60000); 
  delay(60000);

  
}
