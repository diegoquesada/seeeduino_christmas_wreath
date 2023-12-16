#include <Adafruit_NeoPixel.h>
#include "animation.h"
#include "pins.h"
#include "colors.h"

Adafruit_NeoPixel pixels(30, PIXELSPIN, NEO_GRB + NEO_KHZ800);

uint32_t lastAnimationChange = 0;
const uint32_t ANIMATION_LENGTH = 60000;
uint32_t nextStepChange = 0; // Time in ms for the next animation step change

/**
 * Declaration for a function that can be called to set the LEDs
 * Takes the step number as an argument and returns the delay in ms.
 */
typedef uint16_t (*AnimationStepFunction)(uint8_t, uint32_t);

uint16_t fadeColor(uint8_t step, uint32_t parameter);
uint16_t scrollArray(uint8_t step, uint32_parameter);

typedef struct {
  uint16_t ms; // milliseconds to stay in this state
  Color_t pixels[30];
} AnimationStep_t;

typedef struct {
  uint8_t numSteps;     // number of steps in the animation
  uint8_t type;         // 0: array of steps, 1: function
  union {
    const AnimationStep_t *steps;
    AnimationStepFunction fx;
  };
  uint32_t parameter;   // parameter to be passed to fx
} AnimationSpec_t;

uint8_t currentStep = 0;
const AnimationSpec_t *currentAnimation = nullptr;

const AnimationStep_t animation1_Steps[2] = {
  { 1000, { RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK } },
  { 1000, { BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED, BLACK, GREEN, BLACK, YELLOW, BLACK, RED } },
};
const AnimationSpec_t animation1 = { 2, 0, { .steps = animation1_Steps }, 0 };

const AnimationStep_t animation2_Steps[2] = {
  { 500, { RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK } },
  { 500, { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
};
const AnimationSpec_t animation2 = { 2, 0, { .steps = animation2_Steps }, 0 };

/* const AnimationStep_t animation3_Steps[14] = {
  { 250, { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED50, RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { RED25, RED50, RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { BLACK, RED25, RED50, RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { BLACK, BLACK, RED25, RED50, RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
  { 250, { BLACK, BLACK, BLACK, RED25, RED50, RED75, RED, RED, RED, RED, RED75, RED50, RED25, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK } },
};
const AnimationSpec_t animation3 = { 14, 0, { .steps = animation3_Steps }, 0 };
 */
const AnimationSpec_t animation3 = { 16, 1, { .fx = fadeColor }, 0x002000 };
const AnimationSpec_t animation4 = { 16, 1, { .fx = fadeColor }, 0x200000 };
const AnimationSpec_t animation5 = { 16, 1, { .fx = fadeColor }, 0x000020 };

void animationInit()
{
  Serial.println("animationInit");

  pixels.begin();

  currentAnimation = &animation3;
}

void animationStart()
{
  Serial.println("animationStart");

  currentStep = 0;
  pixels.setBrightness(128);
  lastAnimationChange = millis();
  nextStepChange = millis();
}

/**
 * Trigger the next animation when the required time has passed.
*/
void animationNext()
{
  if (millis() - lastAnimationChange > ANIMATION_LENGTH)
  {
    Serial.println("animationNext");

    if (currentAnimation == &animation1)
      currentAnimation = &animation2;
    else if (currentAnimation == &animation2)
      currentAnimation = &animation3;
    else if (currentAnimation == &animation3)
      currentAnimation = &animation4;
    else if (currentAnimation == &animation4)
      currentAnimation = &animation5;
    else
      currentAnimation = &animation1;
    animationStart();
  }
}

/**
 * Increments to the next step in the current animation.
*/
void animationStep()
{
  if (millis()> nextStepChange)
  {
    if ( currentStep >= currentAnimation->numSteps )
      currentStep = 0; // switch back to the first step

    nextStepChange = millis() + currentAnimation->steps[currentStep].ms;
    
    if (currentAnimation->type == 0)
    {
      Serial.print("Array step ");
      Serial.println(currentStep);

      for (uint8_t i = 0; i < 30; i++ )
      {
        const Color_t& thisColor = currentAnimation->steps[currentStep].pixels[i];
        pixels.setPixelColor( i, thisColor.r, thisColor.g, thisColor.b );
      }
    }
    else
    {
      nextStepChange = millis() + currentAnimation->fx(currentStep, currentAnimation->parameter);
    }

    pixels.show();
    currentStep++;
  }
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


uint16_t fadeColor(uint8_t step, uint32_t parameter)
{
  // 16 steps, 0 - 8 descending, 9 to 16 ascending
  // FF, E0, C0, A0, 80, 60, 40, 20, 0
  uint32_t color = 0;
  if (step < 8)
  {
    color = ((((parameter & 0xFF0000) >> 16) * (8 - step) - 1) << 16) |
            ((((parameter & 0x00FF00) >>  8) * (8 - step) - 1) <<  8) |
            ((((parameter & 0x0000FF)      ) * (8 - step) - 1));
    //color = ((uint32_t)0x20 * (8 - step) - 1) << 8;
  }
  else
  {
    color = ((((parameter & 0xFF0000) >> 16) * (step - 8)) << 16) |
            ((((parameter & 0x00FF00) >>  8) * (step - 8)) <<  8) |
            ((((parameter & 0x0000FF)      ) * (step - 8)));
    //color = ((uint32_t)0x20 * (step - 8)) << 8;
  }

  for (int i = 0; i < 30; i++)
    pixels.setPixelColor(i, color);

  return 200;
}

uint16_t scrollArray(uint8_t step, uint32_parameter)
{
  
}