//Arduino Code - RC522 Read RFID Tag UID

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>


#define SS_PIN 53
#define RST_PIN 7
 
MFRC522 rfid(SS_PIN, RST_PIN); // Depends on your configuration
MFRC522::MIFARE_Key key; 
Servo myservo ;

byte id[] = {} ; // AUthorised Student ID

void setup() { 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init(); 
  myservo.attach(9) ;
  pinMode(31, OUTPUT) ;
  pinMode(23, OUTPUT) ;
  
}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  Serial.print(F("RFID Tag UID:"));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println() ;
  if (authorizeID(id, rfid.uid.size, rfid.uid.uidByte)) {
    myservo.write(90) ;
    delay(1000) ;
    myservo.write(180) ;
  }

  

  rfid.PICC_HaltA(); // Halt PICC
}

//Routine to dump a byte array as hex values to Serial. 
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// To check the authorised of this card
bool authorizeID(byte *id, byte buffersize, byte *buffer) {
  byte i = 0 ;
  while (i < buffersize) {
    if (id[i] == buffer[i]) {
      i ++ ;
    }
    else {
      digitalWrite(31, HIGH) ;
      delay(1000) ;
      digitalWrite(31, LOW) ;
      Serial.println("False") ;
      return false ;
    }

    digitalWrite(23, HIGH) ;
    delay(1000) ;
    digitalWrite(23, LOW) ;
    Serial.println("True") ;
    return true ;
  }
}
