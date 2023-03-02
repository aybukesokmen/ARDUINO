#include <SPI.h> 
#include <MFRC522.h>          
#include <Servo.h>               

int RST_PIN = 9; //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10; //RC522 modülü chip select pinini tanımlıyoruz.
int servo_kapi = 8; //Servo motor pinini tanımlıyoruz.
bool dogru_kullanici=0;


int Gaz_sensor = 0;
int Temps = 0;
int ldr_pin = A0;
int sic_sensor = A2;

Servo motor_kapi;
MFRC522 rfid(SS_PIN,RST_PIN);
byte ID[4]={0,0,0,0};
byte ID1[4]={165,118,215,101};
String kullanici[2]={"Oguz","Kaan"};

int pir_sensor=7;
int ofis_lamba=5;
int buzzer=6;
int gaz_sensor_buzzer=4;

void rfid_kontrol()
{
   //kart okundugunda id nin surekli gelmemesi icin
  if(!rfid.PICC_IsNewCardPresent())
  {
    return;
  }
    if(!rfid.PICC_ReadCardSerial())
  {
    return;
  }
 
    if(rfid.uid.uidByte[0]==ID1[0] && rfid.uid.uidByte[1]==ID1[1] && rfid.uid.uidByte[2]==ID1[2]&& rfid.uid.uidByte[3]==ID1[3])
    {
        Serial.println("Hosgeldin");
        Serial.println(kullanici[0]);

        Serial.println("Kapi Acildi");
        ekranaYazdir();
        motor_kapi.write(180);
        delay(3000);
        motor_kapi.write(0);
        id_sifirla();
        dogru_kullanici=1;

    }
      else
    {
        Serial.println("Yetkisiz");
        ekranaYazdir();
        id_sifirla();
        dogru_kullanici=0;

    }
    

  rfid.PICC_HaltA();
  
}
void ekranaYazdir()
{
  Serial.println("ID Numarasi ");
  for(int i=0;i<4;i++)
  {Serial.print(rfid.uid.uidByte[i]);
  Serial.print(" ");}
  Serial.println(" ");
  
}

void id_sifirla()
{
  for(int i=0;i>4;i++)
  {
    ID1[i]=0;
  }
}

void ofis_isik()
{
    int isik = analogRead(A0); //Işık değişkenini A0 pinindeki LDR ile okuyoruz
    Serial.println(isik); //Okunan değeri seri iletişim ekranına yansıtıyoruz
     delay(50);

   if(isik<90)
   {
    digitalWrite(ofis_lamba,HIGH);
    
   }
}
void hirsiz_alarmi()
{
  if(pir_sensor==HIGH && servo_kapi==LOW)
  {
    digitalWrite(6,HIGH);
  }
  
}

void gaz_sensoru_fonksiyonu()
{
  Gaz_sensor = analogRead(A1);
  Serial.println(Gaz_sensor);
  if (Gaz_sensor >= 220) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }

}
void sicaklik_fonksiyonu()
{
  Temps = (-40 + 0.488155 * (analogRead(A2) - 20));
  //Serial.println(Temps);
  if (Temps >= 26) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
  
}


void setup() {
  motor_kapi.attach(servo_kapi);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();//okumaya baslanmasi icin 
  pinMode(pir_sensor, INPUT);
  pinMode(ofis_lamba, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(4, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(3, OUTPUT);
  
}

void loop() {
  rfid_kontrol();
  if(dogru_kullanici==1)
  {

      dogru_kullanici=0;
      Serial.println("Kapi Kapandi");
      
  }

 
}
