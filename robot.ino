const int enA = 3;
const int enB = 5;
const int in1 = 9;
const int in2 = 8;
const int in3 = 10;
const int in4 = 11;
const int photo = A0;
int VELA = 110;
int VELB = VELA+10;
const int tilt = 20;

bool turn = 0;
int foundDark = 1;
unsigned int waitTime;
unsigned int whiteTimer;
unsigned int darkTimer;

int intersecttimer;
int whiteinterval = 600;

bool flashdetected = false;
unsigned long flashstarttime = 0;

struct threshold{
  int dark,light,mid;
}module[6];

void stop(int duration){
  digitalWrite(in1,0);
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,0);
  delay(duration);
}
void left(int duration){
  digitalWrite(in1,0);
  digitalWrite(in2,1);
  digitalWrite(in3,1);
  digitalWrite(in4,0);
  analogWrite(enA,100);
  analogWrite(enB,110);
  delay(duration);

  stop(0);
}
void right(int duration){
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,1);
  analogWrite(enA,100);
  analogWrite(enB,110);
  delay(duration);
  stop(0);
}
void forward(int duration){
  digitalWrite(in1,0);
  digitalWrite(in2,1);
  digitalWrite(in3,0);
  digitalWrite(in4,1);

  analogWrite(enA,VELA);
  analogWrite(enB,VELB);
  
  delay(duration);
  stop(0);
}
void backward(int duration){
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  digitalWrite(in3,1);
  digitalWrite(in4,0);
  analogWrite(enA,VELA);
  analogWrite(enB,VELB);
  delay(duration);
  stop(0);
}
bool checkWhite(){
  return 
       analogRead(A2) > module[2].mid &&
       analogRead(A3) > module[3].mid &&
       analogRead(A4) > module[4].mid &&
       analogRead(A1) > module[1].mid &&
       analogRead(A5) > module[5].mid;
}
bool checkDark(){
  return 
       analogRead(A2) <= module[2].mid &&
       analogRead(A3) <= module[3].mid &&
       analogRead(A4) <= module[4].mid &&
       (analogRead(A1) <= module[1].mid ||
       analogRead(A5) <= module[5].mid);
}
bool checkAllDark(){
  return 
       analogRead(A2) <= module[2].mid &&
       analogRead(A3) <= module[3].mid &&
       analogRead(A4) <= module[4].mid &&
       analogRead(A1) <= module[1].mid &&
       analogRead(A5) <= module[5].mid;
}
void setup(){
  Serial.begin(9600);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  module[1].light = 1000;
  module[1].dark = 560;
  module[2].light = 1000;
  module[2].dark = 560;
  module[3].light = 1000;
  module[3].dark = 510;
  module[4].light = 1000;
  module[4].dark = 540;
  module[5].light = 1000;
  module[5].dark = 200;

  for(int i=1 ; i<=5 ; i++){
    module[i].mid = (module[i].dark + module[i].light)/2;
  }
}
  //Check Dark : <=
  //Check Light : >=
void loop()
{
  if((checkDark() && foundDark%2 == 0) || (checkAllDark() && foundDark%2 == 1)){
    darkTimer = millis();
    while(analogRead(photo) > 300);
    foundDark++;
    waitTime = millis();
    while(millis()-waitTime < 4000)
      if(analogRead(photo) > 300)
        while(millis()-waitTime < 4000)
          if(analogRead(photo) <= 300)
            turn = true;
    if(turn){
      forward(300);
      right(400);
      while(analogRead(A3) > module[3].mid && analogRead(A2) > module[4].mid)
        right(tilt);
    }
    else{
      forward(300);
      left(400);
      while(analogRead(A3) > module[3].mid && analogRead(A4) > module[4].mid)
        left(tilt);
    }
    turn = false;
    whiteTimer = millis();
  }
  
  else 
  {
    if(analogRead(A3) <= module[3].mid && analogRead(A5) <= module[5].mid){
      intersecttimer = millis();
      while(millis() - intersecttimer <= 400 && !((checkDark() && foundDark%2 == 0) || (checkAllDark() && foundDark%2 == 1)))
        forward(1);
      while(analogRead(A3) > module[3].mid && analogRead(A2) > module[2].mid){
        whiteTimer = millis();
        right(tilt);
      }
    }
    else if(analogRead(A3) <= module[3].mid && analogRead(A1) <= module[1].mid){
      intersecttimer = millis();
      while(millis() - intersecttimer <= 400 && !((checkDark() && foundDark%2 == 0) || (checkAllDark() && foundDark%2 == 1)))
        forward(1);
      while(analogRead(A3) > module[3].mid && analogRead(A4) > module[4].mid){
        whiteTimer = millis();
        left(tilt);
      }
    }

    if(analogRead(A2) > module[2].mid && analogRead(A4) > module[4].mid) //Mid Dark
    {
      whiteTimer = millis();
      forward(tilt);      
    }
    else if(analogRead(A1) <= module[1].mid){
      whiteTimer = millis();
      left(tilt*2);
    }
    else if(analogRead(A5) <= module[5].mid){
      whiteTimer = millis();
      right(tilt*2);
    }
    else if(analogRead(A2) <= module[2].mid){
      whiteTimer = millis();
      left(tilt);
      forward(tilt);
    }
    else if(analogRead(A4) <= module[4].mid){
      whiteTimer = millis();
      right(tilt);
      forward(tilt);
    }
  }
}
