/*

 Smoothing
 
 Reads repeatedly from an analog input, calculating a running average
 and printing it to the computer.  Keeps ten readings in an array and 
 continually averages them.
 
 The circuit:
 * Analog sensor (potentiometer will do) attached to analog input 0
 
 Created 22 April 2007
 By David A. Mellis  <dam@mellis.org>
 modified 9 Apr 2012
 by Tom Igoe
 http://www.arduino.cc/en/Tutorial/Smoothing
 
 This example code is in the public domain.
 
 
 */


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 10;

int reading;      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;

int cutoff = 505;

int led = 13;
int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;

int near = 0;

int counter = 0;

unsigned long time;

unsigned long dt;




void setup()
{
  pinMode(led,OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);     
  pinMode(led3, OUTPUT);     
  pinMode(led4, OUTPUT);     
  pinMode(led5, OUTPUT);     

  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  counter = 0; 
  cutoff = analogRead(A1);  
  time = millis();
  cutoff = 505;
  
}

void loop() {
  
  reading = analogRead(inputPin);

  if(reading < cutoff){
      dt = millis() - time;
      
    if(!near && dt > 50){
      
      Serial.print(counter);
      Serial.print("\t");
      Serial.print(dt);
      Serial.print("\t");
      Serial.println(reading);
      counter++;

      if(dt < 400){
          digitalWrite(led5, HIGH); 
  
        }else{
          digitalWrite(led5, LOW); 

        }
        
        if(dt < 450){
          digitalWrite(led4, HIGH); 
  
        }else{
          digitalWrite(led4, LOW); 

        }
        
        if(dt < 500){
          digitalWrite(led3, HIGH); 
  
        }else{
          digitalWrite(led3, LOW); 

        }
        if(dt < 550){
          digitalWrite(led2, HIGH); 
  
        }else{
          digitalWrite(led2, LOW); 

        }
        if(dt < 600){
          digitalWrite(led1, HIGH); 
  
        }else{
          digitalWrite(led1, LOW); 

        }    
    
    

      time = millis();
    }  
    near = 1;

  } 
  else{
    near = 0;
   digitalWrite(led, LOW); 

  } 
  // send it to the computer as ASCII digits
  //Serial.println(average);   
  //delay(10);        // delay in between reads for stability            
}



