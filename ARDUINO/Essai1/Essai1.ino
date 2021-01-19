#include <dht_nonblocking.h>        //Capteur Temp
#include <LiquidCrystal.h>          //LCD
#define DHT11_PIN 12                //PIN Capteur Temp
#define DHT11_TYPE 0                //TYPE Capteur Temp

DHT_nonblocking DHT(DHT11_PIN, DHT11_TYPE);
LiquidCrystal lcd(27, 26, 25, 24, 23, 22);

void setup() {
  pinMode(52, OUTPUT);                //LED
  pinMode(53, INPUT);                 //Bouton
  digitalWrite(52, HIGH);             //Init LED
  lcd.begin(16, 2);                   //Colonne / ligne
  lcd.print("Smoke Weed !");          //Message !
}

//***** Déclenchement mesure température ******
static bool Time_Trigger (float *temperature, float *humidity){
  static unsigned long MEMO_temps = millis( );   //Init MEMO
  
  if (millis( ) - MEMO_temps > 10000ul) {         //Si temps écoulé >10s
        if( DHT.measure( temperature, humidity ) == true ) {
          MEMO_temps = millis( );               //MEMO
          return(true);
        }
  }  
  return( false );
}

//**************************************
//**************************************
//**************************************
void loop() {
  int Bouton = digitalRead(53);
  float Temperature, Humidite;

  lcd.display();
  //lcd.autoscroll();
  lcd.scrollDisplayLeft();
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

  //**** Affichage température si mesure effectuée *******
  if( Time_Trigger( &Temperature, &Humidite ) == true )
  {
    char Tampon[255], Tampon_Temp[255], Tampon_Humid[255];
    // # WTF    wizard skills
    dtostrf (Temperature, 2, 1, Tampon_Temp);
    dtostrf (Humidite, 2, 1, Tampon_Humid);
    sprintf(Tampon, "Smoke weed:%4s SnoopDog:%4s%%", Tampon_Temp, Tampon_Humid); 
    lcd.clear();
    lcd.print(Tampon);          //Message !
  }
}
