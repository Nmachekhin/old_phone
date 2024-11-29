advanced_dial_test:
Test script for new dialing algorytm.


initialisation_test:
Basic ininiation and sim communication test, can be used for manual control of sim module.


main:
Main code for device operation.


main.ino:
Executable file that keeps track after system and sim module statuses, uses other mosules according to situation. It opperates to state varriables, that are independent: global state describes state of arduino, and sim state describes state of sim module and is read from it. It allows arduino to react on diffrent scenarious and sudden changes. Global timer allows for all modules to be in sync with each other, no metter wat time it takes to execute them (Exept the part with dial reading). Function tha reads dial has the highest priority and when has to be used main calls it and only it.

Dial.h / Dial.ino:
Module that is responsible for entering number with rotary dial.
int get_reading() - returns readings from dial pin.
void operate_dial(unsigned long, SimModule&) - detects pulses of from dial, needs global timer for operation and sim module module for canseling tone when entering number. It uses global timer only for calculating delay after uiser finishes entering number, sepparate timer is used for detecting pulses, it is more precise and is using microseconds. Code detects series of pulses, and then filters out only the real one to convert for input. The criteria is: pulse has to be longer then 11000 microsecconds and to be bigger then two neares neibours. This function needs to be called as frequent as possible.
void reset_timers(unsigned long) - sets internal timer to current time.
bool ready_to_call(unsigned long) - returns true if there is number entered and delay after last input has passed.
String get_number() - returns entered number.
void reset_number() - sets endered number to empty.


SimCommands.h / SimCommands.ino
Module that is responsible for communication with sim module using at commands.
Constructor sends commands to sim module with all required settings.
void stop_tone() - stops any tone what is playing through speaker.
void play_tone(short, unsigned int) - plays tone through speakers, takes tone type and duration in millisecconds as parameters.
void hang() - commands module ho terminate call.
void accept() - commands module to accept incomming call.
void dial(String) - dials selected number (if there are call in progress already, it will be terminated).
void check_status(unsigned long, SimStatus&) - checks status of sim module, takes global timer and sim status as parameters, sets sim status to corresponding one. It sends check status command to sim module and awaits for answer, according to which sets sim status. Because of waiting for response, this function may take a while to opperate, and it is called only when dial is not in use. Also it has its own delay between requests, to prevent spamming.


SoundController.h / SoundController.ino
const unsigned char ring_sound[] PROGMEM - array of pwm values to play sound using only arduino. Stored in program memory section.
void play_ring(unsigned long) - plays ring one time, takes global timer to not start playback again when already playing.
play_silence(unsigned long) - pulls playback pin to low to prevent aplifyer playing static noises.