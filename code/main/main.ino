#include "Dial.h"
#include "SimCommands.h"
#include "SoundController.h"


const short int HANG_PIN=2;
const short int DIAL_PIN=3;

unsigned long global_timer;



SimModule sim800l(7, 8);
Dial PhoneDial(DIAL_PIN);
SoundController Amp=SoundController();

enum MainStatus{
  UNKNOWN_STATUS=0,
  IDDLE=1,
  DIALING=2,
  CALLING=3,
  SPEAKING=4,
  OPONENT_HANGED=5
};

MainStatus status=UNKNOWN_STATUS;

//AT+CPAS

SimStatus sim_status=UNKNOWN;

void setup() {
  Serial.begin(9600);
  pinMode(HANG_PIN, INPUT_PULLUP);
  pinMode(DIAL_PIN, INPUT_PULLUP);
  Serial.println("Initialization is complete!");
  global_timer=millis();
}


bool hanged()
{
    return !digitalRead(HANG_PIN);
}

void to_iddle()
{
  sim800l.play_tone(1,200);
  sim800l.hang();
  
  status=IDDLE;
  PhoneDial.reset_number();
  Serial.println("hanged");
}

void answer()
{
  sim800l.stop_tone();
  status=SPEAKING;
  sim800l.accept();
}


void loop() {

  
  global_timer=millis();

  if (hanged() && status!=IDDLE) to_iddle();
  if (hanged() && sim_status==INCOMMING) Amp.play_ring(global_timer);
  else Amp.play_silence(global_timer);
  if (!hanged() && status==IDDLE && sim_status==READY) 
  {
    status=DIALING;
    sim800l.play_tone(1,15200000);
    PhoneDial.reset_timers(global_timer);
  }
  if (!hanged() && status==SPEAKING && sim_status==READY)
  {
    status = OPONENT_HANGED;
    sim800l.play_tone(3,15200000);
  }
  if (!hanged() && status==IDDLE && sim_status==INCOMMING) answer();
  if(!hanged() && status==DIALING) 
  {
    PhoneDial.operate_dial(global_timer, sim800l);
    Serial.println(PhoneDial.get_number());
    if (PhoneDial.ready_to_call(global_timer)) call();
  }
  if (status==IDDLE || status==SPEAKING)
    sim800l.check_status(global_timer, sim_status);
  Serial.print(status);
  Serial.print(" ");
  Serial.println(sim_status);
}


void call()
{
    sim800l.play_tone(1,200);
    sim800l.hang();
    sim800l.dial(PhoneDial.get_number());
    status=SPEAKING;
    sim_status=INCALL;
}










/*void manual_sim_control()
{
  while (Serial.available()) 
  {
    sim800l.write(Serial.read());
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());
  }
}*/






