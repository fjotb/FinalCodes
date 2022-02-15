#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10                   // Defines SS pin to Digital Pin 10
#define RST_PIN 9                   // Define Reset pin to Digital Pin 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Creating MFRC522 instance. (RFID Module)
#define buzzer 8                    // Define Piezo Buzzer as buzzer at Digital pin 8
#define redled  7                   // Define Red LED as redled at Digital Pin 7
#define blueled 6                   // Define Blue LED as blueled at Digital Pin 6
#define button 4                    // Define button at Digital Pin 4

int buttonstate = 0;                // initializing buttonstate as 0, 
                                    //-this variable will be used later as a- 
                                    //-container for the status of the
                                    //-button (whether it is pressed or not)

                                    // alloweduid = {"04 5F 3B 01 3F 1E 80", 
                                    // "E3 4C 2B 13", "04 4F 23 91 FA 24 80"};
                                    // Above is a list OF UID that the system- 
                                    //-is programmed to grant access to
void setup()                        // Code that runs once
{
  pinMode (button, INPUT);          // Setting button as input
  pinMode(buzzer, OUTPUT);          // Setting buzzer as output
  pinMode(redled, OUTPUT);          // Setting redled as output
  pinMode(blueled, OUTPUT);         // Setting blueled as output
  Serial.begin(9600);               // Beginning commication with board through serial port
  SPI.begin();                      // Initiating the  SPI bus
  mfrc522.PCD_Init();               // Initiating MFRC522 module.
  Serial.println();
  

}
void loop()                         // Code that keeps running on an infinite loop
{ 
  digitalWrite(redled, HIGH);       // This is the-
  digitalWrite(blueled, LOW);       // -Rest State of the-
  digitalWrite(buzzer, LOW);        // -Access Control
  
                                    // Start of Emergency Access Function
  buttonstate =digitalRead(button); // buttonstate becomes the container of the status of button
  if (buttonstate == HIGH){         // Stage 1 if button is high
    delay(800);                     // 0.8 seconds delay
    buttonstate = digitalRead(button);
    if (buttonstate == HIGH){       // if its still high, proceeds to stage 2
      digitalWrite(blueled, HIGH);  // blue LED turns on, now both LED is on
      delay(900);                   // 0.9 seconds delay
      buttonstate = digitalRead(button);
      if (buttonstate == LOW){      // if button is not held, proceeds to stage 3
      digitalWrite(redled, LOW);    // red LED turns off while blue LED stays on
      delay(900);                   // 0.9 seconds delay
      buttonstate = digitalRead(button);
      if (buttonstate == HIGH){     // If button is held, proceeds to stage 4:
        digitalWrite(redled,HIGH);
        digitalWrite(blueled, HIGH);// Both LEDs turnt on
        delay(900);                 // 0.9 seconds delay
        digitalWrite(redled, LOW);  // red LED turns off Blue LED stays ON
        digitalWrite(blueled, HIGH);
        digitalWrite(buzzer, HIGH); // buzzer turns on
        buttonstate = digitalRead(button);
        if (buttonstate == LOW){    // If button state is low, proceeds to stage 5
        for(int i = 0;i<3; i++){    // prompt through indicators for confirmation on emergency access
          digitalWrite(blueled, LOW);
        
          digitalWrite(buzzer, LOW);
          delay(270);
          digitalWrite(blueled,HIGH);
        
          digitalWrite(buzzer, HIGH);
          delay(270);
          digitalWrite(blueled, LOW);
          digitalWrite(buzzer, LOW);
        }
        buttonstate = digitalRead(button);
        if (buttonstate == HIGH){   // if button is held during this read emergency access is granted
          Serial.println("Emergency Access Granted");
          for (int i =0; i<4; i++){ // Indication for Emergency Access Grant
          digitalWrite(redled, HIGH);
          digitalWrite(buzzer, LOW);
          delay(150);
          digitalWrite(redled, LOW);
          digitalWrite(buzzer, HIGH);
          
          }
          digitalWrite(blueled, HIGH);
          delay(2200);
          digitalWrite(redled, HIGH);
          digitalWrite(redled, LOW);
          digitalWrite(buzzer, LOW);
                                    // End of Emergency Access Function
        }
        }
        }
      }
      
    }
    
  }
  
  if ( ! mfrc522.PICC_IsNewCardPresent())
                                    // Checks if RFID tag is present near the RFID reader
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
                                    // Selects and reads the RFID Tag's Serial
  {
    return;
  }
  
                                    //Below is the Algorithm to get the UID of the- 
                                    //-RFID tag and contain it into the variable content
  String content= "";               // Creating an empty string
  byte letter;                      // sets data type
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
                                    // containing the UID into the variable content
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();            // turning the container's content into all capital letters(uppercase)
                                    //Start of the Access Grant function
  
  if (content.substring(1) == "E3 4C 2B 13" || content.substring(1) == "04 4F 23 91 FA 24 80" || 
  content.substring(1) == "04 5F 3B 01 3F 1E 80")
                                    // Above is access grant list, if UID read by the- 
                                    //-RFID is present in this list, access will be granted
                                    // to add more just add to the conditons:-
                                    //- || content.substring "Candidate UID"
  {
    digitalWrite(blueled,HIGH);     // blue LED turns on
    digitalWrite(redled, LOW);      // red LED turns off
    Serial.print("Access Granted ");// prints Access Granted on Serial Port (used later through Python)
    
    Serial.print(content.substring(1));
                                    // Prints the UID of the tag which has been granted access-
                                    //-to (this information will be used later through python.)
   
    digitalWrite(buzzer, HIGH);     // buzzer on
    delay(2000);                    // 2 seconds delay
    digitalWrite(buzzer, LOW);      // blue LED turns off
    digitalWrite(redled, HIGH);     // red LED turns on
    digitalWrite(blueled,LOW);      // blue LED turns off
                                    // End of Access Grant Function
  }
    
 
 else {                             // If UID not in access grant list: Access Denial
    Serial.print("Unidentified ");
    Serial.print(content.substring(1));
    for (int i =0 ; i< 5 ; i++){    // loop to flash red LED to indicate access denial
      digitalWrite(redled, LOW);
      digitalWrite(buzzer, HIGH);
      delay(70);
      digitalWrite(redled, HIGH);
      digitalWrite(buzzer, LOW);
      delay(70);
    }
    delay(1000);
                                    // End of Access Denial function
  }
  Serial.println();
}
