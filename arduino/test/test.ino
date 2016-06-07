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




int cutoff = 510;

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
  pinMode(LED_PIN,OUTPUT);
    

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

  digitalWrite(motorPin, LOW); 
  allLEDOff();
  
}



void loop() {

 int value =  analogRead(LASER_1_PIN); 
 if(value < 90){
  digitalWrite(LED_PIN, HIGH);
 }else{
  digitalWrite(LED_PIN,LOW);
 }
 Serial.print(analogRead(LASER_1_PIN));
 Serial.print('\t');
 Serial.print(analogRead(LASER_2_PIN));
 Serial.print('\t');
 Serial.println(analogRead(LASER_3_PIN));
 
 delay(30);

}



