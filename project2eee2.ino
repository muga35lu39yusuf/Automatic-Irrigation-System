#include <SoftwareSerial.h>
SoftwareSerial gsm(11, 12); // RX, TX
int sensorPin = A0;
int sensorValue=0;
int percentValue=0;
int motor=5;
int upper;
int OPTION;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(sensorPin, INPUT);
pinMode(motor, OUTPUT);
gsm.begin(9600);
  modem_init();
  data_init();
  internet_init();
Serial.println("1:Automatic");Serial.println("2: custom");
Serial.println("Enter OPTION");
 while(Serial.available()==0){};
   OPTION=Serial.parseInt();
switch(OPTION){ 
  case 1:
 Serial.println("Default irrigation on");
  break;
  case 2:
    Serial.println("Enter upper limit");
   while(Serial.available()==0){};
   upper=Serial.parseInt();
  break;
  default:
  Serial.println("Invalid input");
  break;
}




}
void motorstate(int state){
digitalWrite(5,state);
}




void loop() {
  // put your main code here, to run repeatedly:
if(OPTION==1){
 custom();
 Send_data(); 
}
else if(OPTION==2){
  Autommatic();
  Send_data();
}
  else{
    Serial.println("Invalid input");
  }
}




void Autommatic(){
  sensorValue=analogRead(sensorPin);
Serial.println("AnalogValue");
Serial.println(sensorValue);
delay(1000);
percentValue=map(sensorValue,1023,200,0,100);
//percentValue=100-(sensorValue*100);
Serial.println("percentValue");
Serial.print(percentValue);
Serial.println("%");
if(percentValue>40){
motorstate(400);
digitalWrite(5,LOW);
}
else{
motorstate(1000);
digitalWrite(5,HIGH);
}
delay(1000);
}


 void custom(){
  sensorValue=analogRead(sensorPin);
Serial.println("AnalogValue");
Serial.println(sensorValue);
delay(1000);
percentValue=map(sensorValue,1023,200,0,100);
//percentValue=100-(sensorValue*100);
Serial.println("percentValue");
Serial.print(percentValue);
Serial.println("%");
if(percentValue>upper){
motorstate(400);
digitalWrite(5,LOW);
}
else{
motorstate(1000);
digitalWrite(5,HIGH);
}
delay(1000);
 }

 void modem_init()
{
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}
void data_init()
{
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000); delay(1000);
  gsm.println("AT+CPIN?");
  delay(1000); delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("contype");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("GPRS");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000); ;
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("APN");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  //------------APN------------//
  gsm.print("airtelgrps.com"); //APN Here
  //--------------------------//
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("USER");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("PWD");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(2000);
  gsm.print("AT+SAPBR=1,1");
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(3000);
}
void internet_init()
{
  Serial.println("Please wait.....");
  delay(1000);
  gsm.println("AT+HTTPINIT");
  delay(1000); delay(1000);
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("CID");
  gsm.print('"');
  gsm.print(',');
  gsm.println('1');
  delay(1000);
}
void Send_data()
{
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("URL");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("http:");
  gsm.print('/');
  gsm.print('/');
  //-----------------------Your API Key Here----------------------//
  //Replace put thewrite API key.
  gsm.print("api.thingspeak.com/update?api_key=XN1E50TNI7VF21BE&field1="); 
  //---------------------------------------------------------------//
  gsm.print(percentValue); //>>>>>>  variable 1
  delay(1000);
  gsm.println("AT+HTTPACTION=0");
  delay(1000);
}
