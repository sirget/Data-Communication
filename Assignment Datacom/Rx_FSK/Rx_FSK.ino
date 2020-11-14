#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#include <TEA5767Radio.h>

TEA5767Radio radio = TEA5767Radio();
/* cbi this for increase analogRead Speed */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
#define r_slope 45
int min = 1023;
int max = 0;
boolean check = true;
boolean checkTime = true;
int countCycle = 0;
unsigned long long startTime, startTime2;
int prev = 0;
byte outPut = B00000000;
int countOutput = 0;
void setup() {
  Wire.begin();
  radio.setFrequency(107.7); // pick your own frequency
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Serial.begin(115200);

}

void loop() {
    if ((micros() - startTime2) > 18000 && checkTime == false) {
      countCycle = 0 ;
      //check = true;
      outPut = B00000000;
      countOutput = 0;
      checkTime = true;
    }
  if ((micros() - startTime) > 3500 && countCycle > 0) {
    if (countCycle == 2) {
      outPut >>= 2;
      outPut |= B00000000;
      countOutput++;
    }

    else if (countCycle == 3 ) {
      outPut >>= 2;
      outPut |= B01000000;
      countOutput++;
    }
    else if (countCycle == 4 ) {
      outPut >>= 2;
      outPut |= B10000000;
      countOutput++;
    }
    else if (countCycle == 5 ) {
      outPut >>= 2;
      outPut |= B11000000;
      countOutput++;
    }
    if (countOutput == 4) {
      Serial.print(char(outPut));
      outPut = B00000000;
      countOutput = 0;
      checkTime = true;
    }

    //check = true;
    countCycle = 0 ;

  }

  int tmp = analogRead(A0); // read signal from analog pin

  if (max - tmp > r_slope && check == false) {// falling signal
    min  = 1023;
    check = true; // change check status is true
  }
  if (tmp > max) { // update max value
    max = tmp;
  }
  if (tmp < min) { // update min value
    min = tmp;
  }
  if (tmp - min > r_slope) { // check for rasing signal
    if (check == true) {
      max = 0;
      countCycle++;
      if (countCycle == 1) {
        startTime = micros();
        if (checkTime) {
          startTime2 = micros();
          checkTime = false;

        }

      }

    }
    check = false; // change check status is false
  }
  prev = tmp; // assign temp value to previous

}
