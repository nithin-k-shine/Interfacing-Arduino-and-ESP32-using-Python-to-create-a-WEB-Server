/* This file is meant to be used with the FLOSS-Arduino toolbox*/

#include <LiquidCrystal.h>//For LCD display
#include <SoftwareSerial.h>//To create additional Serial port

void setup() {
  /* initialize serial                                       */
  Serial.begin(115200);
}

void loop() {
  
  /* variables declaration and initialization                */
  int  val =  0;           /* generic value read from serial */
  int  agv =  0;           /* generic analog value           */
  int  dgv =  0;           /* generic analog value           */
  int pin;                 /* pin number                     */
  int rs,en,d4,d5,d6,d7;   /* pins used for LCD display      */
  int rx,tx;               /* pins for serial communication with ESP32*/
  while (Serial.available()==0) {}; // Waiting char
  val = Serial.read(); 
  
  //Checking for availability of firmware
  if(val==118)
  {
    delay(10);
    Serial.write("ok"); 
  }

  //case A -> Analog
  else if (val==65){//A -> Analog read
    while (Serial.available()==0) {}; // Waiting char
    val=Serial.read();
    
    if (val>47 && val<67) { //from pin 0, to pin 19
      pin=val-48; //number of the pin
      agv=analogRead(pin);
      //Serial.println(agv);
      Serial.write((uint8_t*)&agv,2); /* send binary value via serial  */   
    }
       val=-1;
  }
  else if (val==87){//W -> Analog write
      while (Serial.available()==0) {}; // Waiting char
      val=Serial.read();
      if (val>47 && val<67) { //from pin 0 to pin 19
        pin=val-48; //number of the pin
        while (Serial.available()==0) {}; // Waiting char
        val=Serial.read();
        analogWrite(pin,val);
         }
          val=-1;
  }

  //case D -> Digital
  else if (val==68){//D -> Digital pins
    while (Serial.available()==0) {}; // Waiting char
    val=Serial.read();
    if (val==97){ //'a'-> declare pin
       while (Serial.available()==0) {}; // Waiting char
       val=Serial.read();
       if (val>49 && val<102) {
         pin=val-48;
         while (Serial.available()==0) {}; // Waiting char
         val=Serial.read();
         if (val==48 || val==49) { 
            if (val==48)pinMode(pin,INPUT);//'0' -> input
            else if (val==49)pinMode(pin,OUTPUT);//'1' -> output
         }
       }
    }
    if (val==114){ //'r'-> read pin
       while (Serial.available()==0) {}; // Waiting char
       val=Serial.read();
       if (val>49 && val<102) { 
          pin=val-48; //number of the digital pin
          dgv=digitalRead(pin);         
          Serial.print(dgv);         
       }
    }
    if (val==119){ //'w'-> write pin
       while (Serial.available()==0) {}; // Waiting char
       val=Serial.read();
       if (val>49 && val<102) { 
          pin=val-48; //number of the digital pin
          while (Serial.available()==0) {}; // Waiting char
          val=Serial.read();
          if (val==48 || val==49) { // 0 or 1
            dgv=val-48;
            digitalWrite(pin,dgv);   
          }
       }
    }
   val=-1;
  }
  
  //case L -> LCD display
  else if (val==76){//L -> LCD Display
    while (Serial.available()==0) {}; // Waiting char
    rs=Serial.read()-48;//Read/Write pin
    while (Serial.available()==0) {}; // Waiting char
    en=Serial.read()-48;//Enable pin
    while (Serial.available()==0) {}; // Waiting char
    d4=Serial.read()-48;//D4 pin
    while (Serial.available()==0) {}; // Waiting char
    d5=Serial.read()-48;//D5 pin
    while (Serial.available()==0) {}; // Waiting char
    d6=Serial.read()-48;//D6 pin
    while (Serial.available()==0) {}; // Waiting char
    d7=Serial.read()-48;//D7 pin
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Declare a LiquidCrystal object
    lcd.begin(16, 2);                          //Set up the LCD's number of columns and rows
    while (Serial.available()==0) {};          // Waiting char
    lcd.print(Serial.readString());            //Print a message to the LCD
    val=-1;
  }
  
  //case E ->Sending to ESP
  else if(val==69){
    while (Serial.available()==0) {}; // Waiting char
    rx=Serial.read()-48;              //rx pin
    while (Serial.available()==0) {}; // Waiting char
    tx=Serial.read()-48;              //tx pin
    SoftwareSerial WiFiBoard(rx,tx);  //Declare a SoftwareSerial object
    WiFiBoard.begin(9600);            //initialize serial
    delay(100);
    while (Serial.available()==0) {}; // Waiting char
    //sending data to ESP32
    WiFiBoard.println(Serial.readString());
    delay(100);
    val=-1;
  }
  
} /* end loop statement                                      */
