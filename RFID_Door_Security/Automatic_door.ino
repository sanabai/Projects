#include <pitches.h>







#include <deprecated.h>

#include <MFRC522.h>

#include <MFRC522Extended.h>

#include <require_cpp11.h>



/* 

 *  RFID Module RC522

 *  Simple Project:- Arduino will read RFID Tag and Display To the Serial Monitor!

 *  

 *  RFID RC522 and Arduino Uno Pin Configuration

 *  

 *  RFID RC522          Arduino Uno

 *  SS/SDA              D10

 *  SCK                 D13

 *  MOSI                D11

 *  MISO                D12

 *  IRQ                 Not Connected

 *  GND                 GND

 *  RST                 D9

 *  3.3V                3.3V

 *  

 *  //Note: We connected LCD to the Analog Pins.

 *  Remember, Analog Pins can be act as a Digital Pins,

 *  But Digital Pins CAN NOT be act as a Analog Pins.

 *  

 *  LCD(16x2) Connections

 *  

 *  VSS pin to GND

 *  VCC pin to 5V

 *  //We are connecting "Contrast pin to GND for Maximum Brightness"

 *  //You can also connect it to 10K Pot if you wish to change the Brightness Of LCD.

 *  VEE pin to GND 

 *  RS pin to Analog pin A0

 *  R/W pin to ground

 *  Enable pin to Analog pin A1

 *  D4 pin to Analog pin 5

 *  D5 pin to Analog pin 4

 *  D6 pin to Analog pin 3

 *  D7 pin to Analog pin 2

 *  LED+ pin to POWER

 *  LED- pin to GND 

 *   

 *  Servo is Connected to Digital Pin 5  

 *  

 *  RGB LED

 *  From RGB LED, We need Only Red and Green Color, 

 *  So we are not connecting Blue pin of the RGB. 

 *  

 *  Red pin to Digital pin 6

 *  Green pin to Digital pin 7

 *  

 *  Buzzer to Digital pin 8

 * 

 *  

 *  Note:- RFID uses SPI Protocol to transfer the information.

 *  I took the Melody(for the Buzzer) from, 

 *  File -> Examples -> Digital -> ToneMelody

*/



//Include sections



#include <SPI.h>

#include <LiquidCrystal.h>

#include <Servo.h>





//Define Component to Arduino Pins

#define SS_PIN 10

#define RST_PIN 9



#define SERVO_PIN 5



#define Red_LED 6

#define Green_LED 7



#define Buzzer 8





//initialize the library with the numbers of the interface pins

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo DoorLock;

MFRC522 rfid(SS_PIN, RST_PIN );



//Unique ID of RFID Tag, which you want to give access. 

int My_RFID_Tag[5] = {0x58,0x76,0x17,0x10,0x29};



//variable to hold your Access_card

boolean My_Card = false;  

   

// notes in the melody, taken from:

//File -> Examples -> Digital -> ToneMelody

int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4

};



// note durations: 4 = quarter note, 8 = eighth note, etc.:

int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4

};



void setup() 

{

  // put your setup code here, to run once:

  //set the pins as an input/output

  pinMode(Red_LED,OUTPUT);

  pinMode(Green_LED,OUTPUT);

  pinMode(Buzzer,OUTPUT);

  

  //Servo Connnected to pin Digital Pin 5

  DoorLock.attach(SERVO_PIN);

  //open the serial port at 9600 baudrate.

  Serial.begin(9600); 

  //Initialise the LCD to 16x2 Character Format

  lcd.begin(16,2);

  //Initialise Servo and RFID

  SPI.begin();

  rfid.PCD_Init();

}



void loop() 

{

  // put your main code here, to run repeatedly:

  

  //First Assume detected card(Or tag) is My_Card, 

  //Then later we will check is it My_Card or not! 

  My_Card = true; 

  DoorLock.write(0); //Servo at 0 Position, Door is Closed.

  lcd.clear();

  lcd.print("Robodia Technology");

  lcd.setCursor(0,1);

  lcd.print("gy Solutions!");

  

  //Check if any RFID Tags Detected or not?

  if( rfid.PICC_IsNewCardPresent() )

  {

      //if RFID Tag is detected, check for the Unique ID,

      //and print it on the Serial Window

      if( rfid.PICC_ReadCardSerial() )

      {   

          lcd.clear();      

          lcd.print("UNIQUE ID is:- ");

          delay(500);          

          lcd.setCursor(0,1); //Set LCD Cursor to Second Row, First Character

  

      //Unique id is 5 Digit Number.

          //Printing in HEX for better Understanding

          for( int i = 0; i < 5; i++ )

          {

              Serial.print(rfid.uid.uidByte[i], HEX);

              Serial.print(" ");              

              lcd.print(rfid.uid.uidByte[i],HEX);

              lcd.print(" ");                            

          }  

          delay(500);

          

      //Compare this RFID Tag Unique ID with your My_RFID_Tag's Unique ID

          for(int i = 0; i < 5; i++)

          {   

              //if any one Unique ID Digit is not matching,

              //then make My_Card = false and come out from loop

              //No need to check all the digit!

              if( My_RFID_Tag[i] != rfid.uid.uidByte[i] )

              {

                My_Card = false;

                break;                

              }           

          }

          Serial.println(); 

          delay(1000); 



          //If RFID Tag is My_Card then give access to enter into room

          //else dont open the door.

          if(My_Card)

          {

            Serial.println("\nWelcome To Your Room, Robodia!");

            lcd.clear();

            lcd.print("Welcome to Your");

            lcd.setCursor(0,1);

            lcd.print("Room, Robodia!");

            delay(2000);                        

            

            //Turn on the Green LED as an indication of permission is given 

            //to access the room.

            digitalWrite(Green_LED,HIGH);

            

            //Buzzer Config, taken from:

            //File -> Examples -> Digital -> ToneMelody

            // iterate over the notes of the melody:

            int i = 0;

            while(i < 2)

            {

              for (int thisNote = 0; thisNote < 8; thisNote++) 

              {          

                // to calculate the note duration, take one second

                // divided by the note type.

                //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

                int noteDuration = 1000 / noteDurations[thisNote];

                tone(8, melody[thisNote], noteDuration);                        

                // to distinguish the notes, set a minimum time between them.

                // the note's duration + 30% seems to work well:

                int pauseBetweenNotes = noteDuration * 1.30;

                delay(pauseBetweenNotes);              

                // stop the tone playing:

                noTone(8);

              }

              i =  i + 1;

              delay(500);              

            }

            delay(1000);            



            //Now, Open the Door with the help of Servo Motor

            DoorLock.write(180);            

            delay(200);

            lcd.clear();                                    

            lcd.print("Door is Open");

            lcd.setCursor(0,1);

            lcd.print("Now!");

            delay(2000);

            lcd.clear();



            //Give 10 Sec delay to enter into room

            //After that door will again closed!

            for(int i = 10; i > 0; i--)

            {

              lcd.print("Door will close");

              lcd.setCursor(0,1);

              lcd.print("in ");

              lcd.print(i);

              lcd.print(" Sec.HurryUp!");

              delay(1000);

              lcd.clear();

            }



            //Now,Door is closed and Green LED is Turned-Off.

            DoorLock.write(0);

            digitalWrite(Green_LED,LOW);

            delay(200);

            lcd.clear();

            lcd.print("Door is Close");

            lcd.setCursor(0,1);

            lcd.print("Now!");

            delay(2000);                            

          }

          

          // If RFID Tag is not My_Card then

          // Do not open the Door and 

          //Turn-On Red LED and Buzzer as an indication of Warning:

          //Somebody else is trying to enter into your room. 

          else

          {

            Serial.println("\nGet Out of Here !");

            lcd.clear();

            lcd.print("Card isNOT FOUND!");

            lcd.setCursor(0,1);

            lcd.print("Get Out of Here!");



            for(int i = 0; i < 7; i++)

            {

              digitalWrite(Buzzer, HIGH);

              digitalWrite(Red_LED,HIGH);

              delay(500);

              digitalWrite(Buzzer, LOW);

              digitalWrite(Red_LED,LOW);

              delay(500);              

            }

            delay(1000);            

          }                 

      }      

   }

  //Put RFID Reader into Halt, untill it not detects any RFID Tag.

   rfid.PICC_HaltA();

}
