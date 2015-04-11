#include <VirtualWire.h>


#define soilPin1 7
#define soilPin2 5
#define sensorPin 0

const int plant_number = 1;

const int led_pin = 11;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;
int timeBetweenReadings = 1000;


void debugMessage(String message, int value) {
  message += value;
  Serial.println(message);
}


void setup(){

  //serial port comms, mostly for debugging
  Serial.begin(9600);

  //set up the plant sensor  
  pinMode(soilPin1, OUTPUT);
  pinMode(soilPin2, OUTPUT);
  pinMode(sensorPin, INPUT);
  
    // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec
  pinMode(led_pin, OUTPUT);
}


void setSensorPolarity(boolean flip){
  if(flip){
    digitalWrite(soilPin1, HIGH);
    digitalWrite(soilPin2, LOW);
  }
  else {
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





byte count = 1;

void sendDataViaRF(int moisture_level) {
  debugMessage("Sending RF message, moisture_level = ", moisture_level);
  
  char msg[4] = {'p', plant_number, ':', moisture_level};

  msg[6] = count;
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 7);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(1000);
  count = count + 1;
}

void loop(){
  debugMessage("Looping", 0);
  int moisture_level = readPlantMoisture();

  sendDataViaRF(moisture_level);
  
    
}
  

