 //İnternet ve aracı site için kullanacağımız kütüphaneler
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define kapama            D2
#define acma              D2

//Bağlanacağımız ağ adı ve şifresi
#define WLAN_SSID       "***"             // MODEM ADI
#define WLAN_PASS       "***"        // MODEM ŞİFRESİ 

/************************* Adafruit.io Setup *********************************/
//MQTT temelli aracı site,sunucu, bilgileri 
#define AIO_SERVER      "io.adafruit.com"                      //Bağlanacağımız Site Adı
#define AIO_SERVERPORT  1883                                  //Standart Port (Bağlantı Portu)     
#define AIO_USERNAME    "***"                             // ADAFRUİT.İO KULLANICI ADI
#define AIO_KEY         "***"  // ADAFRUİT.İO ANAHTAR KODUMUZ

//Ağ bağlantısı kurarken kullanacağımız bir wifi nesnesi oluşturuyoruz
WiFiClient client;
 
//Aracı site ile bağlantı kurmamızı sağlayacak bir nesne oluşturuyoruz
//nesnenin bu bağlantıyı sağlarken kullanacağı değişkenleri beliritiyoruz
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//Aracı sitede abone olacağımız,kullanmak istediğimiz bilgilerin bulunduğu, konu başlıklarını atayacağımız değişkenleri oluşturuyoruz
Adafruit_MQTT_Subscribe komut1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/kapama"); 
Adafruit_MQTT_Subscribe komut2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/acma");

//MQTT bağlantısı için kullandığımız fonksiyon
void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(kapama, OUTPUT);
  pinMode(acma, OUTPUT);
 
  
  // WİFİ ye bağlanmaya çalışıyoruz
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

  //subscribe olduğumuz topic deki değerin ilk durumu (?)
  mqtt.subscribe(&komut1);
  mqtt.subscribe(&komut2);
 
}

void loop() {
  //MQTT fonsiyonundan gelen değeri kullanıyoruz
  MQTT_connect();
  
  //Abone olduğumuz topiclere bir işaretçi tanımlaması
  Adafruit_MQTT_Subscribe *subscription;

  //abonelik paketlerinin gelmesini bekliyoruz
  //veri var mı
  while ((subscription = mqtt.readSubscription(200))) {
    Serial.print("kapı");
    if (subscription == &komut1) {
      Serial.print(F("acıldı: "));
      //son durumu char bir değişken olarak tanımladığımız komut1.lastread e kaydet
      Serial.println((char *)komut1.lastread);
      //int komut1_State = atoi((char *)komut1.lastread); (?)
      digitalWrite(kapama, HIGH);
      
    }
    if (subscription == &komut2) {
      Serial.print(F("kapandı : "));
      Serial.println((char *)komut2.lastread);
      //int komut2_State = atoi((char *)komut2.lastread); (?)
      digitalWrite(acma, LOW);
    }
   }
}

//MQTT sunucu bağlantısı
//MQTT ye bağlı olup olmadığımızın ayarlanması
void MQTT_connect() {
  //8 bitlik değişken (-128,+127)
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  //8 bitlik pozitif değişken (0,255)
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
