#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   7
#define SS_PIN    8

MFRC522 RFID(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

byte tagActual[4];
byte tagConocido[4] = {0x39, 0xEB, 0xE4, 0xA3};  // UID conocido

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) 
    return;

  if (!mfrc522.PICC_ReadCardSerial()) 
    return;

  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    tagActual[i] = buffer[i];
  }
  Serial.println(); // Nueva línea para claridad en el monitor serial

  if (compararArray(tagActual, tagConocido)) {
    Serial.println(" Acceso Permitido...");
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    tone(9, 200);
    delay(4000);
    noTone(9);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH); // Volver a rojo después del acceso permitido
  } 
  else {
    Serial.println(" Desconocido");
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH); // Rojo encendido para tarjeta desconocida
    tone(9, 400);
    delay(1000);
    noTone(9);
  }
}

boolean compararArray(byte array1[], byte array2[]) {
  for (byte i = 0; i < 4; i++) {
    if (array1[i] != array2[i]) return false;
  }
  return true;
}
