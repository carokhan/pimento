
#include <Servo.h>

int smaxFLpwm = 3;
int smaxFRpwm = 5;
int smaxBLpwm = 6;
int smaxBRpwm = 9;

int leftXChannel = 1;
int leftYChannel = 2;
int rightXChannel = 3;

Servo smaxs[4];

int ppmPin = 2;
int channelAmount = 6;

unsigned long int current, past, interval;
int order[15], channel1, channel[6], channelInit[15];  

int separator = 5000;

unsigned long int kx, ky, kstrafe;  

unsigned long int a,b,c;
int x[15],ch1[15],ch[7],i;

void mecanum() {
  kx = ch[leftXChannel] + 1000;
  ky = (ch[leftYChannel] * 1) + 1000;
  kstrafe = ch[rightXChannel] + 1000;

  // smaxs[0].write(ky + kx + kstrafe);
  // smaxs[1].write(ky - kx - kstrafe);
  // smaxs[2].write(ky - kx + kstrafe);
  // smaxs[3].write(ky + kx - kstrafe);

  Serial.println(ky);
  Serial.println(kx);
  Serial.println(kstrafe);

  Serial.println("Test");
  smaxs[0].write((ky + kx + kstrafe + 3000)/9 + 1000);
  smaxs[1].write((ky - kx - kstrafe + 3000)/9 + 1000);
  smaxs[2].write((ky - kx + kstrafe + 3000)/9 + 1000);
  smaxs[3].write((ky + kx - kstrafe + 3000)/9 + 1000);
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

// Serial.print(ch[1]);Serial.print("\t");
// Serial.print(ch[2]);Serial.print("\t");
// Serial.print(ch[3]);Serial.print("\t");
// Serial.print(ch[4]);Serial.print("\t");
// Serial.print(ch[5]);Serial.print("\t");
// Serial.print(ch[6]);Serial.print("\n");
/*ESC1.write();
ESC2.write();
ESC3.write();
ESC4.write();
*/
mecanum();
delay(20);
}


void read_me()  {
 //this code reads value from RC reciever from PPM pin (Pin 2 or 3)
 //this code gives channel values from 0-1000 values 
 //    -: ABHILASH :-    //
a=micros(); //store time value a when pin value falling
c=a-b;      //calculating time inbetween two peaks
b=a;        // 
x[i]=c;     //storing 15 value in array
i=i+1;       if(i==15){for(int j=0;j<15;j++) {ch1[j]=x[j];}
             i=0;}}//copy store all values from temporary array another array after 15 reading  
void read_rc(){
int i,j,k=0;
  for(k=14;k>-1;k--){if(ch1[k]>10000){j=k;}}  //detecting separation space 10000us in that another array                     
  for(i=1;i<=6;i++){ch[i]=(ch1[i+j]-1000);}}     //assign 6 channel values after separation space