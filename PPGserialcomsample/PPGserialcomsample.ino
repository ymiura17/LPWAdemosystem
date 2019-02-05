void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
  delay(1000);
  Serial.write(0x02);
  Serial.write(0x35);
  Serial.write(0x48);
  Serial.write(0x52);
  Serial.write(0x5F);
  Serial.write(0x4D);
  Serial.write(0x6F);
  Serial.write(0x64);
  Serial.write(0x65);
  Serial.write(0x37);
  Serial.write(0x0D);
  Serial.write(0x0A);



  }



char buff[12];
int counter = 0;

void loop() {
//
//   int ppgout;
//  ppgout=Serial.read();
//  Serial.print(ppgout);
//  Serial.println("kore");
//  if(ppgout=="LR8Z36,ver_018"){
//    Serial.println("5HR_Mode7");
//  }

  if(Serial.available()>0){
    char val = Serial.read();
    buff[counter] = val;
    
    if (val == '\n'){
      Serial.write(buff);
      counter = 0;
    }
    else{
      counter++;
    }
  }







}
