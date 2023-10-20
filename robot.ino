const int enA = 5;
const int enB = 6;
const int in1 = 9;
const int in2 = 8;
const int in3 = 10;
const int in4 = 11;

const int VEL = 100;

const int tilt = 5;
const int normal = 400;
const int forwardNormal = 300;

const int obtuse = 730;
const int forwardObtuse = 570;

char lastSt = 'b', st = 'b';

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
  analogWrite(enA,VEL);
  analogWrite(enB,VEL);
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
  analogWrite(enA,VEL);
  analogWrite(enB,VEL);
  delay(duration);
  stop(0);
}
void forward(int duration){
  digitalWrite(in1,0);
  digitalWrite(in2,1);
  digitalWrite(in3,0);
  digitalWrite(in4,1);
  
  analogWrite(enA,VEL);
  analogWrite(enB,VEL);
  
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
  //Mid White
  while(analogRead(A3) > module[3].mid)
  {
    if(analogRead(A1) > module[1].mid &&
       analogRead(A2) > module[2].mid&&
       analogRead(A3) > module[3].mid &&
       analogRead(A4) > module[4].mid)
    {
      backward(100);
    }
    //Tilt Left
    else if(analogRead(A1) <= module[1].mid ||
       analogRead(A2) <= module[2].mid)
    {
      left(tilt); 
    }
    else if(analogRead(A4) <= module[4].mid || 
            analogRead(A5) <= module[5].mid)
    {
      right(tilt); 
    }
  }
  
  //Mid Dark
  while(analogRead(A3) <= module[3].mid)
  {
    if(analogRead(A1) <= module[1].mid &&
       analogRead(A2) <= module[2].mid &&
       analogRead(A3) <= module[3].mid &&
       analogRead(A4) <= module[4].mid)
    {
      forward(forwardNormal); 
      right(normal); 
      forward(forwardNormal); 
    }
    
    //90 Degree Left
    if(analogRead(A1) <= module[1].mid &&
       		analogRead(A2) <= module[2].mid)
    {
      forward(forwardNormal); 
      left(normal);  
      forward(forwardNormal); 
    }
    else if(analogRead(A4) <= module[4].mid &&
       		analogRead(A5) <= module[5].mid)
    {
      forward(forwardNormal); 
      right(normal); 
      forward(forwardNormal); 
    }

    //315 Degree Left
    else if(analogRead(A1) <= module[1].mid)
    {
      forward(forwardObtuse); 
      left(obtuse); 
      forward(forwardObtuse); 
    }
    else if(analogRead(A5) <= module[5].mid)
    {
      forward(forwardObtuse); 
      right(obtuse); 
      forward(forwardObtuse); 
    }

    //Forward
    else
    {
      forward(100); //Less time = more frequent uppdate
    }
  }
}
