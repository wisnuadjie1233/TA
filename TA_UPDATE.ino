/* CATATAN 

   -NEXTION : kabel biru pin 2, kabel kuning pin 3
   -Komunikasi serial nextion --> arduino
      #include <SoftwareSerial.h>

      SoftwareSerial lcd(2,3);

      void setup(){
        Serial.begin(9600);
        lcd.begin(9600);
      }

      void loop(){
        if(lcd.available()){
          String y = lcd.readString();
          Serial.println(y);
        }
      }

*/

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <BH1750.h>
#include <Wire.h>

#define DHTPIN 11 
#define DHTTYPE DHT22 

SoftwareSerial lcd(2,3);
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

const int arraySize = 3;
int lock = 0; 
int lampu = 12;
int kelembaban;
int dats[arraySize];     
int count = 0; 

float temperatur;

uint16_t Address = 0;
uint16_t lux;

char data[] = "ANGGREK";
char data2[] = "ANTHURIUM";

unsigned long interval22 = 0;
unsigned long interval2 = 0;
unsigned long interval = 0;
String LCDREAD;

struct ModeTanamanHias {

  int kelembabans;
  int suhus;
  uint16_t cahayas;

};

void running(){

  if(millis() - interval > 1000){
      TampilkanDHT(kelembaban, temperatur);
      TampilkanLux(lux);
      DisplayMode();
      interval = millis();
  }

}

void DisplayMode(){

    lcd.print("t00.txt=\"");

      char Mode1[100];
      for(int index = 0; index < sizeof(Mode1); index++){
        Mode1[index] = EEPROM.read(Address + index);
        if(Mode1[index] == '\0'){
          break;
        }
      }

    lcd.print(Mode1);
    lcd.print("\"");
    lcd.write(0xff);
    lcd.write(0xff);
    lcd.write(0xff);

}

void TampilkanDHT(int kelembaban,  float suhu){

     lcd.print("t3.txt=\"");
  lcd.print(suhu,1);
  lcd.print("`C");
  lcd.print("\"");
  lcd.write(0xff);
  lcd.write(0xff);
  lcd.write(0xff);

    lcd.print("t4.txt=\"");
  lcd.print(kelembaban);
  lcd.print("\"");
  lcd.write(0xff);
  lcd.write(0xff);
  lcd.write(0xff);

}

void TampilkanLux(int lux){

  lcd.print("t5.txt=\"");
  lcd.print(lux);
  lcd.print("\"");
  lcd.write(0xff);
  lcd.write(0xff);
  lcd.write(0xff);

}

void runPlantSystem(ModeTanamanHias &plantMode, int choice){
 
  String mode[] = {"anggrek", "anthurium", "jandabolong"};
  byte value;

      if(mode[choice].compareTo("anggrek") == 0){
        
        for(int index = 0; index < sizeof(data); index++){
          EEPROM.update(Address + index, data[index]);
        }

        char BacaData[sizeof(data)];
        for(int index2 = 0; index2 < sizeof(BacaData); index2++){
          BacaData[index2] = EEPROM.read(0 + index2);
        }

        Serial.println(BacaData);

      } else if(mode[choice].compareTo("anthurium") == 0){
        Serial.println("anthurium");

         for(int index = 0; index < sizeof(data2); index++){
          EEPROM.update(Address + index, data2[index]);
        }

        char BacaData[sizeof(data2)];
        for(int index2 = 0; index2 < sizeof(BacaData); index2++){
          BacaData[index2] = EEPROM.read(0 + index2);
        }
      }

}

//void yourFunction() {
//    int arraySize = sizeof(dats) / sizeof(dats[0]); // Mendapatkan ukuran array dats
//
//    // Simpan nilai dats[] ke dalam array lain
//    int copiedArray[arraySize];
//    for (int i = 0; i < arraySize; i++) {
//        copiedArray[i] = dats[i];
//    }
//
//    int datss = copiedArray[0] * 100 +  copiedArray[1] * 10 +  copiedArray[2];
//    lcd.print("A1.txt=\"");
//    lcd.print(datss);
//    lcd.print("\"");
//    lcd.write(0xff);
//    lcd.write(0xff);
//    lcd.write(0xff);
//
//}

void setup() {

  pinMode(lampu, OUTPUT);
  Serial.begin(9600);
  lcd.begin(9600);
  Wire.begin();
  dht.begin();
  lightMeter.begin();

}

void loop(){
  int f = 1;
  int x; int z; int count = 0; 
  ModeTanamanHias modeTerpilih;
  unsigned long Interval3;

  kelembaban = dht.readHumidity();
  temperatur = dht.readTemperature();
  lux = lightMeter.readLightLevel();
  running();

    if (lcd.available() && lock == 0) {
              z = lcd.read(); 
              Serial.println(z); 
              if(isPrintable(z)){ 
                Serial.println(z);
              }
              if(z == 83){
                lock = 1;
              } 
    }

   if (lcd.available() && lock == 1) {
            x = lcd.read();
            if(isPrintable(x)){ 
              Serial.println(x);
            }
            if(x == 84){
              lock = 2;
            }
            if(x == 'A'){
              lock = 0;
            }
        }     

    while (lcd.available() && count < arraySize && lock == 2) {

          char c = lcd.read();
          if(c == 83){
            lock = 1;
          }
          if (c >= '0' && c <= '9') { 
            dats[count] = c - '0';    
            count++;                  
          }

    }

    if (count > 0) {
      Serial.print("Angka yang terbaca: ");
      for (int i = 0; i < count; i++) {
        Serial.print(dats[i]); // Mencetak angka ke Serial Monitor
      }
//      if(millis() - interval22 > 100){
//        yourFunction();
//        interval22 = millis();
//      }
      Serial.println(); // Pindah ke baris baru setelah mencetak
    }
     x = x - 49;
     if (x == 0){

      modeTerpilih = {80, 28, 1500};

      Serial.println("1d");

     }else if(x == 1){

      modeTerpilih = {75, 25, 1100};

      Serial.println("2d");
    }

  delay(200);
  runPlantSystem(modeTerpilih, x);
    
}

// #include <PID_v1.h> // deklarasi library PID
// #include <BH1750.h> // deklarasi library BH1750
// BH1750 lightMeter; // deklarasi objek lightmeter dari class BH1750

// double Setpoint ; // setpoin akan diatur berdasarkan keinginan user
// double Input; // cahaya luaryang diterima sensor BH1750
// double Output ; // growlight
// double Kp=0, Ki=10, Kd=0; // deklarasi satuan nilai kp(proportional), ki(integral), kd(derivative)
// int RelayGrowLight = 3 //deklarasi relay growlight pada pin 3
// PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT); // deklarasi objek myPID dari class PID

// void setup(){
//   pinMode(RelayGrowLight, OUTPUT);
//   Serial.begin(9600); // memulai serial monitor
//   Wire.begin(); // memulai protokol komunikasi
//   lightMeter.begin(); // memulai objek lightmeter
//   myPID.SetMode(AUTOMATIC); //setting objek myPID ke otomatis
//   myPID.SetTunings(Kp, Ki, Kd); //set tuning nilai proportional, integral dan derivative
// }

// void loop(){
//   Setpoint = 1000; //setpoint nilai input (lux) maksimal tidak lebih dari 1000
//   float lux = lightMeter.readLightLevel(); //membaca nilai lux dari sensor
//   Serial.print("Light: "); //menampilkan nilai lux pada serial monitor
//   Serial.print(lux);
//   Serial.println(" lx");

//       Input = map(lux, 0, 54612.50, 0, 255); //merubah nilai input dengan fungsi mapping (dari range 0 - 54612 ke 0 - 255)
//       myPID.Compute(); //menghitung nilai input dengan PID

//   analogWrite(RelayGrowLight, Output); //merubah kondisi relay berdasarkan input perhitungan PID
//   Serial.print(Input); //menampilkan nilai input di serial monitor
//   Serial.print(" ");
//   Serial.println(Output); //menampilkan nilai output di serial monitor
//   Serial.print(" ");  
//   Serial.println(Setpoint); //menampilkan nilai setpoin di serial monitor
//   delay(100); // berikan jeda 100 milisecond
// }

// #include "DHT.h" // onclude library DHT
// #define DHTPIN 2 // mendefinisikan DHT pada pin 2   
// #define DHTTYPE DHT22 // mendefinisikan tipe DHT ke DHT 22

// DHT dht(DHTPIN, DHTTYPE); // membuat objek dht dari class DHT

// #define Relay_Lampu 3 // mendefinisikan pin lampu pada pin 3
// #define Relay_PELTIER 4 // mendefinisikan pin peltier pada pin 4
// #define Relay_MISTMAKER 5 // mendefinisikan pin mistmaker pada pin 5
// #define Relay_EXHAUSTFAN 6 // mendefinisikan pin exhaustfan pada pin 6

// void setup() {
//   Serial.begin(9600);
//   dht.begin();

//   pinMode(Relay_Lampu, OUTPUT);
//   pinMode(Relay_PELTIER, OUTPUT);
//   pinMode(Relay_MISTMAKER, OUTPUT);
//   pinMode(Relay_EXHAUSTFAN, OUTPUT);
// }

// void loop() {

//   delay(2000);

//   float h = dht.readHumidity();
//   float t = dht.readTemperature();

//   if (isnan(h) || isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   Serial.print(F("Kelambaban: "));
//   Serial.print(h);
//   Serial.print(F("%  Temperatur: "));
//   Serial.print(t);
//   Serial.println(F("°C"));

//   // kontrol Lampu
//   if (t < 18) {
//     digitalWrite(LAMP_PIN, HIGH); // nyalakan relay lampu
//   } else {
//     digitalWrite(LAMP_PIN, LOW);  // matikan relay lampu
//   }

//   // kontrol Peltier
//   if (t > 27) {
//     digitalWrite(PELTIER_PIN, HIGH); // nyalakan relay peltier
//   } else {
//     digitalWrite(PELTIER_PIN, LOW);  // matikan relay peltier
//   }

//   // kontrol Mistmaker
//   if (h < 50) {
//     digitalWrite(MISTMAKER_PIN, HIGH); // nyalakan relay Mistmaker
//   } else {
//     digitalWrite(MISTMAKER_PIN, LOW);  // matikan relay Mistmaker
//   }

//   //kontrol exhaustfan
//   if (h > 80) {
//     digitalWrite(EXHAUST_FAN_PIN, HIGH); // nyalakan relay exhaustfan
//   } else {
//     digitalWrite(EXHAUST_FAN_PIN, LOW);  // matikan relay exhaustfan
//   }
// }

// #include <SoftwareSerial.h> //deklarasi library I2C (RX dan TX)
// #include "DHT.h" //deklarasi library DHT22
// #include <BH1750.h> //deklarasi library BH1750
// #include <Wire.h> //deklarasi library untuk protokol komunikasi I2C
// #define DHTPIN 11  //pendefinisian DHT pada pin 11
// #define DHTTYPE DHT22 //pendefinisian tipe DHT (DHT22)

// SoftwareSerial lcd(2,3); //deklarasi pin I2C (2 = RX, 3 = TX)
// BH1750 lightMeter; //deklarasi objek lightmeter dari class BH1750
// DHT dht(DHTPIN, DHTTYPE); //deklarasi objek dht dari class DHT

// void setup() {
//   Serial.begin(9600); //memulai serial komunikasi dengan kecepatan baud rate 9600
//   lcd.begin(9600); //memulai komunikasi software serial dengan kecepatan 9600 baud rate
//   Wire.begin(); //memulai protokol komunikasi
//   dht.begin(); //memulai objek dht
//   lightMeter.begin(); memulai objek lightmeter

// }

// void loop(){
//   int lux = lightMeter.readLightLevel(); //membaca nilai lux yang akan disimpan didalam variable 'lux'
//   Serial.print("Light: "); // ===============================
//   Serial.print(lux); // print nilai lux pada serial monitor
//   Serial.println(" lx"); // ===============================

//   int h = dht.readHumidity(); //membaca nilai kelembaban
//   int t = dht.readTemperature(); // membaca nilai temperatur

//     Serial.print(F("Humidity: ")); // menampilkan nilai kelembaban pada serial monitor
//   Serial.print(h);
//   Serial.print(F("%  Temperature: ")); // menampilkan suhu pada serial monitor
//   Serial.print(t);
//   Serial.print(F("°C "));
  
//   //barisan kode nextion
//     lcd.print("x0.val="); // proses mengirim nilai variable 't' (temperatur) pada display nextion
//   lcd.print(t);
//   lcd.write(0xff);
//   lcd.write(0xff);
//   lcd.write(0xff);
//     lcd.print("x1.val="); // proses mengirim nilai variable 'h' (humidity) pada display nextion
//   lcd.print(h);
//   lcd.write(0xff);
//   lcd.write(0xff);
//   lcd.write(0xff);
//     lcd.print("x2.val="); // proses mengirim nilai variable 'lux' (lux) pada display nextion
//   lcd.print(lux);
//   lcd.write(0xff);
//   lcd.write(0xff);
//   lcd.write(0xff);

//   delay(1000); // beri jeda selama 1 detik
// }
