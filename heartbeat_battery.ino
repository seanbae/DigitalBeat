#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h> 

#define PIN 4

int bluetoothTx = 2;  // TX-O pin of BLUESMiRF
int bluetoothRx = 3;  // RX-I pin of BLUESMiRF

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

volatile int diff1 = 0;
volatile int diff2 = 0;
volatile int diff3 = 0;
volatile int diff4 = 0;
volatile int diff5 = 0;

int BPM, BPMforDisplay;
unsigned long iterationCounter;
int animationPosition = 4;
byte oldSample, sample;
long pulsetime, lastpulsetime;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);
int prevHeartValue = 0;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
int targetRate = 100;

    int redPixels[] = {3,4,5,6,10,15,17,23,24,30,32,38,41,47,50,55,59,60,61,62};
    int blackPixels[] = {0,1,2,7,8,9,16,31,39,40,48,49,56,57,58,63};

void setup() {
  Serial.begin(9600);
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  
  Serial.println("8x8 LED Matrix Test");
  pinMode(10, INPUT);
  Serial.println("Waiting for heart beat...");

  //Wait until a heart beat is detected  
  while (!digitalRead(10)) {};
  Serial.println ("Heart beat detected!");
  matrix.begin(0x70);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  
  strip.begin();
  strip.show();
}

void loop() {

  if (animationPosition < -20){
    animationPosition = 6;
    BPMforDisplay = BPM;
  }
      sample = digitalRead(10);  //Store signal output 
  if (sample && (oldSample != sample)) {
    Serial.print("Beat: ");
    Serial.println(BPM);
    Serial.println(targetRate); 
    
    if (BPM <= (targetRate-30)){
      fillRows(1);
    }
    else if (BPM > (targetRate-30) && BPM <= (targetRate-20)) {
      fillRows(2);
    }
    else if (BPM > (targetRate-20) && BPM <= (targetRate-10)) {
      fillRows(3);
    }
    else if (BPM >= targetRate){
      fillRows(4);
    }
    
    HRpulse();
  }
  oldSample = sample;           //Store last signal received
    if (iterationCounter % 900 == 0){
    
    matrix.clear();
    matrix.setCursor(animationPosition,0);
    matrix.print(BPMforDisplay);
    matrix.writeDisplay();
    animationPosition--;
    
    for (int i=0; i<21; i++){
       strip.setPixelColor( redPixels[i], strip.Color(255, 0, 0));
    }
    
    for (int j=0; j<16; j++){
      strip.setPixelColor(blackPixels[j], strip.Color(0,0,0));
    }
         strip.setPixelColor(0, strip.Color(0,0,0));
    strip.show();
  }
  
 iterationCounter++;
 
 if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    targetRate = (int)bluetooth.read(); 
    Serial.println(targetRate); 
  }
 
}

void HRpulse()
{
  pulsetime = millis();
  rollBuffer();
  diff1 = pulsetime - lastpulsetime;
  if (diff5 != 0) {
    BPM = 60000 / ((diff1 + diff2 + diff3 + diff4 + diff5)/5);
  }
  lastpulsetime = pulsetime;
}

void rollBuffer()
{
  diff5 = diff4;
  diff4 = diff3;
  diff3 = diff2;
  diff2 = diff1;
  diff1 = 0;
}

void fillRows(int heartValue) {

  if (heartValue == 1){
    fillRow(0, strip.Color(0, 0, 255));
  }
  
  else if (heartValue == 2){
    fillRow(2, strip.Color(255, 255, 0));
  }
  
  else if (heartValue == 3){
    fillRow(5, strip.Color(255, 0, 0));
  }
  
  else if (heartValue == 4){ 
    
    rainbow(20);

  }
  
  prevHeartValue = heartValue;
}  

void fillRow(int mod, uint32_t c){
  for(uint16_t i=0; i<64; i++) { 
      if (i%8 <= mod+2){
        strip.setPixelColor(i, c);
      }
      else if (i%8 > mod+2){
        strip.setPixelColor(i,strip.Color(0, 0, 0));
      }
  }
  for(int j=0;j<16;j++){
    strip.setPixelColor(blackPixels[j],strip.Color(0, 0, 0));
  }
  for (int l=0;l<21;l++){
      strip.setPixelColor(redPixels[l], strip.Color(255, 0, 0));
    }
            strip.setPixelColor(0,strip.Color(0, 0, 0));
  strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j, k;

  for(j=0; j<256; j+=2) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    for (k=0;k<16;k++){
      strip.setPixelColor(blackPixels[k], strip.Color(0, 0, 0));
    }
            strip.setPixelColor(0,strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
}

void clearPixels(uint8_t wait){
  for (int i=0;i<64;i++){
      strip.setPixelColor(i,strip.Color(0, 0, 0));
  }
  
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
