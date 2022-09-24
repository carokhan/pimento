#include <Servo.h>

int neoFrontLeft = 3;
int neoFrontRight = 6;
int neoBackLeft = 5;
int neoBackRight = 9;
double frontLeft0, frontRight1, backLeft2, backRight3;
int frontLeft0scaled, frontRight1scaled, backLeft2scaled, backRight3scaled,
    frontLeft0scaledRev, backLeft2scaledRev;

int leftXChannel = 4;
int leftYChannel = 3;
int rightXChannel = 1;

int separator = 5000;

Servo neos[4];

int ppmPin = 2;
int channelAmount = 6;

long int kx, ky, kstrafe;

unsigned long int current, past, interval;
int peaks[15], frame[15], channel[7], channelNum;

void mecanum() {
  kx = channel[leftXChannel];
  ky = channel[leftYChannel];
  kstrafe = channel[rightXChannel];

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

  frontLeft0 = ky + kx + kstrafe;   // 0 to 3000
  frontRight1 = ky - kx - kstrafe;  // -2000 to 1000
  backLeft2 = ky - kx + kstrafe;    // -1000 to 2000
  backRight3 = ky + kx - kstrafe;   // -1000 to 2000

  frontLeft0scaled = (int)((double)frontLeft0 / 3.0 + 1000);
  frontLeft0scaledRev = ((frontLeft0scaled - 1500) * -1) + 1500;
  frontRight1scaled = (int)(((double)frontRight1 + 2000) / 3.0 + 1000);
  backLeft2scaled = (int)((((double)backLeft2 + 1000) / 3.0) + 1000);
  backLeft2scaledRev = ((backLeft2scaled - 1500) * -1) + 1500;
  backRight3scaled = (int)(((double)backRight3 + 1000) / 3.0 + 1000);

  Serial.print(frontLeft0scaledRev);
  Serial.print("\t");
  Serial.print(frontRight1scaled);
  Serial.print("\t");
  Serial.print(backLeft2scaledRev);
  Serial.print("\t");
  Serial.print(backRight3scaled);
  Serial.print("\t");
  Serial.println();

  neos[0].write(frontLeft0scaledRev);
  neos[1].write(frontRight1scaled);
  neos[2].write(backLeft2scaledRev);
  neos[3].write(backRight3scaled);
}

void setup() {
  Serial.begin(9600);
  pinMode(neoFrontLeft, OUTPUT);
  pinMode(neoFrontRight, OUTPUT);
  pinMode(neoBackLeft, OUTPUT);
  pinMode(neoBackRight, OUTPUT);
  neos[0].attach(neoFrontLeft);
  neos[1].attach(neoFrontRight);
  neos[2].attach(neoBackLeft);
  neos[3].attach(neoBackRight);

  pinMode(ppmPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), ppmSetup, FALLING);
}

void loop() {
  ppmRead();
  mecanum();
  delay(20);
}

void ppmSetup() {
  current = micros();
  interval = current - past;
  past = current;
  peaks[channelNum] = interval;
  channelNum = channelNum + 1;
  if (channelNum == 15) {
    for (int i = 0; i < 15; i++) {
      frame[i] = peaks[i];
    }
    channelNum = 0;
  }
}

void ppmRead() {
  int i, activeChannel, ppmChannel = 0;
  for (ppmChannel = 14; ppmChannel > -1; ppmChannel--) {
    if (frame[ppmChannel] > separator) {
      activeChannel = ppmChannel;
    }
  }
  for (i = 1; i <= channelAmount; i++) {
    channel[i] = (frame[i + activeChannel] - 1000);
  }
}