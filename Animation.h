#ifndef Animation.h
#define Animation.h

boolean animfast = false;
#define ANIM_JUMP3 0
#define ANIM_JUMP7 1
#define ANIM_FADE3 2
#define ANIM_FADE7 3
#define ANIM_FLASH 4
#define ANIM_OFF -1
short currentanim = ANIM_OFF;

/* BELOW THIS LINE IS THE MATH -- YOU SHOULDN'T NEED TO CHANGE THIS FOR THE BASICS

    ##### TAKEN FROM THE ARDUINO TUTORIALS #####

  The program works like this:
  Imagine a crossfade that moves the red LED from 0-10,
    the green from 0-5, and the blue from 10 to 7, in
    ten steps.
    We'd want to count the 10 steps and increase or
    decrease color values in evenly stepped increments.
    Imagine a + indicates raising a value by 1, and a -
    equals lowering it. Our 10 step fade would look like:

    1 2 3 4 5 6 7 8 9 10
  R + + + + + + + + + +
  G   +   +   +   +   +
  B     -     -     -

  The red rises from 0 to 10 in ten steps, the green from
  0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.

  In the real program, the color percentages are converted to
  0-255 values, and there are 1020 steps (255*4).

  To figure out how big a step there should be between one up- or
  down-tick of one of the LED values, we call calculateStep(),
  which calculates the absolute gap between the start and end values,
  and then divides that gap by 1020 to determine the size of the step
  between adjustments in the value.
*/

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step != 0) {                      // If its non-zero,
    step = 1020 / step;            //   divide by 1020
  }
  return step;
}

int calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;
    }
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    }
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  }
  else if (val < 0) {
    val = 0;
  }
  return val;
}

void animate() {
  static short currentRandomColor = 0;
  static unsigned long nextRun = millis() - 1;

  //anim fade variables
  static int i = 0;
  //start with black (off)
  static int redVal = 0;
  static int grnVal = 0;
  static int bluVal = 0;
  static int prevR = 0;
  static int prevG = 0;
  static int prevB = 0;
  static int stepR = 0;
  static int stepG = 0;
  static int stepB = 0;

  //anim flash
  static int flashstate = 0;

  //only run when animation is needed
  if ( currentanim > ANIM_OFF ) {
    switch (currentanim) {

      case ANIM_JUMP3:
      case ANIM_JUMP7:
        if ( nextRun < millis() ) {
          if ( ( currentanim == ANIM_JUMP3 )
               && ( ++currentRandomColor > 2 ) )
            currentRandomColor = 0;

          if ( ( currentanim == ANIM_JUMP7 )
               && ( ++currentRandomColor > 6 ) )
            currentRandomColor = 0;

          setGammaCorrectedColor(0, randomColors[currentRandomColor]);

          if ( animfast ) {
            nextRun = millis() + 1000;
          } else {
            nextRun = millis() + 2000;
          }
        }
        break;

      case ANIM_FADE3:
      case ANIM_FADE7:
        if ( nextRun < millis() ) {
          //re-initialize if step is over 1020
          if ( i > 510 ) { //1020
            i = 0;
          }

          if ( i == 0 ) {

            if ( ( currentanim == ANIM_FADE3 )
                 && ( ++currentRandomColor > 2 ) )
              currentRandomColor = 0;

            if ( ( currentanim == ANIM_FADE7 )
                 && ( ++currentRandomColor > 6 ) )
              currentRandomColor = 0;

            stepR = calculateStep(prevR, (int) randomColors[currentRandomColor].r );
            stepG = calculateStep(prevG, (int) randomColors[currentRandomColor].g );
            stepB = calculateStep(prevB, (int) randomColors[currentRandomColor].b );
          }

          //former for loop
          redVal = calculateVal(stepR, redVal, i);
          grnVal = calculateVal(stepG, grnVal, i);
          bluVal = calculateVal(stepB, bluVal, i);

          if ( i == 0 ) {
            // Update current values for next loop
            prevR = redVal;
            prevG = grnVal;
            prevB = bluVal;
          }

          myRGB fadedColor = { redVal, grnVal, bluVal };
          setColor(0, fadedColor );

          i++;

          nextRun = 0; //run as fast as possible
        }
        break;

      case ANIM_FLASH:
        if ( nextRun < millis() ) {
          switch (flashstate) {
            case 0:
              if ( ++currentRandomColor > 6 )
                currentRandomColor = 0;
              setColor(0, randomColors[currentRandomColor]);
              nextRun = millis() + 600;
              flashstate++;
              break;
            case 1:
              setColor(0, {0, 0, 0});
              nextRun = millis() + 300;
              flashstate++;
              break;
            case 2:
              setColor(0, randomColors[currentRandomColor]);
              nextRun = millis() + 350;
              flashstate++;
              break;
            case 3:
              setColor(0, {0, 0, 0});
              nextRun = millis() + 120;
              flashstate++;
              break;
            case 4:
              setColor(0, randomColors[currentRandomColor]);
              nextRun = millis() + 300;
              flashstate++;
              break;
            case 5:
              setColor(0, {0, 0, 0});
              nextRun = millis() + 1500;
              flashstate = 0;
              break;
          }
        }
        break;
    }
  }
}

#endif
