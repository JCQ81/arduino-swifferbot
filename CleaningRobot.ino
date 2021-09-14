
#define pUSLecho 11
#define pUSLtrig 13
#define pUSRecho 10
#define pUSRtrig 12

#define pMTLfwd 3
#define pMTLrev 5
#define pMTRfwd 9
#define pMTRrev 6

#define MDLY  60    // Delay between motor actions
#define TDLY  500   // Delay (time) when turning around
#define SDLY  400   // Delay on stop (not brake)
#define MSPD  120   // Motor speed (PWM)
#define MDIST 25    // Turn at (distance to object)
#define MLOOP 160   // Maximum number of "loops" before turning by itself

//#define DEBUG

#define FWD   1
#define REV   2
#define BRK   3
#define STOP  4
#define TURNL 5
#define TURNR 6

void move(byte action) {
  digitalWrite(pMTLfwd, LOW);
  digitalWrite(pMTLrev, LOW);
  digitalWrite(pMTRfwd, LOW);
  digitalWrite(pMTRrev, LOW);
  delay(MDLY);
  if (action == STOP) 
    delay(SDLY);  
  if (action == FWD) {
    analogWrite(pMTLfwd, MSPD);
    delay(MDLY);
    analogWrite(pMTRfwd, MSPD);
    delay(MDLY);
  }
  if (action == REV) {
    analogWrite(pMTLrev, MSPD);
    delay(MDLY);
    analogWrite(pMTRrev, MSPD);
    delay(MDLY);
  }
  if (action == BRK) {
    analogWrite(pMTLfwd, MSPD);
    analogWrite(pMTLrev, MSPD);
    analogWrite(pMTRfwd, MSPD);
    analogWrite(pMTRrev, MSPD);
    delay(MDLY);
    digitalWrite(pMTLfwd, LOW);
    digitalWrite(pMTLrev, LOW);
    digitalWrite(pMTRfwd, LOW);
    digitalWrite(pMTRrev, LOW);
  }
  if (action == TURNL) {
    analogWrite(pMTLfwd, MSPD);
    delay(MDLY);
    analogWrite(pMTRrev, MSPD);
    delay(MDLY);
  }
  if (action == TURNR) {
    analogWrite(pMTLrev, MSPD);
    delay(MDLY);
    analogWrite(pMTRfwd, MSPD);
    delay(MDLY);
  }
    
}

int distance(byte pTrig, byte pEcho) {
  long duration;
  digitalWrite(pTrig, LOW);
  delayMicroseconds(5);  
  digitalWrite(pTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pTrig, LOW);  
  return pulseIn(pEcho, HIGH) * 0.017;
}

int distL;
int distR;
int lpcnt;
bool fwd;

void setup() {
  pinMode(pUSLtrig,OUTPUT);
  pinMode(pUSLecho,INPUT);  
  pinMode(pUSRtrig,OUTPUT);
  pinMode(pUSRecho,INPUT);  
  pinMode(pMTLfwd,OUTPUT);
  pinMode(pMTLrev,OUTPUT);
  pinMode(pMTRfwd,OUTPUT);
  pinMode(pMTRrev,OUTPUT);
  digitalWrite(pMTLfwd, LOW);
  digitalWrite(pMTLrev, LOW);
  digitalWrite(pMTRfwd, LOW);
  digitalWrite(pMTRrev, LOW);  
  lpcnt = 0;   
  fwd = false; 

  #ifdef DEBUG
    Serial.begin(9600);
    while (true) {
      Serial.print(distance(pUSLtrig,pUSLecho));
      Serial.print(" - ");
      Serial.println(distance(pUSRtrig,pUSRecho));
      delay(200);
    }
  #endif
}

void loop() {  
  distL = distance(pUSLtrig,pUSLecho);
  distR = distance(pUSRtrig,pUSRecho);  
  
  if (lpcnt > MLOOP) 
  {
    move(STOP);
    delay(1200);
    move(REV);
    delay(1000);
    move(TURNL);
    delay(TDLY);    
    lpcnt = 0;
    fwd = false;
  }

  if (distL < MDIST) {
    move(TURNR);
    delay(TDLY);
    move(STOP);    
    //lpcnt = MLOOP / 2;
    fwd = false;
  }
  else if (distR < MDIST) {
    move(TURNL);
    delay(TDLY);
    move(STOP);
    //lpcnt = MLOOP / 2;
    fwd = false;
  }
      
  if (!fwd) {
    move(FWD);  
    fwd = true;
  }

  lpcnt++;
  delay(100);  
}
