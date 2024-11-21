#include "SimCommands.h"

SimModule::SimModule(uint8_t rx, uint8_t tx):sim800l(rx, tx)
{
  sim800l.begin(9600);
  sim800l.println("at+sndlevel=0,100");
  sim800l.println("at+cmic=0,15");
  sim800l.println("atl9");
  status_timer=0;
}

void SimModule::stop_tone()
{
  sim800l.println("at+sttone=0");
}

void SimModule::hang()
{
  sim800l.println("ATH");
}

void SimModule::accept()
{
  sim800l.println("ATA");
}


void SimModule::dial(String number)
{
  sim800l.println("atd"+number+";");
}

void SimModule::play_tone(short type, unsigned int time)
{
  sim800l.println("AT+STTONE=1,"+String(type)+","+String(time));
}



void SimModule::check_status(unsigned long global_timer, SimStatus& sim_status)
{
  if(global_timer-status_timer>=status_delay)
  {
    status_timer=global_timer;
    
    sim800l.println("AT+CPAS");
      String response="";
    while(!sim800l.available()); 
    while(sim800l.available()) 
    {
      response+=sim800l.readString();
    }
    //Serial.println(response);
    if (response.indexOf("+CPAS: 0") >=0 )
      sim_status=READY;
    else if (response.indexOf("+CPAS: 2") >=0 )
      sim_status=UNKNOWN;
    else if (response.indexOf("+CPAS: 3") >=0 )
      sim_status=INCOMMING;
    else if (response.indexOf("+CPAS: 4") >=0 )
      sim_status=INCALL;

  }


}
