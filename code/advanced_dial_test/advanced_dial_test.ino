void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
}
double t=0;
bool s=false;
int k=0;
double last=0;
double lastlast=0;
double ar[100]={0};
int len=1;
void loop() {
  if(!s && digitalRead(3))
  {
      s=true;
      t=micros();
  }
  if(s && !digitalRead(3))
  {
    s=false;
    ar[len]=micros()-t;
    len++;
  }

  if(!s && micros()-t>1000000 && len>1)
  {
    k=0;
    while(Serial.available()) Serial.read();
    for(int i=1; i<len-1; i++)
      if (ar[i]>ar[i-1] && ar[i]>ar[i+1] && ar[i]>=11000)
        {
          Serial.println(ar[i]);
          k++;
        }
    
    len=1;
    Serial.println(k);
  }
}
