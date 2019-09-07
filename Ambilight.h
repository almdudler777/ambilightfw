#ifndef Ambilight.h
#define Ambilight.h
#include <SoftwareSerial.h>

#ifdef UNO
//const auto& AMBISERIAL = Serial;
#endif

#ifdef MEGA
//const auto& AMBISERIAL = Serial3;
#endif

boolean ambilightActive = true;

int SerialBuffer = 0;
int packets[20];

SoftwareSerial AMBISERIAL(RXPIN, TXPIN);

//packet structure: FF 00 00 0F   [rr gg bb (sum)] [rr gg bb left] [rr gg bb (right)]   [rr gg bb (top)] [rr gg bb (bottom)]
//                           ^^ = Number of Channels => 15 => 5 x 3 bytes per channel
//                                 05 06 07         08 09 10        11 12 13             14 15 16         17 18 19
void ambilight() {
  static int bytesAvail = 0;
  static int packetCounter = 1;

  if ( ( packetCounter <= 4) && (AMBISERIAL.available() > 0) ) { //see if enough data for the start byte have arrived
    bytesAvail = AMBISERIAL.available();  //save it so we wont end in an endless loop (working slower than new data arrives)
    while (bytesAvail > 0 && packetCounter <= 4) {
      SerialBuffer = AMBISERIAL.read();
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

      //if we reach this, we received some packet that was not in the right order (restart sequence)
      packetCounter = 1;

      //special case when we received => 0xff 0xff 0x00 0x00 0x0f when we discard the current SerialBuffer we would "waste" one 0xff which could be our "real" startbyte
      if (SerialBuffer == 0xff) {
        packetCounter = 2;
      }

      /*switch (SerialBuffer) {
        case 0xff:
          if (packetCounter == 1) {
            packetCounter++;
          } else {
            packetCounter = 2; //wrong order but two 0xff after another so assume current is the new start byte
          }
          break;
        case 0x00:
          if (packetCounter == 2 || packetCounter == 3) {
            packetCounter++;
          } else {
            packetCounter = 1; //wrong order reset to first packet
          }
          break;
        case 0x0f:
          if (packetCounter == 4) {
            packetCounter++;
          } else {
            packetCounter = 1; //wrong order reset to first packet
          }
          break;
        default:
          packetCounter = 1; //any other packet will reset the sequence
          break;
        }*/
    }
  }

  if ( (packetCounter >= 5) && (packetCounter <= 19) && (AMBISERIAL.available() > 0) ) { //we retrieved the start byte now its just rgb colors -> SUM, left, right, top, bottom (3 packets each)
    bytesAvail = AMBISERIAL.available();  //save it so we wont end in an endless loop (working slower than new data arrives)
    while (bytesAvail > 0 && packetCounter <= 19) {
      SerialBuffer = AMBISERIAL.read();
      bytesAvail--;
      packets[packetCounter++] = SerialBuffer;
    }
  }

  //if we got 19 packets fine, lets set the colors and re-initialize for the nexe picture
  if (packetCounter > 19) {
#ifdef UNO
    analogWrite(R1, packets[5]);
    analogWrite(G1, packets[6]);
    analogWrite(B1, packets[7]);
#endif

#ifdef MEGA
    analogWrite(R1, packets[8]);
    analogWrite(G1, packets[9]);
    analogWrite(B1, packets[10]);

    analogWrite(R2, packets[11]);
    analogWrite(G2, packets[12]);
    analogWrite(B2, packets[13]);

    analogWrite(R3, packets[14]);
    analogWrite(G3, packets[15]);
    analogWrite(B3, packets[16]);

    analogWrite(R4, packets[17]);
    analogWrite(G4, packets[18]);
    analogWrite(B4, packets[19]);
#endif
    packetCounter = 1;
  }
}
#endif
