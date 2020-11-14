#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#include <TEA5767Radio.h>
Adafruit_MCP4725 dac;
#define defaultFreq 1700
#define f0 500
#define f1 750
#define f2 1000
#define f3 1250
const uint16_t S_DAC[4] = { 2000, 4000, 2000, 0};
char inData[55];
int delay0, delay1, delay2, delay3;
int cycleAll[4];
int delayAll[4];
TEA5767Radio radio = TEA5767Radio();
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

void sendpic(int numpic) {
  //openpic
  //getdata
  //send data
}

void takepic() {
  //start arduino 3 to take pic
}

int stobit(char x){
  int c = int(x);
  int tmp[12] = {0,0,0,0,0,0,0,0,0,0,0,0}
  for(int i=0;i<8;i++)
    {
        tmp[7-i] = c%2;
        c /= 2;
    }
}

void division(int temp[]) {
  for (int i = 0; i < 8; i++)
  {
    if (gen[0] == temp[i])
    {
      for (int j = 0, k = i; j < 5 + 1; j++, k++)
        if (!(temp[k]^gen[j]))
          temp[k] = 0;
        else
          temp[k] = 1;
    }
  }
}

void setup() {
  dac.begin(0x62); // set to default
  Serial.begin(115200);
  Wire.begin();
  radio.setFrequency(107.7);
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  for (int i = 1 ; i <= 4; i++) {
    cycleAll[i - 1] = i + 1;
    delayAll[i - 1] = 1000000 / 4 / ((i + 1) * 250);
    Serial.println(delayAll[i - 1]);
  }
  Serial.flush(); // for clear buffer serial
  dac.setVoltage(0, false);
}

void loop() {
  //x = getdatafrom pc 1
  if (x == 0) {
    //to control cam to take 3 pic
    takepic()
    status = true;
  }
  else {
    if (status) {
      sendpic(x)
    }
  }
}
