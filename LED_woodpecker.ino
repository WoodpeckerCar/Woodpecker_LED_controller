// Woodpecker  http://woodpecker.mobi   

// PIN Numbers
//
// PIN 2 External Relay
// PIN 3 LED strip Front Left
// PIN 4 LED strip Right
// PIN 5 LED strip Left
// PIN 6 LED strip Rear Right
// PIN 7 External Relay
// PIN 8 External Relay
// PIN 9 CAN
// PIN 10 External Relay
// PIN 12 LED strip Rear Left
// PIN 13 Sound Buzzer
//
//
//
//

#include <Adafruit_NeoPixel.h>
#include <mcp_can.h>
#include <SPI.h>

#define NUM_LEDS1 72
#define NUM_LEDS2 72
#define NUM_LEDS3 144
#define NUM_LEDS4 144
#define NUM_LEDS5 72
#define NUM_LEDS6 72

#define PIN1 11  // Front Right
#define PIN2 3  // Front Left
#define PIN3 4  // Right
#define PIN4 5  // Left
#define PIN5 6  // Rear Right
#define PIN6 12  // Rear Left
int piezoPin = 13; //Sound buzzer
byte relayPin[4] = {    //Relay module pins
  2,7,8,10};
const int SPI_CS_PIN = 9; //CANbus module 

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
const int ledHIGH    = 1;
const int ledLOW     = 0;
unsigned char stmp[8] = {ledHIGH, 1, 2, 3, ledLOW, 5, 6, 7};


#define NUMPIXELS1 72
#define NUMPIXELS2 72
#define NUMPIXELS3 144
#define NUMPIXELS4 144
#define NUMPIXELS5 72
#define NUMPIXELS6 72

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUMPIXELS3, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUMPIXELS4, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUMPIXELS5, PIN5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUMPIXELS6, PIN6, NEO_GRB + NEO_KHZ800);

uint32_t white = strip1.Color(255, 255, 255);
uint32_t blue = strip1.Color(0, 0, 255);
uint32_t red = strip1.Color(255, 0, 0);
uint32_t orange = strip1.Color(255, 165, 0);
uint32_t yellow = strip1.Color(255, 255, 0);
uint32_t nocolor = strip1.Color(0, 0, 0);
int orangeHUE = 6000;
int yellowHUE = 9000;

// strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));

int count = 1;  //For initialization show run
#define DELAYVAL 1 // Time (in milliseconds) to pause between pixels

#define INT32U unsigned long int
INT32U canId = 0x101;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
String command;
String CanMessage="";

int LeftTurnState=0;
int RightTurnState=0;
int HazardState=0;
int LowBeamState=0;
int HighBeamState=0;
int BrakeLightState=0;
int DRLState=0;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  for(int i = 0; i < 4; i++)  pinMode(relayPin[i],OUTPUT);
  for(int j = 0; j < 4; j++)  digitalWrite(relayPin[j],LOW);
  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();
  strip3.begin();
  strip3.show();
  strip4.begin();
  strip4.show();
  strip5.begin();
  strip5.show();
  strip6.begin();
  strip6.show();
  ClearAllLeds();
  demorun();      
 // while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
 //   {
 //      Serial.println("CAN BUS Shield init fail");
 //       Serial.println(" Init CAN BUS Shield again");
 //       delay(100);
 //   }
 //  Serial.println("CAN BUS Shield init ok!");
}

                              void loop() {
     demorun(); 
                              }
   

// ***************************************
// Clear Lights ///
// ***************************************

void ClearLights1() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip1.setPixelColor(i, 0);    //turn every pixel off
  }
  strip1.show();
}

void ClearLights2() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip2.setPixelColor(i, 0);    //turn every pixel off
  }
  strip2.show();
}

void ClearLights3() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip3.setPixelColor(i, 0);    //turn every pixel off
  }
  strip3.show();
}

void ClearLights4() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip4.setPixelColor(i, 0);    //turn every pixel off
  }
  strip4.show();
}

void ClearLights5() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip5.setPixelColor(i, 0);    //turn every pixel off
  }
  strip5.show();
}

void ClearLights6() {
  for (int i = 0; i < strip1.numPixels(); i = i + 1) {
    strip6.setPixelColor(i, 0);    //turn every pixel off
  }
  strip6.show();
}

// ***************************************
// Clear Lights End
// ***************************************


void BlinkOuter(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { // The j<# determines how many cycles
    strip1.setPixelColor(strip1.numPixels() - 1, c);
    strip1.setPixelColor(0, c);
    strip1.show();
    delay(wait);
    strip1.setPixelColor(strip1.numPixels() - 1, 0);
    strip1.setPixelColor(0, 0);
    strip1.show();
    delay(wait);
  }
}

void OnOuter(uint32_t c) {
  strip1.setPixelColor(strip1.numPixels() - 1, c);
  strip1.setPixelColor(0, c);
  strip1.show();
  delay(3000);
}


// ***************************************
// ** STROBE **
// ***************************************

void StrobeRight(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll1(red,green,blue);
    setAll3(red,green,blue);
    setAll5(red,green,blue);
    showStrip1();
    showStrip3();
    showStrip5();
    delay(FlashDelay);
    setAll1(0,0,0);
    showStrip1();
    setAll3(0,0,0);
    showStrip3();
    setAll5(0,0,0);
    showStrip5();
    delay(FlashDelay);
  }
 delay(EndPause);
}

void StrobeLeft(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll2(red,green,blue);
    setAll4(red,green,blue);
    setAll6(red,green,blue);
    showStrip2();
    showStrip4();
    showStrip6();
    delay(FlashDelay);
    setAll2(0,0,0);
    showStrip2();
    setAll4(0,0,0);
    showStrip4();
    setAll6(0,0,0);
    showStrip6();
    delay(FlashDelay);
  }
 delay(EndPause);
}

void Strobe1(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll1(red,green,blue);
    showStrip1();
    delay(FlashDelay);
    setAll1(0,0,0);
    showStrip1();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void Strobe2(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll2(red,green,blue);
    showStrip2();
    delay(FlashDelay);
    setAll2(0,0,0);
    showStrip2();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void Strobe3(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll3(red,green,blue);
    showStrip3();
    delay(FlashDelay);
    setAll3(0,0,0);
    showStrip3();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void Strobe4(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll4(red,green,blue);
    showStrip4();
    delay(FlashDelay);
    setAll4(0,0,0);
    showStrip4();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void Strobe5(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll5(red,green,blue);
    showStrip5();
    delay(FlashDelay);
    setAll5(0,0,0);
    showStrip5();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void Strobe6(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll6(red,green,blue);
    showStrip6();
    delay(FlashDelay);
    setAll6(0,0,0);
    showStrip6();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

// ***************************************
// ** NeoPixel Common Functions **
// ***************************************

// ***************************************
// ** LED 1**
// ***************************************

// Apply LED color changes
void showStrip1() {
   strip1.show();
}

// Set a LED color (not yet visible)
void setPixel1(int Pixel, byte red, byte green, byte blue) {
   strip1.setPixelColor(Pixel, strip1.Color(red, green, blue));
 }

// Set all LEDs to a given color and apply it (visible)
void setAll1(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS1; i++ ) {
    setPixel1(i, red, green, blue);
  }
  showStrip1();
}

// ***************************************
// ** LED 2 **
// ***************************************

// Apply LED color changes
void showStrip2() {
   strip2.show();
}

// Set a LED color (not yet visible)
void setPixel2(int Pixel, byte red, byte green, byte blue) {
   strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));
}

// Set all LEDs to a given color and apply it (visible)
void setAll2(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS2; i++ ) {
    setPixel2(i, red, green, blue);
  }
  showStrip2();
}

// ***************************************
// ** LED 3 **
// ***************************************

// Apply LED color changes
void showStrip3() {
   strip3.show();
}

// Set a LED color (not yet visible)
void setPixel3(int Pixel, byte red, byte green, byte blue) {
   strip3.setPixelColor(Pixel, strip3.Color(red, green, blue));
 }

// Set all LEDs to a given color and apply it (visible)
void setAll3(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS3; i++ ) {
    setPixel3(i, red, green, blue);
  }
  showStrip3();
}

// ***************************************
// ** LED 4 **
// ***************************************

// Apply LED color changes
void showStrip4() {
   strip4.show();
}

// Set a LED color (not yet visible)
void setPixel4(int Pixel, byte red, byte green, byte blue) {
   strip4.setPixelColor(Pixel, strip4.Color(red, green, blue));
}

// Set all LEDs to a given color and apply it (visible)
void setAll4(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS4; i++ ) {
    setPixel4(i, red, green, blue);
  }
  showStrip4();
}

// ***************************************
// ** LED 5 **
// ***************************************

// Apply LED color changes
void showStrip5() {
   strip5.show();
}

// Set a LED color (not yet visible)
void setPixel5(int Pixel, byte red, byte green, byte blue) {
   strip5.setPixelColor(Pixel, strip5.Color(red, green, blue));
}

// Set all LEDs to a given color and apply it (visible)
void setAll5(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS5; i++ ) {
    setPixel5(i, red, green, blue);
  }
  showStrip5();
}

// ***************************************
// ** LED 6 **
// ***************************************

// Apply LED color changes
void showStrip6() {
   strip6.show();
}

// Set a LED color (not yet visible)
void setPixel6(int Pixel, byte red, byte green, byte blue) {
   strip6.setPixelColor(Pixel, strip6.Color(red, green, blue));
}

// Set all LEDs to a given color and apply it (visible)
void setAll6(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS6; i++ ) {
    setPixel6(i, red, green, blue);
  }
  showStrip6();
}



void turnsignalright() {
  while(count > 0 ) {
  strip1.clear(); // Set all pixel colors to 'off'
  setAll3(0,0,0);
  strip5.clear(); // Set all pixel colors to 'off'
  delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip1.setPixelColor(i, orange);
 //               strip1.setPixelColor(i, strip1.gamma32(strip1.ColorHSV(yellowHUE)));
                strip5.setPixelColor(i, orange);
 //
 strip5.setPixelColor(i, strip5.gamma32(strip5.ColorHSV(yellowHUE)));
                strip1.setBrightness(150);
                strip5.setBrightness(150);
                strip1.show();   // Send the updated pixel colors to the hardware.
                strip5.show();   // Send the updated pixel colors to the hardware.
                delay(DELAYVAL); // Pause before next pass through loo       
             }
             strip1.setBrightness(255);
             strip5.setBrightness(255);
             strip1.fill(yellow, 0, NUMPIXELS1);
             strip5.fill(yellow, 0, NUMPIXELS1);
                      strip3.setPixelColor(0, 255, 255, 0);
        strip3.setPixelColor(1, 255, 255, 0);
        strip3.setPixelColor(2, 255, 255, 0);
        strip3.setPixelColor(3, 255, 255, 0);
        strip3.setPixelColor(4, 255, 255, 0);
        strip3.setPixelColor(5, 255, 255, 0);
        strip3.setPixelColor(6, 255, 255, 0);
        strip3.setPixelColor(7, 255, 255, 0);
        strip3.setPixelColor(136, 255, 255, 0);
        strip3.setPixelColor(137, 255, 255, 0);
        strip3.setPixelColor(138, 255, 255, 0);
        strip3.setPixelColor(139, 255, 255, 0);
        strip3.setPixelColor(140, 255, 255, 0);
        strip3.setPixelColor(141, 255, 255, 0);
        strip3.setPixelColor(142, 255, 255, 0);
        strip3.setPixelColor(143, 255, 255, 0); 
           strip3.show();
    strip1.show();  
    strip5.show(); 
    headlightturnON();
      delay(1000);       
    count = count -1;
    strip1.fill(nocolor, 0, NUMPIXELS1);
    strip3.fill(nocolor, 0, NUMPIXELS1);
    strip5.fill(nocolor, 0, NUMPIXELS1);
    strip1.show();
    strip3.show(); 
    strip5.show(); 
    headlightturnOFF();
}
}

void turnsignalleft() {
  while(count > 0 ) {
  strip2.clear(); // Set all pixel colors to 'off'
  setAll4(0,0,0);
  strip6.clear(); // Set all pixel colors to 'off'
delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip2.setPixelColor(i, orange);
                strip6.setPixelColor(i, orange);
                strip2.setBrightness(150);
                strip6.setBrightness(150);
                strip2.show();   // Send the updated pixel colors to the hardware.
                strip6.show();   // Send the updated pixel colors to the hardware.
                delay(DELAYVAL); // Pause before next pass through loop 
             }
             strip2.setBrightness(255);
             strip6.setBrightness(255);
             strip2.fill(yellow, 0, NUMPIXELS1);
             strip6.fill(yellow, 0, NUMPIXELS1);
        strip4.setPixelColor(0, 255, 165, 0);
        strip4.setPixelColor(1, 255, 255, 0);
        strip4.setPixelColor(2, 255, 255, 0);
        strip4.setPixelColor(3, 255, 255, 0);
        strip4.setPixelColor(4, 255, 255, 0);
        strip4.setPixelColor(5, 255, 255, 0);
        strip4.setPixelColor(6, 255, 255, 0);
        strip4.setPixelColor(7, 255, 255, 0);
        strip4.setPixelColor(136, 255, 255, 0);
        strip4.setPixelColor(137, 255, 255, 0);
        strip4.setPixelColor(138, 255, 255, 0);
        strip4.setPixelColor(139, 255, 255, 0);
        strip4.setPixelColor(140, 255, 255, 0);
        strip4.setPixelColor(141, 255, 255, 0);
        strip4.setPixelColor(142, 255, 255, 0);
        strip4.setPixelColor(143, 255, 255, 0);    
        strip4.show();
    strip2.show(); 
    strip6.show();
    headlightturnON(); 
      delay(1000);       
    count = count -1;
    strip2.fill(nocolor, 0, NUMPIXELS1);
    strip4.fill(nocolor, 0, NUMPIXELS1);
    strip6.fill(nocolor, 0, NUMPIXELS1);
    strip2.show(); 
    strip4.show(); 
    strip6.show(); 
    headlightturnOFF();
}
}

void turnsignalhazard() {
  while(count > 0 ) {
  strip1.clear(); // Set all pixel colors to 'off'
  strip2.clear(); // Set all pixel colors to 'off'  
  setAll3(0,0,0);
  setAll4(0,0,0);
  strip5.clear(); // Set all pixel colors to 'off'
  strip6.clear(); // Set all pixel colors to 'off'
delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip1.setPixelColor(i, orange);
                strip2.setPixelColor(i, orange);
                strip5.setPixelColor(i, orange);
                strip6.setPixelColor(i, orange);               
                strip1.setBrightness(150);
                strip2.setBrightness(150);
                strip5.setBrightness(150);
                strip6.setBrightness(150);               
                strip1.show();   // Send the updated pixel colors to the hardware.
                strip2.show();   // Send the updated pixel colors to the hardware.
                strip5.show();   // Send the updated pixel colors to the hardware.
                strip6.show();   // Send the updated pi   
        strip3.setPixelColor(0, 255, 255, 0);
        strip3.setPixelColor(1, 255, 255, 0);
        strip3.setPixelColor(2, 255, 255, 0);
        strip3.setPixelColor(3, 255, 255, 0);
        strip3.setPixelColor(4, 255, 255, 0);
        strip3.setPixelColor(5, 255, 255, 0);
        strip3.setPixelColor(6, 255, 255, 0);
        strip3.setPixelColor(7, 255, 255, 0);
        strip3.setPixelColor(136, 255, 255, 0);
        strip3.setPixelColor(137, 255, 255, 0);
        strip3.setPixelColor(138, 255, 255, 0);
        strip3.setPixelColor(139, 255, 255, 0);
        strip3.setPixelColor(140, 255, 255, 0);
        strip3.setPixelColor(141, 255, 255, 0);
        strip3.setPixelColor(142, 255, 255, 0);
        strip3.setPixelColor(143, 255, 255, 0);
        strip4.setPixelColor(1, 255, 255, 0);
        strip4.setPixelColor(2, 255, 255, 0);
        strip4.setPixelColor(3, 255, 255, 0);
        strip4.setPixelColor(4, 255, 255, 0);
        strip4.setPixelColor(5, 255, 255, 0);
        strip4.setPixelColor(6, 255, 255, 0);
        strip4.setPixelColor(7, 255, 255, 0);
        strip4.setPixelColor(136, 255, 255, 0);
        strip4.setPixelColor(137, 255, 255, 0);
        strip4.setPixelColor(138, 255, 255, 0);
        strip4.setPixelColor(139, 255, 255, 0);
        strip4.setPixelColor(140, 255, 255, 0);
        strip4.setPixelColor(141, 255, 255, 0);
        strip4.setPixelColor(142, 255, 255, 0);
        strip4.setPixelColor(143, 255, 255, 0);   
        strip3.show(); 
        strip4.show();                                
    //            delay(DELAYVAL); // Pause before next pass through loop
             }
             strip1.setBrightness(255);
             strip2.setBrightness(255);
             strip5.setBrightness(255);
             strip6.setBrightness(255);
             strip1.fill(yellow, 0, NUMPIXELS1);
             strip2.fill(yellow, 0, NUMPIXELS1);
             strip5.fill(yellow, 0, NUMPIXELS1);
             strip6.fill(yellow, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.show();   
    strip5.show(); 
    strip6.show();
    headlightturnON();  
      delay(1000);       
    count = count -1;
    strip1.fill(nocolor, 0, NUMPIXELS1);
    strip2.fill(nocolor, 0, NUMPIXELS1);
    strip5.fill(nocolor, 0, NUMPIXELS1);
    strip6.fill(nocolor, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.show(); 
    strip5.show(); 
    strip6.show(); 
    headlightturnOFF();
}
}

void runninglightsfront() {
    headlightDRLON();
  ClearAllLeds ();
    strip1.setBrightness(50);
    strip1.fill(white, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.setBrightness(50);
    strip2.fill(white, 0, NUMPIXELS1);
    strip2.show(); 
    strip5.setBrightness(50);
    strip5.fill(red, 0, NUMPIXELS1);
    strip5.show(); 
    strip6.setBrightness(50);
    strip6.fill(red, 0, NUMPIXELS1);
    strip6.show(); 
        strip3.setBrightness(50);
        strip4.setBrightness(50);
        strip3.setPixelColor(0, 255, 165, 0);
        strip3.setPixelColor(1, 255, 165, 0);
        strip3.setPixelColor(2, 255, 165, 0);
        strip3.setPixelColor(3, 255, 165, 0);
        strip3.setPixelColor(4, 255, 165, 0);
        strip3.setPixelColor(5, 255, 165, 0);
        strip3.setPixelColor(6, 255, 165, 0);
        strip3.setPixelColor(7, 255, 165, 0);
        strip3.setPixelColor(136, 255, 165, 0);
        strip3.setPixelColor(137, 255, 165, 0);
        strip3.setPixelColor(138, 255, 165, 0);
        strip3.setPixelColor(139, 255, 165, 0);
        strip3.setPixelColor(140, 255, 165, 0);
        strip3.setPixelColor(141, 255, 165, 0);
        strip3.setPixelColor(142, 255, 165, 0);
        strip3.setPixelColor(143, 255, 165, 0);
        strip4.setPixelColor(0, 255, 165, 0);
        strip4.setPixelColor(1, 255, 165, 0);
        strip4.setPixelColor(2, 255, 165, 0);
        strip4.setPixelColor(3, 255, 165, 0);
        strip4.setPixelColor(4, 255, 165, 0);
        strip4.setPixelColor(5, 255, 165, 0);
        strip4.setPixelColor(6, 255, 165, 0);
        strip4.setPixelColor(7, 255, 165, 0);
        strip4.setPixelColor(136, 255, 165, 0);
        strip4.setPixelColor(137, 255, 165, 0);
        strip4.setPixelColor(138, 255, 165, 0);
        strip4.setPixelColor(139, 255, 165, 0);
        strip4.setPixelColor(140, 255, 165, 0);
        strip4.setPixelColor(141, 255, 165, 0);
        strip4.setPixelColor(142, 255, 165, 0);
        strip4.setPixelColor(143, 255, 165, 0);
        strip3.show(); 
        strip4.show();
}

void runninglightsraer() {
  ClearAllLeds ();
    strip1.setBrightness(50);
    strip1.fill(red, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.setBrightness(50);
    strip2.fill(red, 0, NUMPIXELS1);
    strip2.show(); 
    strip5.setBrightness(50);
    strip5.fill(white, 0, NUMPIXELS1);
    strip5.show(); 
    strip6.setBrightness(50);
    strip6.fill(white, 0, NUMPIXELS1);
    strip6.show(); 
        strip3.setBrightness(50);
        strip4.setBrightness(50);
        strip3.setPixelColor(0, 255, 165, 0);
        strip3.setPixelColor(1, 255, 165, 0);
        strip3.setPixelColor(2, 255, 165, 0);
        strip3.setPixelColor(3, 255, 165, 0);
        strip3.setPixelColor(4, 255, 165, 0);
        strip3.setPixelColor(5, 255, 165, 0);
        strip3.setPixelColor(6, 255, 165, 0);
        strip3.setPixelColor(7, 255, 165, 0);
        strip3.setPixelColor(136, 255, 165, 0);
        strip3.setPixelColor(137, 255, 165, 0);
        strip3.setPixelColor(138, 255, 165, 0);
        strip3.setPixelColor(139, 255, 165, 0);
        strip3.setPixelColor(140, 255, 165, 0);
        strip3.setPixelColor(141, 255, 165, 0);
        strip3.setPixelColor(142, 255, 165, 0);
        strip3.setPixelColor(143, 255, 165, 0);
        strip4.setPixelColor(0, 255, 165, 0);
        strip4.setPixelColor(1, 255, 165, 0);
        strip4.setPixelColor(2, 255, 165, 0);
        strip4.setPixelColor(3, 255, 165, 0);
        strip4.setPixelColor(4, 255, 165, 0);
        strip4.setPixelColor(5, 255, 165, 0);
        strip4.setPixelColor(6, 255, 165, 0);
        strip4.setPixelColor(7, 255, 165, 0);
        strip4.setPixelColor(136, 255, 165, 0);
        strip4.setPixelColor(137, 255, 165, 0);
        strip4.setPixelColor(138, 255, 165, 0);
        strip4.setPixelColor(139, 255, 165, 0);
        strip4.setPixelColor(140, 255, 165, 0);
        strip4.setPixelColor(141, 255, 165, 0);
        strip4.setPixelColor(142, 255, 165, 0);
        strip4.setPixelColor(143, 255, 165, 0);
        strip3.show(); 
        strip4.show();
}

void brakesrear() {
    strip5.setBrightness(255);
    strip5.fill(red, 0, NUMPIXELS1);
    strip5.show(); 
    strip6.setBrightness(255);
    strip6.fill(red, 0, NUMPIXELS1);
    strip6.show();
//    digitalWrite(relayPin[0],HIGH);     
//    delay(3000);       
//    strip5.fill(nocolor, 0, NUMPIXELS1);
//    strip6.fill(nocolor, 0, NUMPIXELS1);
//    strip5.show(); 
//    strip6.show();
//    digitalWrite(relayPin[0],LOW);
     
}

void brakesrearOFF() {
    strip5.setBrightness(255);
    strip5.fill(nocolor, 0, NUMPIXELS1);
    strip6.fill(nocolor, 0, NUMPIXELS1);
    strip5.show(); 
    strip6.show();
//    digitalWrite(relayPin[0],LOW);
     
}

void brakesfront() {
    strip1.setBrightness(255);
    strip1.fill(red, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.setBrightness(255);
    strip2.fill(red, 0, NUMPIXELS1);
    strip2.show(); 
//    digitalWrite(relayPin[0],HIGH);   
    delay(3000);       
    strip1.fill(nocolor, 0, NUMPIXELS1);
    strip2.fill(nocolor, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.show();
//    digitalWrite(relayPin[0],LOW);
}

                          void demorun() {
                           //   delay(20000);
                              initsound();
                              Initialization(0, 0, 250, 8, 1000, 0); //red, green, blue, EyeSize, SpeedDelay, ReturnDelay
                              delay(3000);
                              runninglightsfront();
                              delay(3000);
                              lowbeamON();
                              delay(3000);
                              lowbeamOFF();
                              highbeamON();
                              delay(3000);
                              highbeamOFF();
                              delay(3000);
                              headlightDRLOFF();
                              runninglightsraer();
                              delay(3000);
                              count = 5;
                              turnsignalleft();
                              runninglightsraer();
                              delay(2000);
                              count = 5;
                              turnsignalright();
                              runninglightsraer();
                              delay(2000);
                              count = 5;
                              turnsignalhazard();
                              runninglightsraer();
                              delay(3000);
                              brakesfront();
                              runninglightsraer();
                              delay(3000);
                              dangerall();
                              runninglightsraer();
                              delay(3000);
                              strobewhite();
                              runninglightsraer();
                              delay(3000);
                              strobepolice();
                              runninglightsraer();
                              delay(3000);
                              ClearAllLeds ();
                          }

void FadeIn1(byte red, byte green, byte blue){
  float r, g, b;
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll1(r,g,b);
    delay(1);
    showStrip1();
  }
}

void FadeOut1(byte red, byte green, byte blue){
  float r, g, b;
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll1(r,g,b);
    delay(1);
    showStrip1();
  }
}

 void ClearAllLeds (){
            setAll1(0,0,0);
            setAll2(0,0,0);
            setAll3(0,0,0);
            setAll4(0,0,0);
            setAll5(0,0,0);
            setAll6(0,0,0);  
 }
  void ClearAllLedsandRelays (){
            setAll1(0,0,0);
            setAll2(0,0,0);
            setAll3(0,0,0);
            setAll4(0,0,0);
            setAll5(0,0,0);
            setAll6(0,0,0);  
            digitalWrite(relayPin[0],LOW);
            digitalWrite(relayPin[1],LOW);
            digitalWrite(relayPin[2],LOW);
            digitalWrite(relayPin[3],LOW);
 }

void Danger (byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll1(red,green,blue);
    setAll2(red,green,blue);
    setAll5(red,green,blue);
    setAll6(red,green,blue);
        strip3.setPixelColor(0, red,green,blue);
        strip3.setPixelColor(1, red,green,blue);
        strip3.setPixelColor(2, red,green,blue);
        strip3.setPixelColor(3, red,green,blue);
        strip3.setPixelColor(4, red,green,blue);
        strip3.setPixelColor(5, red,green,blue);
        strip3.setPixelColor(6, red,green,blue);
        strip3.setPixelColor(7, red,green,blue);
        strip3.setPixelColor(136, red,green,blue);
        strip3.setPixelColor(137, red,green,blue);
        strip3.setPixelColor(138, red,green,blue);
        strip3.setPixelColor(139, red,green,blue);
        strip3.setPixelColor(140, red,green,blue);
        strip3.setPixelColor(141, red,green,blue);
        strip3.setPixelColor(142, red,green,blue);
        strip3.setPixelColor(143, red,green,blue);
        strip4.setPixelColor(0, red,green,blue);
        strip4.setPixelColor(1, red,green,blue);
        strip4.setPixelColor(2, red,green,blue);
        strip4.setPixelColor(3, red,green,blue);
        strip4.setPixelColor(4, red,green,blue);
        strip4.setPixelColor(5, red,green,blue);
        strip4.setPixelColor(6, red,green,blue);
        strip4.setPixelColor(7, red,green,blue);
        strip4.setPixelColor(136, red,green,blue);
        strip4.setPixelColor(137, red,green,blue);
        strip4.setPixelColor(138, red,green,blue);
        strip4.setPixelColor(139, red,green,blue);       
        strip4.setPixelColor(140, red,green,blue);
        strip4.setPixelColor(141, red,green,blue);
        strip4.setPixelColor(142, red,green,blue);
        strip4.setPixelColor(143, red,green,blue);
        strip3.show(); 
        strip4.show();
    showStrip1();
    showStrip2();
    showStrip5();
    showStrip6();
    delay(FlashDelay);
    setAll1(0,0,0);
    showStrip1();
    setAll2(0,0,0);
    showStrip2();
    setAll3(0,0,0);
    showStrip3();
    setAll4(0,0,0);
    showStrip4();
    setAll5(0,0,0);
    showStrip5();
    setAll6(0,0,0);
    showStrip6();
    delay(FlashDelay);
  }
 delay(EndPause);
}

void dangerall(){
          Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
        Danger(255, 255, 0, 4, 40, 100);
        delay(500);
  }
void strobewhite(){
                  StrobeLeft(255, 255, 255, 5, 40, 100); //White
                StrobeRight(255, 255, 255, 5, 40, 100); //White
                StrobeLeft(255, 255, 255, 5, 40, 100); //White
                StrobeRight(255, 255, 255, 5, 40, 100); //White
                StrobeLeft(255, 255, 255, 5, 40, 100); //White
                StrobeRight(255, 255, 255, 5, 40, 100); //White
                StrobeLeft(255, 255, 255, 5, 40, 100); //White
                StrobeRight(255, 255, 255, 5, 40, 100); //White
}
void strobepolice(){
                StrobeLeft(0, 0, 255, 5, 40, 100); //Blue
                StrobeRight(0, 0, 255, 5, 40, 100); //Blue
                StrobeLeft(0, 0, 255, 5, 40, 100); //Blue
                StrobeRight(0, 0, 255, 5, 40, 100); //Blue
                StrobeLeft(0, 0, 255, 5, 40, 100); //Blue
                StrobeRight(0, 0, 255, 5, 40, 100); //Blue
                StrobeLeft(0, 0, 255, 5, 40, 100); //Blue
                StrobeRight(0, 0, 255, 5, 40, 100); //Blue
}

void Initialization(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
while(count > 0 ) {
    setAll1(0,0,0);
    setAll2(0,0,0);
    setAll3(0,0,0);
    setAll4(0,0,0);
    setAll5(0,0,0);
    setAll6(0,0,0);
    strip1.setBrightness(255);
    strip2.setBrightness(255);
    strip3.setBrightness(255);
    strip4.setBrightness(255);
    strip5.setBrightness(255);
    strip6.setBrightness(255);
                            for(int i = 0; i < NUMPIXELS1-EyeSize-2; i++) {   
                              setPixel1(i, red/10, green/10, blue/10);       
                              setPixel2(i, red/10, green/10, blue/10);      
                              setPixel3(72-i, red/10, green/10, blue/10);
                              setPixel4(72-i, red/10, green/10, blue/10);
                              setPixel3(i+72, red/10, green/10, blue/10);
                              setPixel4(i+72, red/10, green/10, blue/10);
                              setPixel5(i, red/10, green/10, blue/10);
                              setPixel6(i, red/10, green/10, blue/10);
                                            for(int j = 1; j <= EyeSize; j++) {   
                                              setPixel1(i+j, red, green, blue);
                                              setPixel2(i+j, red, green, blue); 
                                              setPixel3(72-i+j, red, green, blue);
                                              setPixel4(72-i+j, red, green, blue);
                                              setPixel3(i+j+72, red, green, blue);
                                              setPixel4(i+j+72, red, green, blue);
                                              setPixel5(i+j, red, green, blue);
                                              setPixel6(i+j, red, green, blue);
                                            }
                              setPixel1(i+EyeSize+1, red/10, green/10, blue/10);  
                              setPixel2(i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel3(72-i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel4(72-i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel3(72+i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel4(72+i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel5(i+EyeSize+1, red/10, green/10, blue/10);
                              setPixel6(i+EyeSize+1, red/10, green/10, blue/10);
                              showStrip1();
                              showStrip2();
                              showStrip3();
                              showStrip4();
                              showStrip5();
                              showStrip6();
                              setAll1(0,0,0);
                              setAll2(0,0,0);
                              setAll3(0,0,0);
                              setAll4(0,0,0);
                              setAll5(0,0,0);
                              setAll6(0,0,0);
                                
                            }
          setAll1(0,0,250);
          setAll2(0,0,250);
          setAll3(0,0,250);
          setAll4(0,0,250);
          setAll5(0,0,250);
          setAll6(0,0,250);
          strip1.setBrightness(60);
          strip2.setBrightness(60);
          strip3.setBrightness(60);
          strip4.setBrightness(60);
          strip5.setBrightness(60);
          strip6.setBrightness(60);
              delay(SpeedDelay);
          count--;
//          Serial.print("count=");
//          Serial.println(count);
}
}
void initsound(){
          tone(piezoPin, 1000, 500);
          delay(500);
          tone(piezoPin, 2000, 500);
          delay(500);
          tone(piezoPin, 1000, 200);
             Serial.println("sound is ON");
  }

  void backupsound(){
          tone(piezoPin, 2000, 1000);
          delay(2000);
          tone(piezoPin, 2000, 1000);
          delay(2000);
          tone(piezoPin, 2000, 1000);
          delay(2000);
          tone(piezoPin, 2000, 1000);
          delay(2000);
          tone(piezoPin, 2000, 1000);
          delay(2000);
          tone(piezoPin, 2000, 1000);
          delay(2000);
  }

void lowbeamON() {
   digitalWrite(relayPin[0],HIGH);

}

void lowbeamOFF() {
   digitalWrite(relayPin[0],LOW);

}

void highbeamON() {
   digitalWrite(relayPin[1],HIGH);

}

void highbeamOFF() {
   digitalWrite(relayPin[1],LOW);

}

void headlightDRLON() {
   digitalWrite(relayPin[2],HIGH);

}

void headlightDRLOFF() {
   digitalWrite(relayPin[2],LOW);

}

void headlightturnON() {
   digitalWrite(relayPin[3],HIGH);

}

void headlightturnOFF() {
   digitalWrite(relayPin[3],LOW);

}

void turnsignalleftindef() {
  strip2.clear(); // Set all pixel colors to 'off'
  setAll4(0,0,0);
  strip6.clear(); // Set all pixel colors to 'off'
delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip2.setPixelColor(i, orange);
                strip6.setPixelColor(i, orange);
                strip2.setBrightness(150);
                strip6.setBrightness(150);
                strip2.show();   // Send the updated pixel colors to the hardware.
                strip6.show();   // Send the updated pixel colors to the hardware.
                delay(DELAYVAL); // Pause before next pass through loop 
             }
             strip2.setBrightness(255);
             strip6.setBrightness(255);
             strip2.fill(yellow, 0, NUMPIXELS1);
             strip6.fill(yellow, 0, NUMPIXELS1);
        strip4.setPixelColor(0, 255, 165, 0);
        strip4.setPixelColor(1, 255, 255, 0);
        strip4.setPixelColor(2, 255, 255, 0);
        strip4.setPixelColor(3, 255, 255, 0);
        strip4.setPixelColor(4, 255, 255, 0);
        strip4.setPixelColor(5, 255, 255, 0);
        strip4.setPixelColor(6, 255, 255, 0);
        strip4.setPixelColor(7, 255, 255, 0);
        strip4.setPixelColor(136, 255, 255, 0);
        strip4.setPixelColor(137, 255, 255, 0);
        strip4.setPixelColor(138, 255, 255, 0);
        strip4.setPixelColor(139, 255, 255, 0);
        strip4.setPixelColor(140, 255, 255, 0);
        strip4.setPixelColor(141, 255, 255, 0);
        strip4.setPixelColor(142, 255, 255, 0);
        strip4.setPixelColor(143, 255, 255, 0);    
        strip4.show();
    strip2.show(); 
    strip6.show();
    headlightturnON(); 
      delay(1000);       
    strip2.fill(nocolor, 0, NUMPIXELS1);
    strip4.fill(nocolor, 0, NUMPIXELS1);
    strip6.fill(nocolor, 0, NUMPIXELS1);
    strip2.show(); 
    strip4.show(); 
    strip6.show(); 
    headlightturnOFF();
}

void turnsignalrightindef() {
  strip1.clear(); // Set all pixel colors to 'off'
  setAll3(0,0,0);
  strip5.clear(); // Set all pixel colors to 'off'
  delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip1.setPixelColor(i, orange);
 //               strip1.setPixelColor(i, strip1.gamma32(strip1.ColorHSV(yellowHUE)));
                strip5.setPixelColor(i, orange);
 //
 strip5.setPixelColor(i, strip5.gamma32(strip5.ColorHSV(yellowHUE)));
                strip1.setBrightness(150);
                strip5.setBrightness(150);
                strip1.show();   // Send the updated pixel colors to the hardware.
                strip5.show();   // Send the updated pixel colors to the hardware.
                delay(DELAYVAL); // Pause before next pass through loo       
             }
             strip1.setBrightness(255);
             strip5.setBrightness(255);
             strip1.fill(yellow, 0, NUMPIXELS1);
             strip5.fill(yellow, 0, NUMPIXELS1);
                      strip3.setPixelColor(0, 255, 255, 0);
        strip3.setPixelColor(1, 255, 255, 0);
        strip3.setPixelColor(2, 255, 255, 0);
        strip3.setPixelColor(3, 255, 255, 0);
        strip3.setPixelColor(4, 255, 255, 0);
        strip3.setPixelColor(5, 255, 255, 0);
        strip3.setPixelColor(6, 255, 255, 0);
        strip3.setPixelColor(7, 255, 255, 0);
        strip3.setPixelColor(136, 255, 255, 0);
        strip3.setPixelColor(137, 255, 255, 0);
        strip3.setPixelColor(138, 255, 255, 0);
        strip3.setPixelColor(139, 255, 255, 0);
        strip3.setPixelColor(140, 255, 255, 0);
        strip3.setPixelColor(141, 255, 255, 0);
        strip3.setPixelColor(142, 255, 255, 0);
        strip3.setPixelColor(143, 255, 255, 0); 
           strip3.show();
    strip1.show();  
    strip5.show(); 
    headlightturnON();
      delay(1000);       
    strip1.fill(nocolor, 0, NUMPIXELS1);
    strip3.fill(nocolor, 0, NUMPIXELS1);
    strip5.fill(nocolor, 0, NUMPIXELS1);
    strip1.show();
    strip3.show(); 
    strip5.show(); 
    headlightturnOFF();
}

void turnsignalhazardindef() {
  strip1.clear(); // Set all pixel colors to 'off'
  strip2.clear(); // Set all pixel colors to 'off'  
  setAll3(0,0,0);
  setAll4(0,0,0);
  strip5.clear(); // Set all pixel colors to 'off'
  strip6.clear(); // Set all pixel colors to 'off'
delay(500);
          for(int i=0; i<NUMPIXELS1; i++) { // For each pixel...
                strip1.setPixelColor(i, orange);
                strip2.setPixelColor(i, orange);
                strip5.setPixelColor(i, orange);
                strip6.setPixelColor(i, orange);               
                strip1.setBrightness(150);
                strip2.setBrightness(150);
                strip5.setBrightness(150);
                strip6.setBrightness(150);               
                strip1.show();   // Send the updated pixel colors to the hardware.
                strip2.show();   // Send the updated pixel colors to the hardware.
                strip5.show();   // Send the updated pixel colors to the hardware.
                strip6.show();   // Send the updated pi   
        strip3.setPixelColor(0, 255, 255, 0);
        strip3.setPixelColor(1, 255, 255, 0);
        strip3.setPixelColor(2, 255, 255, 0);
        strip3.setPixelColor(3, 255, 255, 0);
        strip3.setPixelColor(4, 255, 255, 0);
        strip3.setPixelColor(5, 255, 255, 0);
        strip3.setPixelColor(6, 255, 255, 0);
        strip3.setPixelColor(7, 255, 255, 0);
        strip3.setPixelColor(136, 255, 255, 0);
        strip3.setPixelColor(137, 255, 255, 0);
        strip3.setPixelColor(138, 255, 255, 0);
        strip3.setPixelColor(139, 255, 255, 0);
        strip3.setPixelColor(140, 255, 255, 0);
        strip3.setPixelColor(141, 255, 255, 0);
        strip3.setPixelColor(142, 255, 255, 0);
        strip3.setPixelColor(143, 255, 255, 0);
        strip4.setPixelColor(1, 255, 255, 0);
        strip4.setPixelColor(2, 255, 255, 0);
        strip4.setPixelColor(3, 255, 255, 0);
        strip4.setPixelColor(4, 255, 255, 0);
        strip4.setPixelColor(5, 255, 255, 0);
        strip4.setPixelColor(6, 255, 255, 0);
        strip4.setPixelColor(7, 255, 255, 0);
        strip4.setPixelColor(136, 255, 255, 0);
        strip4.setPixelColor(137, 255, 255, 0);
        strip4.setPixelColor(138, 255, 255, 0);
        strip4.setPixelColor(139, 255, 255, 0);
        strip4.setPixelColor(140, 255, 255, 0);
        strip4.setPixelColor(141, 255, 255, 0);
        strip4.setPixelColor(142, 255, 255, 0);
        strip4.setPixelColor(143, 255, 255, 0);   
        strip3.show(); 
        strip4.show();                                
    //            delay(DELAYVAL); // Pause before next pass through loop
             }
             strip1.setBrightness(255);
             strip2.setBrightness(255);
             strip5.setBrightness(255);
             strip6.setBrightness(255);
             strip1.fill(yellow, 0, NUMPIXELS1);
             strip2.fill(yellow, 0, NUMPIXELS1);
             strip5.fill(yellow, 0, NUMPIXELS1);
             strip6.fill(yellow, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.show();   
    strip5.show(); 
    strip6.show();
    headlightturnON();  
      delay(1000);       
    strip1.fill(nocolor, 0, NUMPIXELS1);
    strip2.fill(nocolor, 0, NUMPIXELS1);
    strip5.fill(nocolor, 0, NUMPIXELS1);
    strip6.fill(nocolor, 0, NUMPIXELS1);
    strip1.show(); 
    strip2.show(); 
    strip5.show(); 
    strip6.show(); 
    headlightturnOFF();
}

