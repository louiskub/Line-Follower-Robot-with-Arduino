const int enA = 3;
const int enB = 5;
const int in1 = 9;
const int in2 = 8;
const int in3 = 10;
const int in4 = 11;
const int photo = A0;

const int VELA = 110;
const int VELB = VELA+10;
const int tilt = 2;
bool turn = false;

unsigned int waitTime = 0;

int val[6];

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
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,0);

  digitalWrite(in1,0);
  digitalWrite(in2,1);
  digitalWrite(in3,1);
  digitalWrite(in4,0);
  analogWrite(enA,VELA);
  analogWrite(enB,VELB);
  delay(duration);

  stop(0);
}
void right(int duration){
  digitalWrite(in1,0);
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,0);

  digitalWrite(in1,1);
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,1);
  analogWrite(enA,VELA);
  analogWrite(enB,VELB);
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
  delay(duration);
  stop(0);
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

  bool ST;
  bool lastST;
}
  //Check Dark : <=
  //Check Light : >=
void loop()
{
  val[1] = analogRead(A1);
  val[2] = analogRead(A2);
  val[3] = analogRead(A3);
  val[4] = analogRead(A4);
  val[5] = analogRead(A5);
  if(val[2] <= module[2].mid &&
       val[3] <= module[3].mid &&
       val[4] <= module[4].mid &&
       (val[1] <= module[1].mid ||
       val[5] <= module[5].mid))
  {
    while(analogRead(photo) > 300);
    waitTime = millis();
    while(millis()-waitTime < 2900 || !turn)
      if(analogRead(photo) > 300)
        while(millis()-waitTime < 2900 || !turn)
          if(analogRead(photo) < 300)
            turn = true;
    
    if(turn){ 
      while(analogRead(A3) <= module[3].mid)
        forward(tilt);
      forward(200);
      while(analogRead(A2) > module[2].mid || val[3] > module[3].mid)
        right(tilt);
    }
    else {
      while(analogRead(A3) <= module[3].mid)
        forward(tilt);
      forward(200);
      while(analogRead(A4) > module[4].mid || analogRead(A3) > module[3].mid)
        left(tilt);
    }
    turn = false;
  }else {
    if(val[3] <= module[3].mid){
      if(val[1] <= module[1].mid){
        while(analogRead(A2) <= module[2].mid)
          forward(tilt);
        forward(100);
        while(analogRead(A3) > module[3].mid)
          left(tilt);
      }
      else if(val[5] <= module[5].mid){
        while(analogRead(A4) <= module[4].mid)
          forward(tilt);
        forward(100);
        while(analogRead(A3) > module[3].mid)
          right(tilt);
      }

      else if(val[2] <= module[2].mid){
        left(tilt); 
        forward(tilt*2);
      }
      else if(val[4] <= module[4].mid){
        right(tilt);
        forward(tilt*2);
      }
      else 
        forward(tilt);
    }
    else {
      if(val[1] <= module[1].mid || val[2] <= module[2].mid)
        left(tilt); 
      else if(val[4] <= module[4].mid || val[5] <= module[5].mid)
        right(tilt); 
      else
        forward(tilt);
    }
  }
  
}

