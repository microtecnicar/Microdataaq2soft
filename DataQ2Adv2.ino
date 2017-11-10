#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define DS1307_ADDRESS 0x68
#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

Adafruit_ADS1115 ads(0x48);
SoftwareSerial mySerial(3, 2); // RX, TX

byte zero = 0x00;
const int chipSelect = 4;
float Voltage0 = 0.0;
float Amp0 = 0.0;
float Amp1 = 0.0;
float Amp2 = 0.0;
float Amp3 = 0.0;
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 10000.0; // resistance of R2 (10K) - see text!
int value = 0;

  int second;
  int minute;
  int hour;
  int weekDay;
  int monthDay;
  int month;
  int year;
  
File dataFile;

void setup(void)
{
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Arial_bold_14);
  oled.clear();
  //oled.println("MTDataAq2");
  Serial.begin(9600);
  mySerial.begin(9600);
  ads.begin();
  oled.setCursor(0, 0 );
  oled.println("Signals (mA)");
  oled.print("S1>");
  oled.print(Amp0 / 100, 2);
  oled.print("   ");
  oled.print("S2>");
  oled.print(Amp1 / 100, 2);
  oled.println(" ");
  oled.print("S3>");
  oled.print(Amp2 / 100, 2);
  oled.print("   ");
  oled.print("S4>");
  oled.print(Amp3 / 100, 2);
  oled.println(" ");
  printDate();
  //setDateTime(); //MUST CONFIGURE IN FUNCTION
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
  Serial.println("Card failed, or not present");
    while (1) ;
  }
  //Serial.println("card initialized.");
  dataFile = SD.open("MTdl.txt", FILE_WRITE);
  if (! dataFile) {
  Serial.println("error opening datalog.txt");
    while (1) ;
  }
}

void loop(void)
{
  int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result

  adc0 = ads.readADC_SingleEnded(0);
  Voltage0 = (adc0 * 0.1875) / 1000;
  Voltage0 = Voltage0 * 100;
  Amp0 = map(Voltage0, 0, 500, 0, 2000);

  adc0 = ads.readADC_SingleEnded(1);
  Voltage0 = (adc0 * 0.1875) / 1000;
  Voltage0 = Voltage0 * 100;
  Amp1 = map(Voltage0, 0, 500, 0, 2000);

  adc0 = ads.readADC_SingleEnded(2);
  Voltage0 = (adc0 * 0.1875) / 1000;
  Voltage0 = Voltage0 * 100;
  Amp2 = map(Voltage0, 0, 500, 0, 2000);

  adc0 = ads.readADC_SingleEnded(3);
  Voltage0 = (adc0 * 0.1875) / 1000;
  Voltage0 = Voltage0 * 100;
  Amp3 = map(Voltage0, 0, 500, 0, 2000);

  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09) {
    vin = 0.0;
  }

  Serial.print(Amp0 / 100, 7);
  //Serial.print("6.5");
  Serial.print("A");
  mySerial.print("A");
  mySerial.print((Amp0 / 100)+50, 2);
  mySerial.println();

  Serial.print(Amp1 / 100, 7);
  //Serial.print("12.5");
  Serial.print("B");
  mySerial.print("B");
  mySerial.print((Amp1 / 100)+50, 2);
  mySerial.println();

  Serial.print(Amp2 / 100, 7);
  //Serial.print("16.5");
  Serial.print("C");
  mySerial.print("C");
  mySerial.print((Amp2 / 100)+50, 2);
  mySerial.println();

  Serial.print(Amp3 / 100, 7);
  //Serial.print("19.5");
  Serial.print("D");
  mySerial.print("D");
  mySerial.print((Amp3 / 100)+50, 2);
  mySerial.println();


  Serial.print(vin);
  Serial.println("F");
  mySerial.print(vin);
  mySerial.println("F");

  oled.setCursor(0, 0 );
  oled.println("Signals (mA)");
  oled.print("S1>");
  oled.print(Amp0 / 100, 2);
  oled.print("   ");
  oled.print("S2>");
  oled.println(Amp1 / 100, 2);
  oled.print("S3>");
  oled.print(Amp2 / 100, 2);
  oled.print("   ");
  oled.print("S4>");
  oled.println(Amp3 / 100, 2);
  printDate();
  delay(1000);

 String dataString = "";
 dataString += String(Amp0 / 100, 7);
 dataString += ",";
 dataString += String(Amp1 / 100, 7);
 dataString += ",";
 dataString += String(Amp2 / 100, 7);
 dataString += ",";
 dataString += String(Amp3 / 100, 7);
 dataString += ",";
 dataString += String(month);
 dataString += "/"; 
 dataString += String(monthDay);
 dataString += "/";  
 dataString += String(year);
 dataString += ","; 
 dataString += String(hour);
 dataString += ":"; 
 dataString += String(minute);
 dataString += ":"; 
 dataString += String(second);

 
 dataFile.println(dataString);
 dataFile.flush();
}

void setDateTime(){

  byte second =      00; //0-59
  byte minute =      48; //0-59
  byte hour =        21; //0-23
  byte weekDay =     5; //1-7
  byte monthDay =    9; //1-31
  byte month =       11; //1-12
  byte year  =       17; //0-99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();

}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  oled.print(month);
  oled.print("/");
  oled.print(monthDay);
  oled.print("/");
  oled.print(year);
  oled.print(" ");
  oled.print(hour);
  oled.print(":");
  oled.print(minute);
  oled.print(" ");
 //oled.print(":");
 //oled.println(second);

}
