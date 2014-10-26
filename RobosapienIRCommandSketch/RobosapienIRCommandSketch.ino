/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

#include <stdio.h>
#include <stdlib.h>

const int led = 2; //using external LED

const int boardLED=13;

const int waitMS = 25; //1 000 000 / 39 200 = 25.5102040816

const int headerPulse = 6666;

const int headerSpace = 3333;

const int onePulse = 833; //833*3 or 3333                                                                                    

const int oneSpace = 3333;

const int zeroPulse = 833;

const int zeroSpace = 833;

//VERY IMPORTANT NOTE
//delayMicros on an Edison will delay you a minimum of 90MS!!!!
void sendPair(int p, int s, int w)
{
  int targetMicros=micros()+p;
  
  int micros1;
  int micros2=micros();
  while(micros()<targetMicros)
  {
    int micros1=micros2;
    digitalWrite(led, HIGH);
    digitalWrite(boardLED, HIGH);
    int microsTarget=micros1+w;
    //digitalWrite(boardLED, HIGH);  
    //delayMicroseconds(1);
    int micros2;
    while((micros2=micros())<microsTarget)
    {}
    int H=micros2-micros1;
    
    micros1=micros2;
    digitalWrite(led, LOW);
    digitalWrite(boardLED, LOW); 
    microsTarget=micros1+w;
    while((micros2=micros())<microsTarget)
    {}
    /*Serial.println("High");
    Serial.println(H);
    Serial.println(w);
    Serial.println("Low");
    Serial.println(micros2-micros1);*/
  }
  delayMicroseconds(s);
}

void sendHeader()
{
  sendPair(headerPulse, headerSpace, waitMS);
};

void sendOne()
{
  //Serial.println("sendOne!");
  sendPair(onePulse, oneSpace, waitMS);
};


void sendZero()
{
  //Serial.println("sendZero!");
  sendPair(zeroPulse, zeroSpace, waitMS);
};

void sendCommand(unsigned char cmdIn)
{
  unsigned char cmd=cmdIn;
  Serial.println("Start cmd");
  Serial.println(cmd);
  sendHeader();
  for(int i=0; i<8; i++)
  {
    //Serial.println(cmd);
    if(cmd&128)
      sendOne();
    else
      sendZero();
    cmd=cmd<<1;
  }
  Serial.println("End cmd");
  
}
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output. 
  Serial.begin(9600);
  //Serial.println("Launching node");
  //system("node &");
  //Serial.println("Finished node launch");
  pinMode(boardLED, OUTPUT);   
  pinMode(led, OUTPUT);   
  
}

// the loop routine runs over and over again forever:
void loop() {            // wait for a second
  //digitalWrite(led, HIGH);  
  //
  /*
  sendHeader();
  //wave is 0x88
  sendOne();
  sendZero();
  sendZero();
  sendZero();
  sendOne();
  sendZero();
  sendZero();
  sendZero();
  */
  //
  //fopen("/tmp/robocmd","r");
  //sendCommand(0x8E); //0x88 high5 0x8E fart
  int maxfsize=500;
  
  system("curl https://blazing-fire-1158.firebaseio.com/robosapien.json > /tmp/robocmd.txt");
  char *cmdStr = (char*)malloc(maxfsize + 1);
  FILE *lockFile = fopen("/tmp/robolock", "r");
  if(!lockFile)
  {
    FILE *cmdFile = fopen("/tmp/robocmd.txt", "rb");
    if(cmdFile)
    {
      fseek(cmdFile, 0, SEEK_END);
      long fsize = ftell(cmdFile);
      if(fsize<3)
      {
        fclose(cmdFile);
        Serial.println("File length too short. Size in bytes is: ");
        Serial.println(fsize);
      }
      else
      {
        fseek(cmdFile, 1, SEEK_SET); //avoid leading quote
    
        fread(cmdStr, fsize, 1, cmdFile);
        fclose(cmdFile);
    
        cmdStr[fsize] = 0;
        cmdStr[fsize-1] = 0; //delete trailing quote
        
        int cmdInt=atoi(cmdStr);
        
        Serial.println("Command Number");
        Serial.println(cmdInt);
        if(cmdInt)
        {
          Serial.println("Executing Command");
          sendCommand(cmdInt); //0x88 high5 0x8E fart
          Serial.println("Command Executed, erasing command from firebase");
          system("curl -X PUT -d '\"0\"' https://blazing-fire-1158.firebaseio.com/robosapien.json");
          Serial.println("Command erased");
        }
      }
    }
  }
}
