/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include "DHT.h"
// nastavení čísla pinu s připojeným DHT senzorem
#define pinDHT 5

// odkomentování správného typu čidla
#define typDHT11 DHT11     // DHT 11
//#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicializace DHT senzoru s nastaveným pinem a typem senzoru
DHT mojeDHT(pinDHT, typDHT11);

// Insert your network credentials
#define WIFI_SSID "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "REPLACE_WITH_YOUR_PASSWORD" //doplnit na funkcni siti

// Insert Firebase project API Key
#define API_KEY "https://projekt-435aa-default-rtdb.europe-west1.firebasedatabase.app/"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "AIzaSyDwj0XxkZ2q5sPLcKRYJB37B1TaYuMNYQI" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  mojeDHT.begin();
}

void loop()
{
  short tep = round(mojeDHT.readTemperature());
  short vlh = round(mojeDHT.readHumidity());
  if (isnan(tep) || isnan(vlh)) 
  {
    Serial.println("Chyba při čtení z DHT senzoru!");
  } 
  else 
  {
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" stupnu Celsia, ");
    Serial.print("vlhkost: "); 
    Serial.print(vlh);
    Serial.println("  %");
  }

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setInt(&fbdo, "Hodnoty/teplota", tep))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "Hodnoty/vlhkost", vlh))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  }
}