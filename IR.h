#ifndef IR.h
#define IR.h
#include <IRremote.h>

#ifdef UNO
int RECV_PIN = 7;
#endif

#ifdef MEGA
int RECV_PIN = 52;
#endif

IRrecv irrecv(RECV_PIN);
decode_results results;
int lastcode = 0;

void readIRCode() {
  int currentcode;
  boolean repeated = false;

  if (irrecv.decode(&results)) {

    String result = String(results.value, HEX);
    
    if ( (result.length() == 6 || result.length() == 8)  && result.startsWith("ff")) {

      currentcode = results.value;

      if (result.equals("ffffffff") && lastcode != currentcode) {
        currentcode = lastcode;
        repeated = true;
      } else {
        lastcode = currentcode;
        repeated = false;
      }

      switch (currentcode) {
        case 0xff3ac5: //helligkeit hoch
          currentanim = ANIM_OFF;
          setColor(0, brighter(currentcolor));
          break;
        case 0xffba45: //Helligkeit runter
          currentanim = ANIM_OFF;
          setColor(0, dimmer(currentcolor));
          break;
        case 0xff827d: //play (ambilight)
          currentanim = ANIM_OFF;
          break;
        case 0xff02fd: //On/Off
          currentanim = ANIM_OFF;
          setColor(0, {0, 0, 0});
          break;

        case 0xff1ae5: //#b5041e
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {181, 4, 30});
          break;
        case 0xff9a65: //#007944
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {0, 121, 68});
          break;
        case 0xffa25d: //#172772
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {23, 39, 114});
          break;
        case 0xff22dd: //#ffffff
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {255, 255, 255});
          break;

        case 0xff2ad5: //#dc4423
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {220, 68, 35});
          break;
        case 0xffaa55: //#5fb39b
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {95, 179, 155});
          break;
        case 0xff926d: //#00619b
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {0, 97, 155});
          break;
        case 0xff12ed: //#d47ea3
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {212, 126, 163});
          break;

        case 0xff0af5: //#d4742a
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {212, 116, 42});
          break;
        case 0xff8a75: //#3fc1e1
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {63, 193, 225});
          break;
        case 0xffb24d: //#34125c
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {52, 18, 92});
          break;
        case 0xff32cd: //#f6a6cb
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {246, 166, 203});
          break;

        case 0xff38c7: //#de9c2e
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {222, 156, 46});
          break;
        case 0xffb847: //#038b89
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {3, 139, 137});
          break;
        case 0xff7887: //#8a3f82
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {138, 63, 130});
          break;
        case 0xfff807: //#6fc1e6
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {111, 193, 230});
          break;

        case 0xff18e7: //#FFFF00
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {255, 255, 0});
          break;
        case 0xff9867: //#1f818e
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {31, 129, 142});
          break;
        case 0xff58a7: //#d75f8f
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {215, 95, 143});
          break;
        case 0xffd827: //#6cbfe7
          currentanim = ANIM_OFF;
          setGammaCorrectedColor(0, {108, 191, 231});
          break;

        case 0xff28d7: //rot hoch
          currentanim = ANIM_OFF;
          setColor(0, moreC('r', currentcolor));
          break;
        case 0xffa857: //grun hoch
          currentanim = ANIM_OFF;
          setColor(0, moreC('g', currentcolor));
          break;
        case 0xff6897: //blau hoch
          currentanim = ANIM_OFF;
          setColor(0, moreC('b', currentcolor));
          break;
        case 0xffe817: //quick
          animfast = true;
          break;

        case 0xff08f7: //rot runter
          currentanim = ANIM_OFF;
          setColor(0, lessC('r', currentcolor));
          break;
        case 0xff8877: //grun runter
          currentanim = ANIM_OFF;
          setColor(0, lessC('g', currentcolor));
          break;
        case 0xff48b7: //blau runter
          currentanim = ANIM_OFF;
          setColor(0, lessC('b', currentcolor));
          break;
        case 0xffc837: //slow
          animfast = false;
          break;

        case 0xff30cf: //diy 1
          currentanim = ANIM_OFF;
          break;
        case 0xffb04f: //diy 2
          currentanim = ANIM_OFF;
          break;
        case 0xff708f: //diy 3
          currentanim = ANIM_OFF;
          break;
        case 0xfff00f: //auto
          currentanim = ANIM_OFF;
          break;

        case 0xff10ef: //diy 4
          currentanim = ANIM_OFF;
          break;
        case 0xff906f: //diy 5
          currentanim = ANIM_OFF;
          break;
        case 0xff50af: //diy 6
          currentanim = ANIM_OFF;
          break;
        case 0xffd02f: //flash
          currentanim = ANIM_FLASH;
          break;

        case 0xff20df: //jump 3
          currentanim = ANIM_JUMP3;
          break;
        case 0xffa05f: //jump 7
          currentanim = ANIM_JUMP7;
          break;
        case 0xff609f: //fade 3
          currentanim = ANIM_FADE3;
          break;
        case 0xffe01f: //fade 7
          currentanim = ANIM_FADE7;
          break;

      }
    }
    irrecv.resume(); // Receive the next value
  }
}

#endif
