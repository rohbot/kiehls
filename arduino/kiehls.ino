/*

  
 Arduino code for Kiehls machine
 Reads analog input from photo transistors, hall effects
 Output switches on LEDS and controls motor
 Sends count values via serial to RaspberryPi
 
 */


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;

int cutoff = 400;

int led = 13;

int near = 0;

int counter = 0;

void setup()
{
  pinMode(led,OUTPUT);
  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  counter = 0; 
  cutoff = analogRead(A1);   
}

void loop() {
  // subtract the last readanalogRead(A1):
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(inputPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings; 

  int hall_pot = analogRead(A1);

  if(hall_pot > cutoff + 4 || hall_pot < cutoff - 4){
    cutoff = hall_pot;
    Serial.println(cutoff);
    
  }

  if(average < cutoff){
    digitalWrite(led, HIGH); 
    if(!near){
      Serial.print(counter);
      Serial.print("\t");
      Serial.print(cutoff);
      Serial.print("\t");
      Serial.println(average);
      counter++;
    }  
    near = 1;
  } 
  else{
    near = 0;
    digitalWrite(led, LOW); 
  } 
  // send it to the computer as ASCII digits
  //Serial.println(analogRead(A1));   
  delay(10);        // delay in between reads for stability            
}



