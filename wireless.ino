/////////////////////////////////////////////////////////
//Liraries must be included to operate the module
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//Defining the ce(Chip enable pin) and csn(Slave Select)
#define ce 9
#define csn 10
///////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//a flag that indicates whether the radio listening state is started or stopped
bool listenIndicator =false;
/////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//making 2 pipes(channels) one for sending data and the other for receiving
//each pipe should has an address in the form of hexadecimal
const uint64_t p1 = 0xA2A2A2A2B4LL;
const uint64_t p2 = 0xE5E5E5E5E5LL;
////////////////////////////////////////////////////////////////////////////////

RF24 radio(ce,csn);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring of the Transceveing Functions:
//you can change the parameters for transmitted and received data from int to any other data type as string
void trans (int n);
int rec ();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {  
  Serial.begin(9600);
  radio.begin();
  radio.enableAckPayload();
  radio.setDataRate(RF24_250KBPS); //note: we choose a low transceveing data rate in order to increase our range to max.
  radio.openWritingPipe(p1);
  radio.openReadingPipe(1,p2);
}

void loop (){
  int readingValue = analogRead(potentiometer); //reading Value of the potentiometer on that Arduino which we want to send to the other Arduino
  int x=0; //x is the reading value of a potentiometer in the other arduino which we want to receive its reading from it.
  
  while(readingValue>600){
    readingValue=analogRead(potentiometer);
    trans(readingValue);
  }
  
  while(readingValue<600){
    readingValue=analogRead(potentiometer);
    x=rec();
    Serial.print("The recevied reading is: ");
    Serial.println(x);  //display any received data on the serial monitor.
  }
  
}

void trans (int n){
  if(listenIndicator==true){
    radio.stopListening();
    listenIndicator=false;
  }
  bool done=false;
  while (!done){
    done = radio.write(&n,sizeof(n));
  }
}

int rec (){
  int n;
  if(listenIndicator==false){
    radio.startListening();
    listenIndicator=true;
  }
  if(radio.available()){
    bool done=false;
    while(!done){
      done=radio.read(&n,sizeof(n));
    }
    return n;
  }
}
