#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
#include "pitches.h"

TMRpcm tmrpcm;   
char riff[] = "riff00.wav"; //default riff

void setup(){
  
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  Serial.begin(9600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  
  //playRandRiff();
}

void loop(){ 
  playRandRiff();
  delay(5000);
}

void playRandRiff() {
  int num = random(0, 29);
  // convert random number to ascii and input into riff string
  riff[5] = num%10+0x30;  
  riff[4] = (num-num%10)/10+0x30;  
  tmrpcm.play(riff);
}
