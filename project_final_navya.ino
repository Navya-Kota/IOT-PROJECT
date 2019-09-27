
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(7 , 8);

int sensor_pin=A0;//declaring the analog input pin to read the sensor value
int output_value=0;//declaring the output analog pin to analyse the sensor value
//selecting DCmotor digital pin
int DCmotor1=5;
int DCmotor2=9;

//int RED=10;//selecting RED LED digital pin
//int GREEN=11;//selecting GREEN LED digital pin

void setup()
{
  //Declaring the selectedDigital pins as output pins
pinMode(DCmotor1,OUTPUT);
pinMode(DCmotor2,OUTPUT);
//pinMode(RED,OUTPUT);
//pinMode(GREEN,OUTPUT); 
  mySerial.begin(9600);  // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  
  Serial.print("Soil Moisture percentage detector");//displaying the aim of the device
delay(1000);
}


void loop() 
{ 
 //while(1)
 //{//infinite loop condition for the working of the device
  
 output_value=analogRead(sensor_pin);//reading the analoge value from the sensor
  output_value=map(output_value,550,0,0,100);
  Serial.print("Moisture : ");
  Serial.print(output_value);//displaying the percentage of soil moisture content
  //Serial.println("%");
  delay(1000);
  //checking whether the soil moisture content is adequate or not
  if(output_value<=40)
  {
    //enters the block if the soil moisture levels are not adequate
    //DCmotor is switched on
    digitalWrite(DCmotor1,HIGH);
    digitalWrite(DCmotor2,LOW);
    //digitalWrite(RED,HIGH);//RED LED glows
   Serial.println("Moisture level is low "); //message is displayed
   Serial.print("Motor is switched ON..");
  }
  
  else
  {
    //enters the block if the soil moisture levels are adequate
    //DCmotor is switched off
  digitalWrite(DCmotor1,LOW);
  digitalWrite(DCmotor2,LOW);
  //digitalWrite(GREEN,HIGH);//GREEN LED glows
   Serial.println("Moisture level is adequate "); //message is displayed
   Serial.print("Motor is switched OFF..");
    
  }
    Send2Pachube();
     if (mySerial.available())
    Serial.write(mySerial.read());
    }

void Send2Pachube()
{
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CPIN?");
  delay(1000);
  mySerial.println("AT+CREG?");
  delay(1000);
  mySerial.println("AT+CGATT?");
  delay(1000);
  mySerial.println("AT+CIPSHUT");
  delay(1000);
  mySerial.println("AT+CIPSTATUS");
  delay(2000);
  mySerial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
  mySerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
  ShowSerialData();
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
  ShowSerialData();
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  ShowSerialData();
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
String str="GET http://api.thingspeak.com/update?api_key=LUDGEE8QY1SNSMMO&field1=" + String(output_value);
  mySerial.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();
  mySerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println();
  ShowSerialData();
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}

 
