#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#include <TEA5767Radio.h>

Adafruit_MCP4725 dac;

TEA5767Radio radio = TEA5767Radio();

#define defaultFreq 1700
#define f0 500
#define f1 750
#define f2 1000
#define f3 1250

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
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

const uint16_t S_DAC[4] = { 2000, 4000, 2000, 0};
char inData;
int delay0, delay1, delay2, delay3;
int cycleAll[4];
int delayAll[4];

int window[12], w = 0;
int temp[12];
int data[16], d = 0;
int gen[5] = [1, 0, 0, 1, 1], g = 5;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  radio.setFrequency(107.7);
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;

  dac.begin(0x62);
  for (int i = 1 ; i <= 4; i++) {
    cycleAll[i - 1] = i + 1;
    delayAll[i - 1] = 1000000 / 4 / ((i + 1) * 250);
  }
  Serial.flush();
  dac.setVoltage(0, false);

  Serial.println("Start");
}

void division(int temp[], int gen[], int n, int r) {
  for (int i = 0; i < n; i++) {
    if (gen[0] == temp[i]) {
      for (int j = 0, k = i; j < r + 1; j++, k++) {
        if (!(temp[k]^gen[j]))
          temp[k] = 0;
        else
          temp[k] = 1;
      }
    }
  }
}

void sentData(inData) {
  Serial.print(inData);
  for (int k = 7; k >= 0; k -= 2) {
    int tmp = inData & 3;
    for (int s = 0; s < cycleAll[tmp]; s++ ) {
      for (int sl = 0; sl < 4; sl++ ) {
        dac.setVoltage(S_DAC[sl], false);
        delayMicroseconds(delayAll[tmp] - 140);
      }
    }
    inData >>= 2;
  }
  dac.setVoltage(0, false);
}

void loop() {
  if (Serial.available() > 0) {
    sentData(Serial.read());
  }
  else {
    if (w < 12) {
      if ((micros() - startTime2) > 18000 && checkTime == false) {
        countCycle = 0 ;
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
          window[w] = output;
          w++;
          outPut = B00000000;
          countOutput = 0;
          checkTime = true;
        }
        countCycle = 0 ;
      }
      int tmp = analogRead(A0);

      if (max - tmp > r_slope && check == false) {
        min  = 1023;
        check = true;
      }
      if (tmp > max) {
        max = tmp;
      }
      if (tmp < min) {
        min = tmp;
      }
      if (tmp - min > r_slope) {
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
        check = false;
      }
      prev = tmp;
    }
    else if (w == 12) {
      w = 0;
      for (int i = 0; i < 12; i++) {
        temp[i] = window[i];
        division(temp, gen, 8, 5);
        for (int i = 0; i < 5; i++) {
          if (temp[8 + i]) {
            Serial.println("Error detected in received message.");
            sentData("E");
          }
          else {
            Serial.println("No error in received Message.\nReceived Message : ");
            for (int i = 0; i < 8; i++) {
              cout << window[i] << " ";
              data[d] = window[i];
              d++;
            }
            sentData("A");
          }
        }
      }
    }
    else if (d == 16) {
      d = 0;
      Serial.print("RealData : ");
      for (int i = 0; i < 16; i++) {
        cout << data[i] << " ";
      }
    }
  }
}
