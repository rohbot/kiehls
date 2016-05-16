/*

  
 Arduino code for Kiehls machine
 Reads analog input from photo transistors, hall effects
 Smoothing
 Output switches on LEDS and controls motor
 Sends count values via serial to RaspberryPi 
 
 */

#define NUM_READINGS 10
#define LED_PIN 13

#define NUM_SENSORS 4

#define HALL_EFFECT 0
#define LASER_1     1
#define LASER_2     2
#define LASER_3     3

#define HALL_EFFECT_PIN A0
#define LASER_1_PIN     A1
#define LASER_2_PIN     A2
#define LASER_3_PIN     A3


int readings[NUM_SENSORS][NUM_READINGS];  // the readings from the analog input
int index = 0;                            // the index of the current reading
int total[NUM_SENSORS];                   // the running total
int average[NUM_SENSORS];                 // the average

int inputPins[NUM_SENSORS];               // the pin numbers

int thresholds[NUM_SENSORS];               // threshold to trigger count 

int lowHighTrigger[NUM_SENSORS];            // indicate whether to trigger above (1) or below (0) threshold

int triggered[NUM_SENSORS];

int counter[NUM_SENSORS];

void setup()
{
  pinMode(LED_PIN,OUTPUT);
  // initialize serial communication with computer:
  Serial.begin(9600);                   

  // Initialise arrays  
  for(int i =0; i < NUM_SENSORS; i++){

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
  inputPins[HALL_EFFECT] = HALL_EFFECT_PIN;
  inputPins[LASER_1] = LASER_1_PIN;
  inputPins[LASER_2] = LASER_2_PIN;
  inputPins[LASER_3] = LASER_3_PIN;

  // set thresholds
  thresholds[HALL_EFFECT] = 500;
  thresholds[LASER_1] = 90;
  thresholds[LASER_2] = 90;
  thresholds[LASER_3] = 90;
  
  lowHighTrigger[HALL_EFFECT] = 0;      // Hall effect sensor triggers below threshold
  lowHighTrigger[LASER_1] = 1;          //Photo transistors trigger above threshold
  lowHighTrigger[LASER_2] = 1;
  lowHighTrigger[LASER_3] = 0;

  digitalWrite(LED_PIN, LOW); 

 
}

void loop() {
  
  for(int i=0; i < NUM_SENSORS; i++){
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

    if( ((average[i] > thresholds[i]) && lowHighTrigger[i]) || ((average[i] < thresholds[i]) && !lowHighTrigger[i]) ){

      digitalWrite(LED_PIN, HIGH); 
      if(!triggered[i]){
        Serial.print(i);
        Serial.print("\t");
        Serial.print(counter[i]);
        Serial.print("\t");
        Serial.println(average[i]);
        counter[i]++;
      }  
      triggered[i] = 1;
    } 
    else{
      triggered[i] = 0;
      digitalWrite(LED_PIN, LOW); 
    }
      
  } 

  

  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= NUM_READINGS)              
    // ...wrap around to the beginning: 
    index = 0;                           


  // send it to the computer as ASCII digits
  
  /*for(int i=0; i < 2; i++){
    Serial.print(i);
    Serial.print(" ");
    Serial.print(analogRead(inputPins[i]));
    Serial.print(" ");
    Serial.print(triggered[i]);
    Serial.print(" ");
    Serial.print(average[i]);
    Serial.print("\t");
    
  } 
 

  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.println(analogRead(A3));
  */  

  
  delay(10);        // delay in between reads for stability            
}



