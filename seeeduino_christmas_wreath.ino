/**
 * Seeeduino Christmas Wreath Lights
 * Copyright 2022-2023 Diego Quesada
 * Animation and sounds for the KofC Christmas Party.
 */

#include <time.h>
#include "config.h"
#if ENABLE_LIGHTSENSOR==1
#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#endif
#if ENABLE_SOUNDS==1
#include "melodies.h"
#endif
//#include <MsTimer2.h>
//#include <avr/sleep.h>
#include "pins.h"
#include "animation.h"

//const uint16_t hallPin = 4;
//const unsigned long SLEEP_INTERVAL = 10000; // 10 seconds

void wakeup_interrupt_handler();


void setup()
{
  Serial.begin(9600);

  pinMode(SPEAKERPIN, OUTPUT);

#if ENABLE_SOUNDS==1
  digitalWrite(SPEAKERPIN, LOW);
#endif
//  pinMode(hallPin, INPUT);

/*   Wire.begin();
  TSL2561.init(); */

  animationInit();
  animationStart();

#if ENABLE_SOUNDS==1
  melodyInit();
  melodyStart();
#endif

  //MsTimer2::stop();
}

void loop()
{
  animationNext();
  animationStep();

#if ENABLE_SOUNDS==1
  melodyNext();
  melodyStep();
#endif

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
