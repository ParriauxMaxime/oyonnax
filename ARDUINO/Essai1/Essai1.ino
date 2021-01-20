#include <dht_nonblocking.h>        //Capteur Temp
#include <LiquidCrystal.h>          //LCD
#include <Ethernet.h>
#include <SPI.h>
#define HTTP_PORT 80
#define DHT11_PIN 42                //PIN Capteur Temp
#define DHT11_TYPE 0                //TYPE Capteur Temp
#define ANALOG_THERMISTOR_PIN 0    // PIN thermistor

DHT_nonblocking DHT(DHT11_PIN, DHT11_TYPE);
LiquidCrystal lcd(27, 26, 25, 24, 23, 22);

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xF6, 0xFF };
IPAddress server(18, 205, 246, 58);
IPAddress myDns(8, 8, 8, 8);
IPAddress myIp(192, 168, 1, 10);
IPAddress myGateway(192, 168, 1, 1);

EthernetClient client;

void setup() {
  pinMode(52, OUTPUT);                //LED
  pinMode(53, INPUT);                 //Bouton
  digitalWrite(52, HIGH);             //Init LED
  delay(1000);
  digitalWrite(52, LOW);             //Init LED
  delay(1000);
  lcd.begin(16, 2);                   //Colonne / ligne
  lcd.print("Smoke Weed !");          //Message !
  
  Serial.begin(9600);
  Ethernet.init();
  //Ethernet.begin(mac, myIp, myDns, myGateway);
/*
  Ethernet.setDnsServerIP(myDns);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.print("Gateway: ");
  Serial.println(Ethernet.gatewayIP());

  delay(1000);
  Serial.println("connecting...");
  
  if (!client.connect(server, HTTP_PORT))
  { 
    Serial.println("Cannot connect to Server");
  }
  else {
    Serial.println("Connected");                                     
  }*/
}

void postData(void *data) {
  char body[255]; 
  sprintf(body, "{\"temperature\": %4s}", (char *)data);
  Serial.println("Sending to Server: ");
  Serial.println(body);                   
  client.print("POST /data HTTP/1.1");           
  Serial.println("POST /data");           
  client.println("Host: 18.205.246.58");
  client.println("Content-Type: application/json");
  client.println("User-Agent: Arduino/1.0");
  client.print("Content-Length: ");
  client.println(24);
  client.println();
  client.print(body);
  client.println();      
}

static bool async_exec(unsigned long int time, void (*fct)(void *), void *data){
  static unsigned long memo = millis( );   //Init MEMO
  
  if (millis( ) - memo > time) {         //Si temps écoulé >10s
    memo = millis();
    (*fct)(data);
    return true;
  }

  return false;
}

void dht_reading(void *temperature) {
  float humidity;
  DHT.measure((float *)temperature, &humidity);
  Serial.println(*(float *)temperature);
}

void temperature_reading(void *temperature) {
  int tempReading = analogRead(ANALOG_THERMISTOR_PIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );
  float tempC = tempK - 273.15;
  *(float *)temperature = tempC;
}

//***** Déclenchement mesure température ******
static bool Time_Trigger (float *temperature, float *humidity){
  static unsigned long MEMO_temps = millis( );   //Init MEMO
  
  if (millis( ) - MEMO_temps > 3000ul) {         //Si temps écoulé >10s
        Serial.println(*temperature);
        //bool tmp = DHT.measure( temperature, humidity );
        //Serial.println(tmp ? "1": "0");
  
        //if( DHT.measure( temperature, humidity ) == true ) {
        MEMO_temps = millis( );               //MEMO
        return(true);
  }  
  return( false );
}

//**************************************
//**************************************
//**************************************
void loop() {
  int Bouton = digitalRead(53);
  float Temperature, Humidite;
  char Tampon[255], Tampon_Temp[255], Tampon_Humid[255];

  lcd.display();
  //lcd.autoscroll();
  //lcd.scrollDisplayLeft();
  delay(300);

  //****** LED ********

  if (Bouton == LOW){
    digitalWrite(52,HIGH);
    delay(200);
    digitalWrite(52,LOW);
    delay(200);
    //lcd.clear;
    //lcd.print("Caca");
  }
  else{
    digitalWrite(52,HIGH);
  }
/*
  //**** Affichage température si mesure effectuée *******
  if( async_exec(10000ul, &temperature_reading, (void *)&Temperature) == true )
  {
    // # WTF    wizard skills
    dtostrf (Temperature, 2, 2, Tampon_Temp);
    //postData((void *)Tampon_Temp);
    sprintf(Tampon, "Smoke weed:%5s", Tampon_Temp); 
    lcd.clear();
    lcd.print(Tampon);          //Message !
  }
*/
  //if ( async_exec(5000ul, &postData, (void *)Tampon_Temp) == true ) {
  //  Serial.println("Yooolo");
  //}
}
