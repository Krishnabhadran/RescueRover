#include<Servo.h>
Servo s1,s2,s3,s4,s5,s6;
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte address[6] = "1";

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int posy,posx,posc;
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1_Y;
  byte joy1_X;
  byte j1Button;
  byte joy2_Y;
  byte joy2_X;
  byte j2Button;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  
  
};

Data_Package data; //Create a variable with the above structure

void setup() {
  s1.attach(9);// 9th servo not working
  s2.attach(6);
  s3.attach(5);
  s4.attach(4);

posy=90;
posx=180;
posc=90;
  Serial.begin(1000000);
  radio.begin();
  radio.openReadingPipe(0, address);
 // radio.setAutoAck(false);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  // Print the data in the Serial Monitor
  Serial.print("joy1_X: ");
  Serial.print(data.joy1_X);
  Serial.print("; joy1_Y: ");
  Serial.print(data.joy1_Y);
  Serial.print("; joy2_X: ");
  Serial.print(data.joy2_X); 
  Serial.print("; joy2_Y: ");
  Serial.print(data.joy2_Y);
  Serial.print("; button1: ");
  Serial.print(data.button1);
  Serial.print("; button2: ");
  Serial.print(data.button2);

  Serial.print("; j1Button: ");
  Serial.print(data.j1Button);
  Serial.print("; j2Button: ");
  Serial.print(data.j2Button);
  Serial.print("; tSwitch1: ");
  Serial.print(data.tSwitch1);
  Serial.print("; tSwitch2: ");
  Serial.print(data.tSwitch2);
  Serial.println("");
 
 

 
  Serial.print(";  ");

  Serial.println("  ");
   if(data.button2==0 && data.tSwitch1==1){


  if(data.joy1_Y>150){
   
 analogWrite(A0,255);
     analogWrite(A1,0);
     analogWrite(A2,255);
     analogWrite(A3,0);

  } else if(data.joy1_Y<110){
 

   analogWrite(A0,0);
     analogWrite(A1,255);
     analogWrite(A2,0);
     analogWrite(A3,255);
   
  }else if(data.joy2_X>150){

   
    analogWrite(A0,255);
     analogWrite(A1,0);
      analogWrite(A2,0);
     analogWrite(A3,255);
   
  }else if(data.joy2_X<110){
     
      analogWrite(A0,0);
     analogWrite(A1,255);
      analogWrite(A2,255);
     analogWrite(A3,0);

      
    }else {
     analogWrite(A0,0);
     analogWrite(A1,0);
     analogWrite(A2,0);
     analogWrite(A3,0);
  }} //1st if
  
  if (data.button1==1){digitalWrite(10,1);
  }else{
    digitalWrite(10,0);
    
  }//vibrater


  if( data.button2==1 && data.tSwitch1==1){
    
  if(data.joy1_Y>150){
   
     analogWrite(A4,255);
     analogWrite(A5,0);

  } else if(data.joy1_Y<110){
     analogWrite(A4,0);
     analogWrite(A5,255);
  }else if(data.joy2_X>150){
   s1.write(0);
   
  }else if(data.joy2_X<110){ //servo
s1.write(180);
      
    }else {
     analogWrite(A4,0);
     analogWrite(A5,0);
     s1.write(90);}
  }// 2nd if condition


 if(data.tSwitch1==0){
   analogWrite(A0,0);
     analogWrite(A1,0);
     analogWrite(A2,0);
     analogWrite(A3,0);
  analogWrite(A4,0);
     analogWrite(A5,0);
 if(data.joy1_Y>150){
   if(posy<=180)
   posy=posy+5;
   delay(100);
     s2.write(posy);
    

  } else if(data.joy1_Y<110){
    if(posy>=0)
   posy=posy-5;
   delay(100);
     s2.write(posy);
     
  } else if(data.joy1_X<110){
    if(posc>=0)
   posc=posc-5;
   delay(100);
     s3.write(posc);
     
  }else if(data.joy1_X<110){
    if(posc<=180)
   posc=posc+5;
   delay(100);
     s3.write(posc);
     
  }else if(data.joy2_X>150){
     if(posx>=0)
   posx=posx-5;
   delay(100);
     s4.write(posx);
   
  }else if(data.joy2_X<110){ //servo
  if(posx<=180)
   posx=posx+2;
   delay(100);
     s4.write(posx);
      
    }else if(data.j1Button==0){
      digitalWrite(3,1);

    }else {
     analogWrite(2,0);
     analogWrite(3,0);
     }
 }

}//Loop

void resetData() {
  // Set initial default values
  data.joy1_X = 127;
  data.joy1_Y = 127;
  data.joy2_X = 127;
  data.joy2_Y = 127;
  data.j1Button = 1;
  data.j2Button = 1;
 
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  

}
