
#include <ESP8266WiFi.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define kapama            D2
#define acma              D2

#define WLAN_SSID       "Casper VIA_A1_1"             // MODEM ADI
#define WLAN_PASS       "278c5e9fbb0c"        // MODEM ŞİFRESİ 

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"                      //Adafruit Server
#define AIO_SERVERPORT  1883                                  //Standart Port     
#define AIO_USERNAME    "deaath"                             // ADAFRUİT.İO KULLANICI ADI
#define AIO_KEY         ""  // ADAFRUİT.İO ANAHTAR KODUMUZ

//WIFI CLIENT
WiFiClient client;
//IFTT sitesi ile bağlantı kurmamızı sağlayacak bir wifi objesi oluşturuyoruz

//Aracı site ile bağlantı kurmamızı sağlayacak bir obje oluşturuyoruz
//objenin bu bağlantıyı sağlarken kullanacağı değişkenleri beliritiyoruz
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//
Adafruit_MQTT_Subscribe komut1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/kapama"); 
Adafruit_MQTT_Subscribe komut2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/acma");


void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(kapama, OUTPUT);
  pinMode(acma, OUTPUT);
 // pinMode(Relay3, OUTPUT);
 // pinMode(Relay4, OUTPUT);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);


//Belirlediğimiz değişkenler sayesinde belirttiğimiz ağa bağlanıyoruz
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Light1);
 // mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
 // mqtt.subscribe(&Light4);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(200))) {
    Serial.print("kapı");
    if (subscription == &Light1) {
      Serial.print(F("acıldı: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(kapama, HIGH);
      
    }
    if (subscription == &Light2) {
      Serial.print(F("kapandı : "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(acma, LOW);
    }
 /*   if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(Relay3, Light3_State);
    }
    if (subscription == &Light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      digitalWrite(Relay4, Light4_State);
      
    } */
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
