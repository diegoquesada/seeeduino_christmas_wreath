#include <Adafruit_NeoPixel.h>
#include "animation.h"

extern Adafruit_NeoPixel pixels;

uint32_t lastAnimationChange = 0;
const uint32_t ANIMATION_LENGTH = 60000;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color_t;

typedef struct {
  uint16_t ms; // milliseconds to stay in this state
  Color_t pixels[30];
} AnimationStep_t;

typedef struct {
  uint8_t numSteps;
  const AnimationStep_t *steps;
} AnimationSpec_t;

const Color_t BLACK = { 0, 0, 0 };
const Color_t RED = { 0xFF, 0, 0 };
const Color_t GREEN = { 0, 0xFF, 0 };
const Color_t BLUE = { 0, 0, 0xFF };
const Color_t YELLOW = { 0xFF, 0xFF, 0 };
const Color_t WHITE = { 0xFF, 0xFF, 0xFF };

uint8_t currentStep = 0;
const AnimationSpec_t *currentAnimation = nullptr;

const AnimationStep_t animation1_Steps[2] = {
  { 1000, { RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK } },
  { 1000, { BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED } },
};
const AnimationSpec_t animation1 = { 2, animation1_Steps };

const AnimationStep_t animation2_Steps[2] = {
  { 500, { RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK } },
  { 500, { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
};
const AnimationSpec_t animation2 = { 2, animation2_Steps };

void animationInit()
{
  Serial.println("animationInit");

  currentAnimation = &animation1;
}

void animationStart()
{
  Serial.println("animationStart");

  currentStep = 0;
  pixels.setBrightness(128);
  lastAnimationChange = millis();
}

void animationNext()
{
  if (millis() - lastAnimationChange > ANIMATION_LENGTH )
  {
    Serial.println("animationNext");

    if (currentAnimation == &animation1)
        currentAnimation = &animation2;
    else
        currentAnimation = &animation1;
    animationStart();
  }
}

/**
 * Increments to the next step ni the animation.
*/
void animationStep()
{
  if ( currentStep >= currentAnimation->numSteps )
    currentStep = 0;

  uint16_t delayMs = currentAnimation->steps[currentStep].ms;

  for (uint8_t i = 0; i < 30; i++ )
  {
    const Color_t& thisColor = currentAnimation->steps[currentStep].pixels[i];
    pixels.setPixelColor( i, thisColor.r, thisColor.g, thisColor.b );
  }

  pixels.show();

  currentStep++;
  delay(delayMs);
}

void animationStop()
{
  Serial.println("Lights OFF");

  currentStep = 0;
  pixels.clear();
  pixels.show();

//  MsTimer2::set(SLEEP_INTERVAL, wakeup_interrupt_handler);
//  MsTimer2::start();
//  Serial.end();

//  set_sleep_mode (/*SLEEP_MODE_PWR_DOWN*/SLEEP_MODE_PWR_SAVE);  
//  sleep_enable();
//  sleep_cpu ();
}
