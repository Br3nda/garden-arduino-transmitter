
#define voltageFlipPin1 7
#define voltageFlipPin2 5
#define sensorPin 0

int flipTimer = 1000;

void setup(){
  Serial.begin(9600);
  pinMode(voltageFlipPin1, OUTPUT);
  pinMode(voltageFlipPin2, OUTPUT);
  pinMode(sensorPin, INPUT);
       
}


void setSensorPolarity(boolean flip){
  if(flip){
    digitalWrite(voltageFlipPin1, HIGH);
    digitalWrite(voltageFlipPin2, LOW);
  }else{
    digitalWrite(voltageFlipPin1, LOW);
    digitalWrite(voltageFlipPin2, HIGH);
  }
}

void readPlantMoisture() {
  
}

void loop(){
  debugMessage("Looping");
  
  //first read
  setSensorPolarity(true);
  delay(flipTimer);
  int val1 = analogRead(sensorPin);
  delay(flipTimer);  
  
  
  //second read, opposite polarity
  setSensorPolarity(false);
  delay(flipTimer);
  // invert the reading
  int val2 = 1023 - analogRead(sensorPin);
  
  //
  reportLevels(val1,val2);
    
}


void reportLevels(int val1,int val2){
  /**
  Read moisture (voltage change) across 2 pins
  Then read in reverse.
  Alternating polatity makes the metal in the pot last longer.
  */
  debugMessage("Reading soil moisture");
  int avg = (val1 + val2) / 2;
  
  String msg = "avg: ";
  msg += avg;
  Serial.println(msg);

}

void debugMessage(String message) {
  Serial.println(message);
}

