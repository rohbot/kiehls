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




int cutoff = 560;

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;

int motorPin = 3;
int redLED = 2;
int near = 0;


unsigned long time;

unsigned long dt;

unsigned long motorTimer;

unsigned long motorStartDelay;

unsigned long motorOnTime;


int hallEffectReading;

int motorStatus = 0;

int motorState = 0;

void allLEDOff(){
  digitalWrite(led1, HIGH); 
  digitalWrite(led2, HIGH); 
  digitalWrite(led3, HIGH); 
  digitalWrite(led4, HIGH); 
  digitalWrite(led5, HIGH); 

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
  
    for(int i =0; i < NUM_LASERS; i++){

    // initialize all the readings to 0: 
  
    for(int j=0; j < NUM_READINGS; j++){
      readings[i][j] = 0;
    }

    total[i] = 0;
    average[i] = 0;
    triggered[i] = 0;
    counter[i] = 0;

  }

  // Set Pin Numbers
  inputPins[LASER_1] = LASER_1_PIN;
  inputPins[LASER_2] = LASER_2_PIN;
  inputPins[LASER_3] = LASER_3_PIN;

  // set thresholds
  thresholds[LASER_1] = 90;
  thresholds[LASER_2] = 90;
  thresholds[LASER_3] = 90;


  // initialize serial communication with computer:
  Serial.begin(9600);                   

  time = millis();
  
  motorTimer = millis() + 5000;
  motorStartDelay =  millis() + 500;
  digitalWrite(motorPin, LOW); 
  allLEDOff();
  
}


void motorOn(){
  digitalWrite(motorPin, LOW);
  digitalWrite(redLED, LOW); 
  motorStatus = 1;
  if(motorState == 0){
    Serial.println("0 1 Motor On");
    motorOnTime = millis();
  }
  motorState = 1;

}

void motorOff(){
  digitalWrite(motorPin, HIGH);
  digitalWrite(redLED, HIGH); 
  if(motorState == 1){
    Serial.println("0 0 Motor Off");  
    allLEDOff();

  }  
  motorStatus = 0;
  motorState = 0;
}


void loop() {
  
  hallEffectReading = analogRead(HALL_EFFECT_PIN);
  dt = millis() - time;

  // if more than one sec between reads
  if(dt > 15000){
    //turn off LEDs
    allLEDOff();
    //motorStatus = 0;
  }


  if(millis() - motorTimer > 15000){
    motorOff();
  }

  if(millis() - motorOnTime > 20000){
    motorOff();
  }

  if(hallEffectReading < cutoff){
      
    if(!near && dt > 150){
//      digitalWrite(led, HIGH); 
      

      Serial.print(hallEffectReading);
      Serial.print("\t");
      Serial.print(dt);
      Serial.print("\t");
      Serial.println(motorStatus);

      if(dt < 300){
          digitalWrite(led5, LOW); 
          motorTimer = millis();

          if(motorStatus == 0){
            motorStartDelay = millis() - 1;
            motorStatus = 1;
          }


          if(millis() > motorStartDelay){
            motorOn();
          }  
          
        }else{
          digitalWrite(led5, HIGH);
          motorStatus = 0; 

        }
        
        if(dt < 325){
          digitalWrite(led4, LOW); 
  
        }else{
          digitalWrite(led4, HIGH); 

        }
        
        if(dt < 350){
          digitalWrite(led3, LOW); 
  
        }else{
          digitalWrite(led3, HIGH); 

        }
        if(dt < 375){
          digitalWrite(led2, LOW); 
  
        }else{
          digitalWrite(led2, HIGH); 

        }
        if(dt < 400){
          digitalWrite(led1, LOW); 
  
        }else{
          digitalWrite(led1, HIGH); 

        }    
    
    

      time = millis();
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

    if( average[i] < thresholds[i] ){

      //digitalWrite(LED_PIN, HIGH); 
      if(!triggered[i]){
        Serial.print(i + 1);  // display laser number
        Serial.print(" ");
        Serial.print(counter[i]);
        Serial.print(" ");
        Serial.println(average[i]);
        counter[i]++;
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


  //delay(10);        // delay in between reads for stability            
}


