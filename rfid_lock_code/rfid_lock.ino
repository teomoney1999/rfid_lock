#include <SoftwareSerial.h>
#include <SerialCommand.h>

#include <ArduinoJson.h>

#include <SPI.h>
#include <MFRC522.h>

// LY_03
#define LY_03         8
#define LOCK_INIT     0
#define LOCK_OPEN     1
#define LOCK_WAITING  2
#define LOCK_CLOSE    3

byte lockstate = LOCK_INIT;

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;

SoftwareSerial mySerial(2, 3);
SerialCommand sCmd(mySerial);

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  mySerial.begin(115200);
  delay(10);

  pinMode(LY_03, OUTPUT);
  digitalWrite(LY_03, HIGH);      // default state of LY_03

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(100);       // Optional delay. Some board do need more time after init to be ready, see Readme

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  } 

  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  sCmd.addCommand("auth", auth);
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  handleRfid();
  
  sCmd.readSerial();
}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
String toString(byte *buffer, byte bufferSize) {
  String data = "";
  for (byte i = 0; i < bufferSize; i++) {
    data.concat(String(buffer[i] < 0x10 ? " 0" : " "));
    data.concat(String(buffer[i], HEX));
  }
  return data.substring(1);
}

void handleRfid() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  byte sector         = 1;
  byte blockAddr      = 4;
  byte dataBlock[]    = {
    0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
    0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
    0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
    0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
  };
  byte trailerBlock   = 7;
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.println(F("Access Authorized!"));

  // Read data
  Serial.print("Reading data from block ");
  Serial.print(blockAddr);
  Serial.println(" ...");
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status !=  MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  // Handle data from block
  String code = toString(buffer, 16);
  StaticJsonDocument <96> doc;

  doc[F("code")] = code;
  Serial.println(code.length());
  char output[58];
  serializeJson(doc, output);
  Serial.println( output); // debug


  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  mySerial.print("rfid-code");
  mySerial.print("\r");
  mySerial.print(output);
  mySerial.print("\r");
}

void auth() {
  Serial.println("Auth func");
  char* input = sCmd.next(); 

  if (!input) return;
  
  StaticJsonDocument<10> doc;
  deserializeJson(doc, input);

  byte isAuth = doc["isAuth"];
  Serial.println(isAuth);
  if (!isAuth) {
    Serial.println(F("Access Denied!"));
    lockstate = LOCK_CLOSE;
  } else if (isAuth && lockstate != LOCK_OPEN) {
    Serial.println(F("Authorized access!"));
    lockstate = LOCK_OPEN;
  } else {
    // if lock has already been opened,
    // use the card to lock the door
    Serial.println("Door is locked!");
    lockstate = LOCK_CLOSE;
  }

  switch (lockstate) {
    case LOCK_OPEN: 
      digitalWrite(LY_03, LOW);
      break;
    case LOCK_CLOSE: 
      digitalWrite(LY_03, HIGH);
      break;
  }

  Serial.println(lockstate);
}
