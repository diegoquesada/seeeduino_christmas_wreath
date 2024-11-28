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
uint16_t scrollArray(uint8_t step, uint32_t parameter);

/// A single step in an animation, consisting of its duration and LED pattern.
typedef struct {
  uint16_t ms; // milliseconds to stay in this state
  Color_t pixels[30];
} AnimationStep_t;

/// Definition of an animation, consisting of number of steps, type and parameters.
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
const AnimationSpec_t animation3 = { 36, 1, { .fx = fadeColor }, 0x000100 }; // Fading green
const AnimationSpec_t animation4 = { 36, 1, { .fx = fadeColor }, 0x010000 }; // Fading red
const AnimationSpec_t animation5 = { 36, 1, { .fx = fadeColor }, 0x000001 }; // Fading blue

const AnimationSpec_t animation6 = { 30, 1, { .fx = scrollArray }, 0x000100 };
const AnimationSpec_t animation7 = { 30, 1, { .fx = scrollArray }, 0x010000 };

void animationInit()
{
  Serial.println("animationInit");

  pixels.begin();

  currentAnimation = &animation6;
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
    else if (currentAnimation == &animation5)
      currentAnimation = &animation6;
    else if (currentAnimation == &animation6)
      currentAnimation = &animation7;
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
  if (millis() > nextStepChange)
  {
    if ( currentStep > currentAnimation->numSteps )
      currentStep = 0; // switch back to the first step

    if (currentAnimation->type == 0)
    {
      Serial.print("Array step ");
      Serial.println(currentStep);

      nextStepChange = millis() + currentAnimation->steps[currentStep].ms;
    
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
  uint8_t fadeArray[] = { 0xF0, 0xD0, 0xB0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x8, 0x04, 0x00 };
  uint32_t color = 0;
  if (step < 18)
  {
    color = ((((parameter & 0xFF0000) >> 16) * fadeArray[17 - step]) << 16) |
            ((((parameter & 0x00FF00) >>  8) * fadeArray[17 - step]) <<  8) |
            ((((parameter & 0x0000FF)      ) * fadeArray[17 - step]));
  }
  else
  {
    color = ((((parameter & 0xFF0000) >> 16) * fadeArray[step - 18]) << 16) |
            ((((parameter & 0x00FF00) >>  8) * fadeArray[step - 18]) <<  8) |
            ((((parameter & 0x0000FF)      ) * fadeArray[step - 18]));
  }
  for (int i = 0; i < 30; i++)
    pixels.setPixelColor(i, color);

  return 100;
}

uint16_t scrollArray(uint8_t step, uint32_t parameter)
{
  uint8_t scrollArray[] = { 0x10, 0x20, 0x30, 0x40, 0x60, 0x80, 0xB0, 0xF0, 0xB0, 0x80, 0x60, 0x40, 0x30, 0x20, 0x10 };
  for (int i = 0; i < 30; i++)
  {
    if (i < step && step < i + 15)
    {
      uint32_t factor = scrollArray[i + 15 - step];
      uint32_t color = ((((parameter & 0xFF0000) >> 16) * factor) << 16) |
                       ((((parameter & 0x00FF00) >>  8) * factor) <<  8) |
                       ((((parameter & 0x0000FF)      ) * factor));
      pixels.setPixelColor(i, color);
    }
    else
      pixels.setPixelColor(i, 0x00);
  }

  return 100;
}
/*15 - step + i < 14
14 + i < 14 + step
i < step

14 - step + i > 0
i > step - 14

step 0 : B  B  B  B  B  B
step 1 : 14 B  B  B  B  B
step 2 : 13 14 B  B  B  B
step 3 : 12 13 14 B  B  B
step 4 : 11 12 13 14 B  B

                              i < step ? scrollArray[i + 15 - step]
step 13 : 2  3  4  5  6  7    i < 13 ? scrollArray[i + 2] : B
step 14 : 1  2  3  4  5  6    i < 14 ? scrollArray[i + 1] : B
step 15 : 0  1  2  3  4  5    i < 15 ? scrollArray[i] : B
step 16 : B  0  1  2  3  4    i >= step - 15
step 17 : B  B  0  1  2  3    i + 15 >= step
                              step < i + 15*/