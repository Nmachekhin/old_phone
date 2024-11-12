#include <SPI.h>
#include <SD.h>

void setup() {
  // put your setup code here, to run once:
  SD.begin(4);
  Serial.begin(9600);
  File dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.write("Hello world!");
  dataFile.close();

  File myFile = SD.open("data.txt");
  if (myFile) {
    Serial.println("data.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
