#ifndef Color.h
#define Color.h
#include <RGBConverter.h>

struct myRGB {
  byte r;
  byte g;
  byte b;
};

myRGB currentcolor = {0, 0, 0};
extern const uint8_t gamma8[];
RGBConverter RGBConv;
myRGB randomColors[7] = {
  { 235,  52, 52 },
  { 235, 171, 52 },
  { 122, 235, 52 },
  { 52, 235, 201 },
  { 52, 122, 235 },
  { 128, 52, 235 },
  { 255, 255, 0  }
};

void setColor(short zone, myRGB color) {
  currentcolor = color;
  switch (zone) {
    case 0:
#ifdef MEGA
      for (int i = 1; i < 5; i++) {
        setColor(i, color);
      }
#endif
#ifdef UNO
      setColor(1, color);
#endif
      break;
    case 1:
      analogWrite(R1, color.r);
      analogWrite(G1, color.g);
      analogWrite(B1, color.b);
      break;
#ifdef MEGA
    case 2:
      analogWrite(R2, color.r);
      analogWrite(G2, color.g);
      analogWrite(B2, color.b);
      break;
    case 3:
      analogWrite(R3, color.r);
      analogWrite(G3, color.g);
      analogWrite(B3, color.b);
      break;
    case 4:
      analogWrite(R4, color.r);
      analogWrite(G4, color.g);
      analogWrite(B4, color.b);
      break;
#endif
  }
}

void setGammaCorrectedColor(short zone, myRGB color) {
  myRGB correctedColor = {  pgm_read_byte(&gamma8[color.r]),
                            pgm_read_byte(&gamma8[color.g]),
                            pgm_read_byte(&gamma8[color.b])
                         };
  setColor(zone, correctedColor);
}

double getLuminance(myRGB color) {
  double hsl[3];
  RGBConv.rgbToHsl(color.r, color.g, color.b, hsl);
  return hsl[2];
}

myRGB setLuminance(double luminance, myRGB color) {
  //declare hsl array that the converter uses to store values
  double hsl[3];
  byte rgb[3];

  if ( luminance <= 1.0d && luminance >= 0 ) {
    //convert RGB to HSL
    RGBConv.rgbToHsl(color.r, color.g, color.b, hsl);

    //convert back to rgb
    RGBConv.hslToRgb(hsl[0], hsl[1], luminance, rgb);
  }

  return { rgb[0], rgb[1], rgb[2] };
}

myRGB brighter(myRGB color) {
  double luminance = getLuminance(color);

  luminance += 0.1d;
  if (luminance > 0.8d) {
    luminance = 0.8d;
  }

  return setLuminance(luminance, color);
}

myRGB dimmer(myRGB color) {
  double luminance = getLuminance(color);

  luminance -= 0.1d;
  if (luminance < 0.1d) {
    luminance = 0.1d;
  }

  return setLuminance(luminance, color);
}

myRGB moreC(char c, myRGB color) {
  switch (c) {
    case 'r':
      if (color.r + 10 <= 255) {
        color.r += 10;
      } else {
        color.r = 255;
      }
      break;
    case 'b':
      if (color.b + 10 <= 255) {
        color.b += 10;
      } else {
        color.b = 255;
      }
      break;
    case 'g':
      if (color.g + 10 <= 255) {
        color.g += 10;
      } else {
        color.g = 255;
      }
      break;
  }

  return color;
}

myRGB lessC(char c, myRGB color) {
  switch (c) {
    case 'r':
      if (color.r - 10 >= 0) {
        color.r -= 10;
      } else {
        color.r = 0;
      }
      break;
    case 'b':
      if (color.b - 10 >= 0) {
        color.b -= 10;
      } else {
        color.b = 0;
      }
      break;
    case 'g':
      if (color.g - 10 >= 0) {
        color.g -= 10;
      } else {
        color.g = 0;
      }
      break;
  }

  return color;
}

const uint8_t PROGMEM gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

#endif
