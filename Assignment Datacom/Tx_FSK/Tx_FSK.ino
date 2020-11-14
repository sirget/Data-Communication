#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
#define defaultFreq 1700 //DAC speed (Hz)
/*freq0 : frequency of carrier sine wave (Hz)*/
#define f0 500 // FSK f0
#define f1 750 // FSK f1
#define f2 1000 // FSK f2
#define f3 1250 // ASK f3
/*A[0]-A[3] : ASK Amplitude (0,5] (V)*/
const uint16_t S_DAC[4] = { 2000, 4000, 2000, 0};
char inData[55];
int delay0, delay1, delay2, delay3;
int cycleAll[4];
int delayAll[4];
void setup() {
  dac.begin(0x62); // set to default
  Serial.begin(115200);
  for (int i = 1 ; i <= 4; i++) {
    cycleAll[i - 1] = i + 1;
    delayAll[i - 1] = 1000000 / 4 / ((i + 1) * 250);
    Serial.println(delayAll[i - 1]);
  }
  //  delay0 = 1000000 / 500 / 4; //sampling period for FSK 500 Hz
  //  delay1 = 1000000 / 750 / 4; //sampling period for FSK 750 Hz
  //  delay2 = 1000000 / 1000 / 4; //sampling period for FSK 1000 Hz
  //  delay3 = 1000000 / 1250 / 4; //sampling period for FSK 1250 Hz
  //  // (Tsine – delayfrom DAC processing speed)
  //  delayAll[]={delay0,delay1,delay2,delay3};
  //  cycleAll[]={2,3,4,5};
  Serial.flush(); // for clear buffer serial
  dac.setVoltage(0, false);
}

void loop() {
  if (Serial.available() > 0) {
    //if (true) {
    /*use a cycle loop receive inData : message input */
    // for (int i = 0; i == 0 || inData[i - 1] != 0xFFFFFFFF; i++) {
    inData[0] = Serial.read(); // Read a character
    // }
    //inData[0] = 'c';
    /*use a cycle loop i for send data 8 bits*/
    // for (int i = 0; inData[i] != 0xFFFFFFFF; i++ ) {
    Serial.print(inData[0]);
    /*
      use a cycle loop k for 1 ASK signal element (2 bit)
      - map inData[i] to tmp (2 bit)
      - from LSB to MSB
    */
    for (int k = 7; k >= 0; k -= 2) {
      int tmp = inData[0] & 3; // 00, 01, 10, 11
      //Serial.println(tmp);
      /*use a cycle loop sl to send 5 cycle/baud*/
      for (int s = 0; s < cycleAll[tmp]; s++ ) {
        /*use a cycle loop s to send 4 sample/cycle*/
        for (int sl = 0; sl < 4; sl++ ) {
          /*
            Use the selected amplitude above to modify
            sine amplitude
          */
          dac.setVoltage(S_DAC[sl], false);
          delayMicroseconds(delayAll[tmp] - 140); // sampling delay
        }
      }
      inData[0] >>= 2;
    }
    //}
    dac.setVoltage(0, false); // for don't send
  }

}
