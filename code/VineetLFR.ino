# define numSen 6

# define sensorPin_0  A0  // select the input pin for the sensor read 
# define sensorPin_1  A1    // A0-A5 : Analog pins available on Arduino Board. 
# define sensorPin_2  A2
# define sensorPin_3  A3
# define sensorPin_4  A4
# define sensorPin_5  A5
# define leftforward 5
# define leftbackward 6
# define rightforward 9
# define rightbackward 10
# define motordriver 13
# define kp  1.5
# define kd  0
# define defaultSpeed 120
int mid1 =numSen/2;
int mid2 =mid1-1;
int lastErr = 0, derv = 0, prop = 0;
int lastMillis = 0;
int leftM = 0;
int rightM = 0;
int a[numSen];
int w[numSen];

void setup()
{
  Serial.begin(9600);
  pinMode(sensorPin_0, INPUT);  //Initializing pin as input from sensor
  pinMode(sensorPin_1, INPUT);
  pinMode(sensorPin_2, INPUT);
  pinMode(sensorPin_3, INPUT);
  pinMode(sensorPin_4, INPUT);
  pinMode(sensorPin_5, INPUT);
  pinMode(leftbackward, OUTPUT);
  pinMode(leftforward, OUTPUT);
  pinMode(rightforward, OUTPUT);
  pinMode(rightbackward, OUTPUT);
  pinMode(motordriver, OUTPUT);

  digitalWrite(motordriver, HIGH);
  initialize();
}

void loop() {

  reading();
  p=priority();
  if(p==0)
  leftTurn();
  else if(p==1)
  rightTurn();
  
  printValue();

  pid();
}
void priority()
{
  int p=2;//2 for PID
  if(a[0]<100)
  {
    p=0;//left
    motors(0,0,0,0);//break
  }
  else if((a[mid1]<100||a[mid2]<100)&&(a[numSen-1]<100))
  {
    p=1;//right
    motors(0,0,0,0);//break
  }
  return p;
}
void printValue()
{
  int i, j;
  reading();
  for (j = 0; j < 2; j++)
  {
    reading();

    for (i = 0; i < numSen; i++)
    {
      Serial.print(a[i]);
      Serial.print(" ");

    }
    Serial.println();
  }
}

void pid()
{
  int pos = mean();
  int error = pos - 1166;

  prop = error * kp;
  derv = (lastErr - error) * kd;

  int pidValue = prop + derv;

  lastErr = error;

  leftM = defaultSpeed + pidValue;
  rightM = defaultSpeed - pidValue;
  leftM = constrain(leftM, 0, 255);
  rightM = constrain(rightM, 0, 255);
  motors(leftM, 0, rightM, 0);
}


void pidt()
{
  int t = millis() - lastMillis;
  int pos = mean();
  int error = pos - 1166;

  prop = error * kp;
  derv = ((lastErr - error) * kd) / t;

  int pidValue = prop + derv;
  lastErr = error;
  leftM = defaultSpeed + pidValue;
  rightM = defaultSpeed - pidValue;

  motors(leftM, 0, rightM, 0);
  lastMillis = millis();
}

void reading()
{
  a[0] = analogRead(sensorPin_0);
  a[1] = analogRead(sensorPin_1);
  a[2] = analogRead(sensorPin_2);
  a[3] = analogRead(sensorPin_3);
  a[4] = analogRead(sensorPin_4);
  a[5] = analogRead(sensorPin_5);

}

void initialize()
{
  for (int i = 0; i < 6; i++)
  {
    w[i] = (i + 1) * 1000;
  }
}

int mean()
{
  reading();
  int s = 0;
  int as=0;
  for (int i = 0; i < 6; i++)
    {s = s + (a[i] * w[i]);
    as=as+s;
    }
  s = s / as;
  return s;
}

void motors(int a, int b, int c, int d)
{
  analogWrite(leftforward, a);
  analogWrite( leftbackward , b);
  analogWrite (rightforward , c);
  analogWrite ( rightbackward , d);
}
void leftTurn()
{
  while(a[0]<100)
  {
    analogWrite(leftforward, 60);
    analogWrite( leftbackward , 0);
    analogWrite (rightforward , 180);
    analogWrite ( rightbackward , 0);
  
  }
  pid();
}
void rightTurn()
{
  while(a[numSen-1]<100)
  {
    analogWrite(leftforward, 180);
    analogWrite( leftbackward , 0);
    analogWrite (rightforward , 60);
    analogWrite ( rightbackward , 0);
  
  }
  pid();
}
