#include "Dial.h"
#include "SimCommands.h"
#include "SoundController.h"

bool call_started=false;
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
  //Serial.println("hanged");
  call_started=false;
}

void answer()
{
  sim800l.stop_tone();
  status=SPEAKING;
  sim800l.accept();
}


void call()
{
    sim800l.play_tone(1,200);
    sim800l.hang();
    sim800l.dial(PhoneDial.get_number());
    status=SPEAKING;
}

void loop() {

  //update gllobal timer
  global_timer=millis();

  //updating dial
  if(!hanged() && status==DIALING) 
  {
    PhoneDial.operate_dial(global_timer, sim800l);
    //Serial.println(PhoneDial.get_number());
    if (PhoneDial.ready_to_call(global_timer)) 
    {
      //Serial.println(PhoneDial.get_number());
      call();
    }
    return;
  }

  //check weather hanged
  if (hanged() && status!=IDDLE) to_iddle();

  //playing coresponding sounds
  if (hanged() && sim_status==INCOMMING) Amp.play_ring(global_timer);
  else Amp.play_silence(global_timer);

  //entering dial mode
  if (!hanged() && status==IDDLE && sim_status==READY) 
  {
    status=DIALING;
    sim800l.play_tone(1,15200000);
    PhoneDial.reset_timers(global_timer);
  }

  //checking weather oponent has ended a call
  if (!hanged() && status==SPEAKING && sim_status!=INCALL && call_started)
  {
    status = OPONENT_HANGED;
    sim800l.play_tone(3,15200000);
  }

  //answering to incoming call
  if (!hanged() && (status==IDDLE || status==DIALING) && sim_status==INCOMMING) answer();

  //checking sim module status
  if (status==IDDLE || status==SPEAKING)
    sim800l.check_status(global_timer, sim_status);


  if (status==SPEAKING && sim_status==INCALL)
    call_started=true;
}