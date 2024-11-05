#include <SoftwareSerial.h>

const bool DEBUG = true;
const short int HANG_PIN=2;

SoftwareSerial sim800l(7, 8);

enum MainStatus{
  UNKNOWN_STATUS=0,
  IDDLE=1
};

MainStatus status=UNKNOWN_STATUS;

//AT+CPAS
enum SimStatus {
  READY=0,
  UNKNOWN=2,
  INCOMMING=3,
  INCALL=4
};

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
  pinMode(HANG_PIN, INPUT_PULLUP);
}


bool hanged()
{
  if(DEBUG)
    return !digitalRead(HANG_PIN);
  else
    return digitalRead(HANG_PIN);
}

void to_iddle()
{
  sim800l.println("ath");
  status=IDDLE;
  Serial.println("hanged");
}


void loop() {
  if (DEBUG) manual_sim_control();
  if (hanged() && status!=IDDLE) to_iddle();
}


void manual_sim_control()
{
  while (Serial.available()) 
  {
    sim800l.write(Serial.read());
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());
  }
}