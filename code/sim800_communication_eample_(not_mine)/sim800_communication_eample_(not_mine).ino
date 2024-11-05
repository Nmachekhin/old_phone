#include <SoftwareSerial.h>

/*
AT+CPAS - module status:

0 – ready

2 – unknown

3 – incoming call

4 – voice connection
*/



SoftwareSerial sim800l(7, 8);
void setup()
{
  Serial.begin(9600);
  sim800l.begin(9600);

  Serial.println("Initializing...");

  sim800l.println("AT");
  updateSerial();
  sim800l.println("AT+CFUN=0");
  updateSerial();
  sim800l.println("AT+CFGRATPRIO=4");
  updateSerial();
  sim800l.println("AT+CFGDUALMODE=1,1");
  updateSerial();
  sim800l.println("AT+CFUN=1");
  updateSerial();
  sim800l.println("AT+CSQ");
  updateSerial();
  sim800l.println("AT+CCID");
  updateSerial();
  sim800l.println("AT+CREG?");
  updateSerial();
}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}