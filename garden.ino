
#define soilPin1 7
#define soilPin2 5
#define sensorPin 0

int timeBetweenReadings = 1000;

void setup(){
  Serial.begin(9600);
  pinMode(soilPin1, OUTPUT);
  pinMode(soilPin2, OUTPUT);
  pinMode(sensorPin, INPUT);
       
}


void setSensorPolarity(boolean flip){
  if(flip){
    digitalWrite(soilPin1, HIGH);
    digitalWrite(soilPin2, LOW);
  }
  else{
    digitalWrite(soilPin1, LOW);
    digitalWrite(soilPin2, HIGH);
  }
}

int readPlantMoisture() {
  /**
  Read moisture (voltage change) across 2 pins
  Then read in reverse.
  Alternating polatity makes the metal in the pot last longer.
  */
  
  //first read
  setSensorPolarity(true);
  delay(timeBetweenReadings);
  int val1 = analogRead(sensorPin);
  delay(timeBetweenReadings);  
  
  
  //second read, opposite polarity
  setSensorPolarity(false);
  delay(timeBetweenReadings);
  // invert the reading
  int val2 = 1023 - analogRead(sensorPin);
  
  //average the two values
  debugMessage("Reading one = ",val1);
  debugMessage("Reading two = ", val2);
  
  int avg = (val1 + val2) / 2;
  debugMessage("Average reading = ", avg);
  
  return avg;
}

void loop(){
  debugMessage("Looping", 0);
  int moisture_level = readPlantMoisture();

    
}
  
void debugMessage(String message, int value) {
  message += value;
  Serial.println(message);
}

