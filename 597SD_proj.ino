

#include <Arduino.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "REPLACE_WITH_YOUR_PASSWORD"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCUD3LVhn08BMucnlSSTRQgqnl0lzMMcQ0"

#define USER_EMAIL "anse.saintlouis@gmail.com"
#define USER_PASSWORD "12332112"


// Insert RTDB URLefine the RTDB URL 

#define DATABASE_URL "https://sdfinalproject-97340-default-rtdb.firebaseio.com" 

#define DATABASE_SECRET "qPSOpunjBSv4mztBOSRMSXr7EaUAGunSJTMQXhoA"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool flagValue;
int ledPin = 5;  //on board LED
float floatValue, T;
float f[3] = {0.5, 40, 80};
float frequency;
bool signupOK;

void setup() {
  srand(time(NULL)); //initialize random seed
  int RandIndex = rand() % 4;
  frequency = f[RandIndex];
  Serial.begin(115200);
  WiFi.begin("孙老师", "12332112");
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the api key (required) 
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  // Assign the RTDB URL (required) 
  config.database_url = DATABASE_URL;


  //Sign up 
  
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long running token generation task 
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getBool(&fbdo, "/freqlog/flag")) {
      if (fbdo.dataType() == "bool") {
        flagValue = fbdo.boolData();
        if(flagValue){
            int RandIndex = rand() % 4; //generate a random number between 0 and 3
            frequency = f[RandIndex]; //select new frequency
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setFloat(&fbdo, "/freqlog/freq", frequency)) {
    }

    else {
      Serial.println(fbdo.errorReason());
    }

  }
  blink();
  delay(1000);
  
}

void blink(){ // function declaration
  T = 1/frequency * 1000; //get period in ms
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(T/2);                      // wait for on period to finish
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
  delay(T/2);                      // wait for off period to finish
} 