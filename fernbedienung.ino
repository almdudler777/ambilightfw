#include <Thread.h>

// use this to build for UNO (development) or MEGA (release)
#define UNO

#ifdef UNO
#define R1 5
#define G1 6
#define B1 3
#endif

#ifdef MEGA
#define R1 4
#define G1 3
#define B1 2

#define R2 7
#define G2 6
#define B2 5

#define R3 8
#define G3 9
#define B3 10

#define R4 11
#define G4 12
#define B4 13
#endif

Thread IRThread = Thread();
Thread AnimationThread = Thread();

//yeah it's wrong to have everything in .h instead of splitted into .h and .cpp - just couldn't be bothered ...
#include "Color.h"
#include "Animation.h"
#include "IR.h"

void setup()
{
  pinMode(R1, OUTPUT); analogWrite(R1, 0);
  pinMode(G1, OUTPUT); analogWrite(G1, 0);
  pinMode(B1, OUTPUT); analogWrite(B1, 0);

#ifdef MEGA
  pinMode(R2, OUTPUT); analogWrite(R2, 0);
  pinMode(G2, OUTPUT); analogWrite(G2, 0);
  pinMode(B2, OUTPUT); analogWrite(B2, 0);

  pinMode(R3, OUTPUT); analogWrite(R3, 0);
  pinMode(G3, OUTPUT); analogWrite(G3, 0);
  pinMode(B3, OUTPUT); analogWrite(B3, 0);

  pinMode(R4, OUTPUT); analogWrite(R4, 0);
  pinMode(G4, OUTPUT); analogWrite(G4, 0);
  pinMode(B4, OUTPUT); analogWrite(B4, 0);
#endif

  //Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  IRThread.onRun(readIRCode);
  IRThread.setInterval(200);

  AnimationThread.onRun(animate);
  AnimationThread.setInterval(10);
}

void loop() {
  if (IRThread.shouldRun()) {
    IRThread.run();
  }

  if ( ( currentanim > ANIM_OFF )
       && AnimationThread.shouldRun()) {
    AnimationThread.run();
  }
}
