#include <SPI.h> 
#include <MFRC522.h> // Install this library in Arduino IDE

#define SS_PIN 10
#define RST_PIN 9
#define SHORT 8
// Base OUTPUT pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Replace this UID with your own
String validKey = "B3 7A 85 FD";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(SHORT, OUTPUT);
  Serial.println(F("System ready. Place the key.")); // For Diagnostics
}

void loop() {
  // Look for new cards, and select one
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();

  // Remove the first space for correct comparison, if it exists
  String currentKey = content.substring(1);
  Serial.print("Key read: ");
  Serial.println(currentKey); // Tapped mfrc's UID

  if (currentKey == validKey) {
    Serial.println("Access granted!"); // Even though you cannot see the prited statement when PC is OFF, It helps while testing.
    digitalWrite(SHORT, HIGH); // Give input signal to pin 8 for 0.5 sec
    delay(500);
    digitalWrite(SHORT, LOW); // input signal OFF after 0.5 sec
  } 
  
  else {
    Serial.println("Access denied!"); // For Diagnostics
  }
}
