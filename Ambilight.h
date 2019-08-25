#ifndef Ambilight.h
#define Ambilight.h
#include <SoftwareSerial.h>

boolean ambilightActive = false;

int SerialBuffer = 0;

int RedSum = 0;
int GreenSum = 0;
int BlueSum = 0;

int RedLeft = 0;
int GreenLeft = 0;
int BlueLeft = 0;

int RedRight = 0;
int GreenRight = 0;
int BlueRight = 0;

int RedTop = 0;
int GreenTop = 0;
int BlueTop = 0;

int RedBottom = 0;
int GreenBottom = 0;
int BlueBottom = 0;

SoftwareSerial BT(RXPIN, TXPIN);

//packet structure: FF 00 00 0F [rr gg bb (sum)] [rr gg bb left] [rr gg bb (right)] [rr gg bb (top)] [rr gg bb (bottom)]
//                           ^^ = Number of Channels => 15 => 5 x 3 bytes per channel
void ambilight() {
  static int bytesAvail = 0;
  static int packetCounter = 1;

  //if, for whatever reason, the statemachine got stuck reset it on an overflow
  if (BT.overflow()) {
    packetCounter = 1;
  }

  if ( ( packetCounter <= 4) && (BT.available() > 0) ) { //see if enough data for the start byte have arrived
    bytesAvail = BT.available();  //save it so we wont end in an endless loop (working slower than new data arrives)
    while (bytesAvail > 0 && packetCounter <= 4) {
      SerialBuffer = BT.read();
      bytesAvail--;

      if ( (packetCounter == 1) && (SerialBuffer == 0xff) ) { //first byte of start record as expected
        packetCounter++;
        continue; //jump back to loop start
      }

      if ( (packetCounter == 2) && (SerialBuffer == 0x00) ) { //second byte of start record as expected
        packetCounter++;
        continue; //jump back to loop start
      }

      if ( (packetCounter == 3) && (SerialBuffer == 0x00) ) { //third byte of start record as expected
        packetCounter++;
        continue; //jump back to loop start
      }

      if ( (packetCounter == 4) && (SerialBuffer == 0x0f) ) { //fourth byte of start record as expected
        packetCounter++;
        continue; //jump back to loop start
      }
    }
  }

  if ( (packetCounter >= 5) && (packetCounter <= 19) && (BT.available() > 0) ) { //we retrieved the start byte now its just rgb colors -> SUM, left, right, top, bottom (3 packets each)
    bytesAvail = BT.available();  //save it so we wont end in an endless loop (working slower than new data arrives)
    while (bytesAvail > 0 && packetCounter <= 19) {
      SerialBuffer = BT.read();
      bytesAvail--;

      switch (packetCounter) {
        case 5: // 5,6,7 packets will be rgb for sum
          RedSum = SerialBuffer;
          packetCounter++;
          break;
        case 6:
          GreenSum = SerialBuffer;
          packetCounter++;
          break;
        case 7:
          BlueSum = SerialBuffer;
          packetCounter++;
          break;

        case 8: // 8.9.10 packets will be rgb for left
          RedLeft = SerialBuffer;
          packetCounter++;
          break;
        case 9:
          GreenLeft = SerialBuffer;
          packetCounter++;
          break;
        case 10:
          BlueLeft = SerialBuffer;
          packetCounter++;
          break;

        case 11: // 11,12,13 packets will be rgb for right
          RedRight = SerialBuffer;
          packetCounter++;
          break;
        case 12:
          GreenRight = SerialBuffer;
          packetCounter++;
          break;
        case 13:
          BlueRight = SerialBuffer;
          packetCounter++;
          break;

        case 14: // 14,15,16 packets will be rgb for top
          RedTop = SerialBuffer;
          packetCounter++;
          break;
        case 15:
          GreenTop = SerialBuffer;
          packetCounter++;
          break;
        case 16:
          BlueTop = SerialBuffer;
          packetCounter++;
          break;

        case 17: // 17,18,19 packets will be rgb for bottom
          RedBottom = SerialBuffer;
          packetCounter++;
          break;
        case 18:
          GreenBottom = SerialBuffer;
          packetCounter++;
          break;
        case 19:
          BlueBottom = SerialBuffer;
          packetCounter++;
          break;
      }
    }
  }

  //if we got 19 packets fine, lets set the colors and re-initialize for the nexe picture
  if (packetCounter >= 19) {
    packetCounter = 1;

#ifdef UNO
    analogWrite(R1, RedSum);
    analogWrite(G1, GreenSum);
    analogWrite(B1, BlueSum);
#endif

#ifdef MEGA
    analogWrite(R1, RedTop);
    analogWrite(G1, GreenTop);
    analogWrite(B1, BlueTop);

    analogWrite(R2, RedRight);
    analogWrite(G2, GreenRight);
    analogWrite(B2, BlueRight);

    analogWrite(R3, RedBottom);
    analogWrite(G3, GreenBottom);
    analogWrite(B3, BlueBottom);

    analogWrite(R4, RedLeft);
    analogWrite(G4, GreenLeft);
    analogWrite(B4, BlueLeft);
#endif
  }
}
#endif
