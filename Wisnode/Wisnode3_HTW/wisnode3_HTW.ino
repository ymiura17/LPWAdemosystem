

#include "RAK811.h"
#include "SoftwareSerial.h"
#define WORK_MODE LoRaWAN   //  LoRaWAN or LoRaP2P
#define JOIN_MODE OTAA    //  OTAA or ABP
#if JOIN_MODE == OTAA
String DevEui = "XXXXXXXXXXXXXXXX";
String AppEui = "XXXXXXXXXXXXXXXX";
String AppKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
#else JOIN_MODE == ABP
String NwkSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
String AppSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
String DevAddr = "XXXXXXXX";
#endif
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define ATSerial Serial
SoftwareSerial DebugSerial(RXpin,TXpin);    // Declare a virtual serial port
char* buffer = "XXXXXXXXXXXXXXXXXXXXXX";

bool InitLoRaWAN(void);
RAK811 RAKLoRa(ATSerial,DebugSerial);


void setup() {
 DebugSerial.begin(9600);
 while(DebugSerial.read()>= 0) {}
 while(!DebugSerial);
 DebugSerial.println("StartUP");

 ATSerial.begin(9600); // Note: Please manually set the baud rate of the WisNode device to 9600.
 delay(100);
 DebugSerial.println(RAKLoRa.rk_getVersion());
 delay(200);
 DebugSerial.println(RAKLoRa.rk_getBand());
 delay(200);

 while (!InitLoRaWAN());
 
}

bool InitLoRaWAN(void)
{
  if (RAKLoRa.rk_setWorkingMode(WORK_MODE))
  {
    if (RAKLoRa.rk_initOTAA(DevEui, AppEui, AppKey))
    {
      DebugSerial.println("You init OTAA parameter is OK!");
      while (RAKLoRa.rk_joinLoRaNetwork(JOIN_MODE))
      {
        bool flag = false;
        for (unsigned long start = millis(); millis() - start < 90000L;)
        {
          String ret = RAKLoRa.rk_recvData();
          if (ret.startsWith(STATUS_JOINED_SUCCESS))
          {
            DebugSerial.println("You join Network success!");
            return true;
          }
          else if (ret.startsWith(STATUS_RX2_TIMEOUT) || ret.startsWith(STATUS_JOINED_FAILED))
          {
            DebugSerial.println("You join Network Fail!");
            flag = true;
            DebugSerial.println("The device will try to join again after 5s");
            delay(5000);
          }
        }
        if (flag == false)
        {
          DebugSerial.println("Pleases Reset the module!");
          delay(1000);
          return false;
        }
      }
    }
  }
  delay(1000);
  return false;
}

void loop() {

  unsigned long t , t2;
  unsigned long t3 = 0;
  int ans , val;
  int ans2 = 0;
  int ans3 = 0;
  int ans4 = 0;
  int val2 = 0;
  int i;
  int sensorValue;

  // put your setup code here, to run once:
  tone(7,56000);
  delayMicroseconds(30);
  noTone(7);
  t = micros();

//////Ultrasonic part////////
  while(1){
     val = analogRead(A0);
     t2 = micros() - t ;
     if (val > val2){
      val2 = val;
      t3 = t2;
     }
     if (t2 >= 19000) break;
  }
    t = 331500 + (600*25) ;
    t = (t * (t3-300)) / 1000000;
    ans = t / 2;
    delay(200);
    for (byte i = 0 ; i < 50 ; i++)
    {
      ans2 += ans;
    }
    ans3 = ans3 * 0.9 +  (ans2 / 50) * 0.1;
        
   ans3 = 39 - ans3 + 0;

    if (ans3>0){
      Serial.println(ans3);
    }



//////Communication part///////
  String stringOne = String(ans3,HEX);
  if(17 > ans3){
    stringOne = "0265000"+stringOne;
  }

  if(257 > ans3 && ans3 >16){
    stringOne = "026500" +stringOne;
  }
  char luminosity[22];
  stringOne.toCharArray(luminosity,22);

  int packetsflag = 1; // 0: unconfirmed packets, 1: confirmed packets
  if (RAKLoRa.rk_sendData(packetsflag, 2, luminosity))
  {
    for (unsigned long start = millis(); millis() - start < 90000L;)
    {
      String ret = RAKLoRa.rk_recvData();
      if (ret.startsWith(STATUS_TX_COMFIRMED) || ret.startsWith(STATUS_TX_UNCOMFIRMED))
      {
        DebugSerial.println("Send data ok!");
        delay(5000);
        return;
      }
    }
    DebugSerial.println("Send data error!");
      asm volatile (" jmp 0");
    while (1);
  }
}
