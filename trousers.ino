//LED running trousers project
//Archie Roques 2017
//Built with Adafruit's LIS3DH and Neopixel libraries and sample code

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>


//set pin pixels are connected to
#define PIN 7
//define neopixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(29, PIN, NEO_GRB + NEO_KHZ800);

//set all the variables to 0
int counter = 0;
int lit = 0;
int brightness = 0;

//assign a random brightness to each col to create a random RGB colour
int red = random(2,10);
int green = random(2,10);
int blue = random(2,10);

//set the brighnesses to 0
int red_bright = 0;
int blue_bright = 0;
int green_bright = 0;


//how many steps do you want to take?
int target_steps = 28; 

//only light one pixel per X steps, scales values to 28 pixel loop
int divisor = target_steps / 28; 

//define the accelerometer
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

//sensitivity of click value
#define CLICKTHRESHHOLD 80

void setup(void) {
  //begin a serial connection for debug and testing
  Serial.begin(9600);
    Serial.println("Trousers!");
  //try to start accelerometer
  if (! lis.begin(0x18)) {
    Serial.println("Couldnt start accelerometer");
    while (1);
  }
  Serial.println("Accelerometer OK");

  Serial.println(red);
  Serial.print(green);
  Serial.print(blue);
  
  lis.setRange(LIS3DH_RANGE_2_G);
  strip.begin();
  strip.show();

  lis.setClick(2, CLICKTHRESHHOLD);
  delay(100);

}


void loop() {
  uint8_t click = lis.getClick();

  //if we get a double tap
  if (click == 0) return;
  if (! (click & 0x30)) return;
  //increment the counter by one
  counter = counter + 1;
  //print counter to serial for testing
  Serial.println(counter);
  //check which pixel to light
  lit = counter / divisor;
  //set brightness for each colour as 9 times the counter, because 9 28s are almost 255
  red_bright = red * lit;
  green_bright = green * lit;
  blue_bright = blue * lit;
  //start pixels from opposite end to the connected one
  lit = 28 - lit;
  
   //light pixel in greenm (the last 3 numbers are the RGB bitstream)
  strip.setPixelColor(lit, red_bright, green_bright, blue_bright);
  //send colours
  strip.show();
  //if there are more steps than the target revert to zero and reset the strip
  if (counter > (target_steps + 1)) 
  {
   counter = 0;
   strip.clear();
   strip.show();

   //reset the random values too
   red = random(2,10);
   green = random(2,10);
   blue = random(2,10);
  
   }
  
  //wait .1 secs
  delay(100);
  return;
}
