#include <Servo.h>

int smaxFLpwm = 3;
int smaxFRpwm = 6; // 6
int smaxBLpwm = 5; // 5
int smaxBRpwm = 9;
double fl0, fr1, bl2, br3;
int fl0C, fr1C, bl2C, br3C;

int leftXChannel = 4;
int leftYChannel = 3;
int rightXChannel = 1;

int separator = 5000;

Servo smaxs[4];

int ppmPin = 2;
int channelAmount = 6;

unsigned long int current, past, interval;
int order[15], channel1, channel[6], channelInit[15];

long int kx, ky, kstrafe;

unsigned long int a, b, c;
int x[15], ch1[15], ch[7], i;

void mecanum() {
  kx = ch[leftXChannel];
  ky = ch[leftYChannel];
  kstrafe = ch[rightXChannel];

  // smaxs[0].write(ky + kx + kstrafe);
  // smaxs[1].write(ky - kx - kstrafe);
  // smaxs[2].write(ky - kx + kstrafe);
  // smaxs[3].write(ky + kx - kstrafe);
  // Serial.print("\t");Serial.print("\t");Serial.print("\t");
  Serial.print(ky);
  Serial.print("\t");
  Serial.print(kx);
  Serial.print("\t");
  Serial.print(kstrafe);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("\t");
  // Serial.println();

  if ((ky > 1100) || (ky < -100)) {
    Serial.println("ky");
    return;
  }
  if ((kx > 1100) || (kx < -100)) {
    Serial.println("kx");
    return;
  }
  if ((kstrafe > 1100) || (kstrafe < -100)) {
    Serial.println("kstrafe");
    return;
  }

  if ((ky > 450) && (ky < 550)) {
    ky = 500;
  }
  if ((kx > 450) && (kx < 550)) {
    kx = 500;
  }
  if ((kstrafe > 450) && (kstrafe < 550)) {
    kstrafe = 500;
  }

  // Serial.println("Hi");  

  fl0 = ky + kx + kstrafe; // 0 to 3000
  fr1 = ky - kx - kstrafe; // -2000 to 1000
  bl2 = ky - kx + kstrafe; // -1000 to 2000
  br3 = ky + kx - kstrafe; // -1000 to 2000

  fl0C = (int)((double) fl0 / 3.0 + 1000);
  fr1C = (int)(((double) fr1 + 2000) / 3.0 + 1000);
  // bl2C = (int)(((((double) bl2 + 1000) / 3.0) - 500) * 1.05 + 1500);
  bl2C = (int)((((double) bl2 + 1000) / 3.0) + 1000);
  br3C = (int)(((double) br3 + 1000) / 3.0 + 1000);
  // Serial.print(kx);Serial.print("\t\t");Serial.print(ky);Serial.print("\t\t");Serial.print(kstrafe);
  // Serial.print(fl0); Serial.print("\t\t"); Serial.print(fl0C); Serial.print("\t\t");

  // Serial.println("Test");

  Serial.print(fl0C);
  Serial.print("\t");
  Serial.print(fr1C);
  Serial.print("\t");
  Serial.print(bl2C);
  Serial.print("\t");
  Serial.print(br3C);
  Serial.print("\t");
  Serial.println();

  smaxs[0].write(fl0C);
  smaxs[1].write(fr1C);
  smaxs[2].write(bl2C);
  smaxs[3].write(br3C);
}

void setup() {
  Serial.begin(9600);
  pinMode(smaxFLpwm, OUTPUT);
  pinMode(smaxFRpwm, OUTPUT);
  pinMode(smaxBLpwm, OUTPUT);
  pinMode(smaxBRpwm, OUTPUT);
  smaxs[0].attach(smaxFLpwm);
  smaxs[1].attach(smaxFRpwm);
  smaxs[2].attach(smaxBLpwm);
  smaxs[3].attach(smaxBRpwm);

  pinMode(ppmPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), read_me, FALLING);
  // ESC1.attach(3);
  // ESC2.attach(5);
  // ESC3.attach(6);
  // ESC4.attach(9,1000,2000); // (pin min pulse width max pulse width) 
}

void loop() {
  read_rc();

  // Serial.print(ch[2]);Serial.print("\t");
  // Serial.print(ch[leftYChannel]);Serial.print("\t");
  // Serial.print(ch[leftXChannel]);Serial.print("\t");
  // Serial.print(ch[rightXChannel]);Serial.print("\t");

  // Serial.print(ch[5]);Serial.print("\t");
  // Serial.print(ch[6]);Serial.print("\t");
  // Serial.println();
  /*ESC1.write();
  ESC2.write();
  ESC3.write();
  ESC4.write();
  */
  mecanum();
  delay(20);
}

void read_me() {
  //this code reads value from RC reciever from PPM pin (Pin 2 or 3)
  //this code gives channel values from 0-1000 values 
  //    -: ABHILASH :-    //
  a = micros(); //store time value a when pin value falling
  c = a - b; //calculating time inbetween two peaks
  b = a; // 
  x[i] = c; //storing 15 value in array
  i = i + 1;
  if (i == 15) {
    for (int j = 0; j < 15; j++) {
      ch1[j] = x[j];
    }
    i = 0;
  }
} //copy store all values from temporary array another array after 15 reading  
void read_rc() {
  int i, j, k = 0;
  for (k = 14; k > -1; k--) {
    if (ch1[k] > separator) {
      j = k;
    }
  } //detecting separation space 10000us in that another array                     
  for (i = 1; i <= 6; i++) {
    ch[i] = (ch1[i + j] - 1000);
  }
} //assign 6 channel values after separation space