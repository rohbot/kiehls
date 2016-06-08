/*

  
 Arduino code for Kiehls machine
 Reads analog input from photo transistors, hall effects
 Smoothing
 Output switches on LEDS and controls motor
 Sends count values via serial to RaspberryPi 
 
 */

#define NUM_READINGS 10
#define LED_PIN 13

#define NUM_LASERS 3

#define LASER_1     0
#define LASER_2     1
#define LASER_3     2

#define HALL_EFFECT_PIN A0
#define LASER_1_PIN     A1
#define LASER_2_PIN     A2
#define LASER_3_PIN     A3

int readings[NUM_LASERS][NUM_READINGS];  // the readings from the analog input
int index = 0;                            // the index of the current reading
int total[NUM_LASERS];                   // the running total
int average[NUM_LASERS];                 // the average

int inputPins[NUM_LASERS];               // the pin numbers

int thresholds[NUM_LASERS];               // threshold to trigger count 

int triggered[NUM_LASERS];

int counter[NUM_LASERS];

unsigned long lastTrigger[NUM_LASERS];    // Store time the last laser is triggered 

int laser_debounce = 2000;                // Time between laser pulses

int cutoff = 610;

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;

int motorPin = 3;
int redLED = 2;
int near = 0;

unsigned long motorRunTime = 20000;

unsigned long lastPassTime;

unsigned long dt;

unsigned long motorTimer;

unsigned long motorStartDelay;

int hallEffectReading;

int prevReading = 520;

int motorStatus = 0;

int motorState = 0;

int readingThreshold = 30;

void allLEDOff(int step_time){
  digitalWrite(led5, HIGH);
  delay(step_time);   
  digitalWrite(led4, HIGH);
  delay(step_time); 
  digitalWrite(led3, HIGH);
  delay(step_time); 
  digitalWrite(led2, HIGH);
  delay(step_time); 
  digitalWrite(led1, HIGH); 

}


void setup()
{
  pinMode(motorPin,OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);     
  pinMode(led3, OUTPUT);     
  pinMode(led4, OUTPUT);     
  pinMode(led5, OUTPUT);     
  pinMode(redLED,OUTPUT);
  digitalWrite(motorPin, HIGH);
  
    for(int i =0; i < NUM_LASERS; i++){

    // initialize all the readings to 0: 
  
    for(int j=0; j < NUM_READINGS; j++){
      readings[i][j] = 0;
    }

    total[i] = 0;
    average[i] = 0;
    triggered[i] = 0;
    counter[i] = 0;
    lastTrigger[i] = millis();
  }

  // Set Pin Numbers
  inputPins[LASER_1] = LASER_1_PIN;
  inputPins[LASER_2] = LASER_2_PIN;
  inputPins[LASER_3] = LASER_3_PIN;

  // set thresholds
  thresholds[LASER_1] = 500;
  thresholds[LASER_2] = 500;
  thresholds[LASER_3] = 500;


  // initialize serial communication with computer:
  Serial.begin(9600);                   

  lastPassTime = millis();
  
  motorTimer = millis() + motorRunTime;
  motorStartDelay =  millis() + 10;
  digitalWrite(motorPin, LOW); 

  allLEDOff(0);
    
}


void motorOn(){
  //digitalWrite(motorPin, HIGH);
  digitalWrite(motorPin, LOW);
  digitalWrite(redLED, LOW); 
  motorStatus = 1;
  if(motorState == 0)
    Serial.println("0 1 Motor On");
  
  motorState = 1;

}

void motorOff(){
  digitalWrite(motorPin, HIGH);
  //analogWrite(motorPin, 255);
  
  digitalWrite(redLED, HIGH); 
  if(motorState == 1){
    Serial.println("0 0 Motor Off");
    allLEDOff(250);  
  } 
  motorStatus = 0;
  motorState = 0;

}


void loop() {
  
  hallEffectReading = analogRead(HALL_EFFECT_PIN);
  dt = millis() - lastPassTime;

  // if more than one sec between reads
  if(dt > 1000 && !motorState){
    //turn off LEDs
    allLEDOff(0);
  }


  if(millis() - motorTimer > motorRunTime){
    motorOff();
  }


  if(hallEffectReading > cutoff && motorState == 0){
      
    if(!near && (hallEffectReading - prevReading > readingThreshold || hallEffectReading - prevReading < -readingThreshold ) && dt > 100 ){
//      digitalWrite(led, HIGH); 
      

      Serial.print(hallEffectReading);
      Serial.print("\t");
      Serial.print(dt);
      Serial.print("\t");
      Serial.println(motorStatus);

      if(dt < 200){
          digitalWrite(led5, LOW); 
          motorTimer = millis();

          motorOn();
            
       }else{
            if(motorState ==0){
              digitalWrite(led5, HIGH);
            }
          
          //motorStatus = 0; 

        }
        
        if(dt < 220){
          digitalWrite(led4, LOW); 
  
        }else{
          if(motorState ==0){
             digitalWrite(led4, HIGH); 
          }
        }
        
        if(dt < 250){
          digitalWrite(led3, LOW); 
  
        }else{
          if(motorState ==0){
            digitalWrite(led3, HIGH); 
          }
        }
        if(dt < 330){
          digitalWrite(led2, LOW); 
  
        }else{
          if(motorState ==0){
            digitalWrite(led2, HIGH); 
          }

        }
        if(dt < 400){
          digitalWrite(led1, LOW); 
  
        }else{
          if(motorState ==0){
            digitalWrite(led1, HIGH); 
          }
        }    
    
    

      lastPassTime = millis();
    }  
    near = 1;

  } 
  else{
    near = 0;

  } 
  // send it to the computer as ASCII digits
  if(hallEffectReading < cutoff){
    //Serial.println(hallEffectReading);  
  }
  

  //Read Laser Sensors

  for(int i=0; i < NUM_LASERS; i++){
    // subtract the last readanalogRead(A1):
    total[i]= total[i] - readings[i][index];         
    // read from the sensor:  
    readings[i][index] = analogRead(inputPins[i]); 
    // add the reading to the total:
    total[i] = total[i] + readings[i][index];       
    // advance to the next position in the array:  
   
    // calculate the average:
    average[i] = total[i] / NUM_READINGS; 
  
    //Check if average is above or below threshold depending on lowHighTrigger 


    if( average[i] < thresholds[i]  ){

      //digitalWrite(LED_PIN, HIGH); 
      if(!triggered[i] && millis() - lastTrigger[i] > laser_debounce){
        Serial.print(i + 1);  // display laser number
        Serial.print(" ");
        Serial.print(counter[i]);
        Serial.print(" ");
        Serial.println(average[i]);
        counter[i]++;
        lastTrigger[i] = millis();
      }  
      triggered[i] = 1;
    } 
    else{
      triggered[i] = 0;
      //digitalWrite(LED_PIN, LOW); 
    }
      
  } 

  

  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= NUM_READINGS)              
    // ...wrap around to the beginning: 
    index = 0;                           
  prevReading = hallEffectReading;

  //delay(10);        // delay in between reads for stability            
}


