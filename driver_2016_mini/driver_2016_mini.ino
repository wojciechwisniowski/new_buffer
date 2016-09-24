#include <Wire.h>

#define DOG_ID 5   //id na szynie I2C
#define RST_PIN 2  // reset PIN

int gi_counter = 0;
char gs_lastEvent[5];

void setupI2C() {
  Wire.begin(DOG_ID);                // join i2c bus as master
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void setup() {
  digitalWrite(RST_PIN, HIGH); 
  pinMode(RST_PIN, OUTPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  setupI2C();
}

// the loop function runs over and over again forever
void loop() {
  ledSignal(2,100); //two slow blinks 400ms delay
  gi_counter++;
  if(gi_counter >= 150){ // 60s passed without
      resetMaster();
  }
}

void resetMaster(){
    ledSignal(3,500); // three slower
    // reset signal on pin RST_PIN
    digitalWrite(RST_PIN, LOW);   
    delay(500);               
    digitalWrite(RST_PIN, HIGH); 
    gi_counter = 0;                   

}

void ledSignal(int blinks, int ms){
  for(int i=0;i<blinks; i++){ // n fast blinks
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a 50ms
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a 50ms
  }
}

//master buffer writes to dog xxxxyyyy ie 26002700 new/used
void receiveEvent(int howMany) {
  int i = 0;
  while ((1 < Wire.available()) && i < 9) { // last one is the end of transm
      gs_lastEvent[i++] = Wire.read(); // receive byte as a character
  }
  while (Wire.available())
    Wire.read();  //read all trash

  gi_counter = 0;
  ledSignal(5,50);// 5 fast blinks
  delay(100);
}

//master buff ask for status
void requestEvent() {
  char buf[21];
  snprintf(buf, sizeof(buf), "%03d", gi_counter );
  Wire.write(buf); // send temperatures in order 0,1,2,3 for 24.5 245 will be sent and then rpm-new rpm-used 21chars

//max obroty to 2700 na min
}

