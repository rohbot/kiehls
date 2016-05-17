
int inputPin = A0;

int cutoff = 510;

int led = 13;
int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;

int motorPin = 3;
int redLED = 2;
int near = 0;

int counter = 0;

unsigned long time;

unsigned long dt;

unsigned long motorTimer;

unsigned long motorStartDelay;

int reading;

int motorStatus = 0;

void setup()
{
  pinMode(motorPin,OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);     
  pinMode(led3, OUTPUT);     
  pinMode(led4, OUTPUT);     
  pinMode(led5, OUTPUT);     
  pinMode(redLED,OUTPUT);
  
  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  counter = 0; 
  time = millis();
  cutoff = 510;
  motorTimer = millis() + 5000;
  motorStartDelay =  millis() + 2000;
  digitalWrite(motorPin, LOW); 
  
}

void allLEDOff(){
  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW); 
  digitalWrite(led3, LOW); 
  digitalWrite(led4, LOW); 
  digitalWrite(led5, LOW); 

}

void motorOn(){
  digitalWrite(motorPin, HIGH);
  digitalWrite(redLED, LOW); 
  motorStatus = 1;
  //Serial.println("Motor On");
}

void motorOff(){
  digitalWrite(motorPin, LOW);
  digitalWrite(redLED, HIGH); 
  motorStatus = 0;

  //Serial.println("Motor Off");  
}


void loop() {
  
  reading = analogRead(inputPin);
  dt = millis() - time;

  // if more than one sec between reads
  if(dt > 1000){
    //turn off LEDs
    allLEDOff();
    motorStatus = 0;
  }


  if(millis() - motorTimer > 5000){
    motorOff();
  }


  if(reading < cutoff){
      
    if(!near && dt > 100){
//      digitalWrite(led, HIGH); 
      

      Serial.print(counter);
      Serial.print("\t");
      Serial.print(dt);
      Serial.print("\t");
      Serial.println(motorStatus);
      counter++;

      if(dt < 400){
          digitalWrite(led5, HIGH); 
          motorTimer = millis();

          if(motorStatus == 0){
            motorStartDelay = millis() + 1000;
            motorStatus = 1;
          }


          if(millis() > motorStartDelay){
            motorOn();
          }  
          
        }else{
          digitalWrite(led5, LOW);
          motorStatus = 0; 

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
        if(dt < 520){
          digitalWrite(led2, HIGH); 
  
        }else{
          digitalWrite(led2, LOW); 

        }
        if(dt < 550){
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

  } 
  // send it to the computer as ASCII digits
  if(reading < 510){
    //Serial.println(reading);  
  }
     
  //delay(10);        // delay in between reads for stability            
}


