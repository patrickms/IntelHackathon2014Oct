/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int led = 2; //using external LED

const int boardLED=13;

const int waitMS = 25; //1 000 000 / 39 200 = 25.5102040816

const int headerPulse = 6666;

const int headerSpace = 3333;

const int onePulse = 833; //833*3 or 3333                                                                                    

const int oneSpace = 3333;

const int zeroPulse = 833;

const int zeroSpace = 833;

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
  sendPair(onePulse, oneSpace, waitMS);
};


void sendZero()
{
  sendPair(zeroPulse, zeroSpace, waitMS);
};
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output. 
  Serial.begin(9600);
  Serial.println("boo!");
  pinMode(boardLED, OUTPUT);   
  pinMode(led, OUTPUT);   
  
}

// the loop routine runs over and over again forever:
void loop() {            // wait for a second
  //digitalWrite(led, HIGH);  
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
  delay(2000);   
}
