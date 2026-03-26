void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);//pin 3,5,6,9,10,11 supports PWM (analog power)
  pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  breathe();
}

void breathe()
{
  for(int i = 0;i < 255;i++){
    analogWrite(6,i);
    analogWrite(5,255 - i);
    delay(10);
  }

  for(int i = 255;i > 0;i--){
    analogWrite(6,i);
    analogWrite(5,255 - i);
    delay(10);
  }

}
