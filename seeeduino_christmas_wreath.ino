/**
 * Seeeduino Christmas Wreath Lights
 * Copyright 2022-2023 Diego Quesada
 * Animation and sounds for the KofC Christmas Party.
 */

#include <time.h>
#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#include <Adafruit_NeoPixel.h>
//#include <MsTimer2.h>
//#include <avr/sleep.h>
#include "melodies.h"
#include "animation.h"

#define PIXELSPIN A5 //SCL on the Grove connector
#define SPEAKERPIN A3

//const uint16_t hallPin = 4;
Adafruit_NeoPixel pixels(30, PIXELSPIN, NEO_GRB + NEO_KHZ800);
//const unsigned long SLEEP_INTERVAL = 10000; // 10 seconds

void wakeup_interrupt_handler();


void setup()
{
  Serial.begin(9600);

    pinMode(SPEAKERPIN, OUTPUT);
    digitalWrite(SPEAKERPIN, LOW);
//  pinMode(hallPin, INPUT);

/*   Wire.begin();
  TSL2561.init(); */

  pixels.begin();

  animationInit();
  animationStart();

  melodyInit();
  melodyStart();

  //MsTimer2::stop();
}

void loop()
{
  animationNext();
  animationStep();

//  else
//  {
//    animationStop();
//  }
}

/**
 * Interrupt handler for the wake up timer.
 */
void wakeup_interrupt_handler()
{
}
