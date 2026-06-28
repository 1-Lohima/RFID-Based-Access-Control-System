#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define GREEN_LED 4
#define RED_LED 5
#define BUZZER 6

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  Serial.println("==================================");
  Serial.println("   RFID Access Control System    ");
  Serial.println("   Designed by Ms. Lohima S      ");
  Serial.println("==================================");
  Serial.println("Place your card on reader...");
}

void accessGranted() {
  Serial.println("==================================");
  Serial.println("ACCESS GRANTED!");
  Serial.println("Welcome Lohima!");
  Serial.println("==================================");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);
  delay(2000);
  digitalWrite(GREEN_LED, LOW);
}

void accessDenied() {
  Serial.println("==================================");
  Serial.println("ACCESS DENIED!");
  Serial.println("Unauthorized Card!");
  Serial.println("==================================");
  digitalWrite(RED_LED, HIGH);
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
  digitalWrite(RED_LED, LOW);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Print UID
  Serial.print("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Check UID bytes directly
  // Blue Card UID = 01 02 03 04
  if (mfrc522.uid.uidByte[0] == 0x01 &&
      mfrc522.uid.uidByte[1] == 0x02 &&
      mfrc522.uid.uidByte[2] == 0x03 &&
      mfrc522.uid.uidByte[3] == 0x04) {
    accessGranted();
  } else {
    accessDenied();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  Serial.println("Place your card on reader...");
  delay(1000);
}
