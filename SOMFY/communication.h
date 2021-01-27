#define SYMBOL 640
#define UP 0x2
#define STOP 0x1
#define DOWN 0x4
#define PROG 0x8
#define EEPROM_ADDRESS 0
#include "config.h"
#include "somfyRemotes.h"

byte frame[7];
byte checksum;

const int transmitPin = 14;

unsigned long rollingCode = 1;

int getEEPromAddressRemote(somfyRemote remote){
  return remote.number * 4; 
}

void SendCommand(byte *frame, byte sync)
{
  if (sync == 2)
  { // Only with the first frame.
    //Wake-up pulse & Silence
    digitalWrite(transmitPin, HIGH);
    delayMicroseconds(9415);
    digitalWrite(transmitPin, LOW);
    delayMicroseconds(89565);
  }

  // Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i++)
  {
    digitalWrite(transmitPin, HIGH);
    delayMicroseconds(4 * SYMBOL);
    digitalWrite(transmitPin, LOW);
    delayMicroseconds(4 * SYMBOL);
  }

  // Software sync
  digitalWrite(transmitPin, HIGH);
  delayMicroseconds(4550);
  digitalWrite(transmitPin, LOW);
  delayMicroseconds(SYMBOL);

  //Data: bits are sent one by one, starting with the MSB.
  for (byte i = 0; i < 56; i++)
  {
    if (((frame[i / 8] >> (7 - (i % 8))) & 1) == 1)
    {
      digitalWrite(transmitPin, LOW);
      delayMicroseconds(SYMBOL);
      digitalWrite(transmitPin, HIGH);
      delayMicroseconds(SYMBOL);
    }
    else
    {
      digitalWrite(transmitPin, HIGH);
      delayMicroseconds(SYMBOL);
      digitalWrite(transmitPin, LOW);
      delayMicroseconds(SYMBOL);
    }
  }

  digitalWrite(transmitPin, LOW);
  delayMicroseconds(30415); // Inter-frame silence
}


void BuildFrame(byte *frame, byte button, somfyRemote remote)
{
  unsigned int code;
  int myEEPROM_ADDRESS = getEEPromAddressRemote(remote);

  EEPROM.get(myEEPROM_ADDRESS, code);
  //Serial.println("Before: Adress for remote " + String(remote.number) + ": >" + String(myEEPROM_ADDRESS) + "< has value >" + code + "<");

  frame[0] = 0xA7;            // Encryption key. Doesn't matter much
  frame[1] = button << 4;     // Which button did  you press? The 4 LSB will be the checksum
  frame[2] = code >> 8;       // Rolling code (big endian)
  frame[3] = code;            // Rolling code
  frame[4] = remote.id >> 16; // Remote address
  frame[5] = remote.id >> 8;  // Remote address
  frame[6] = remote.id;       // Remote address

  //Serial.print("Frame for remote " + String(remote.number) + " :");
  for (byte i = 0; i < 7; i++)
  {
    if (frame[i] >> 4 == 0)
    {                    //  Displays leading zero in case the most significant
      //Serial.print("0"); // nibble is a 0.
    }
    //Serial.print(frame[i], HEX);
    //Serial.print(" ");
  }

  // Checksum calculation: a XOR of all the nibbles
  checksum = 0;
  for (byte i = 0; i < 7; i++)
  {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // We keep the last 4 bits only

  //Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.

  //Serial.println("");
  //Serial.print("With checksum : ");
  for (byte i = 0; i < 7; i++)
  {
    if (frame[i] >> 4 == 0)
    {
      //Serial.print("0");
    }
    //Serial.print(frame[i], HEX);
    //Serial.print(" ");
  }

  // Obfuscation: a XOR of all the bytes
  for (byte i = 1; i < 7; i++)
  {
    frame[i] ^= frame[i - 1];
  }

  //Serial.println("");
  //Serial.print("Obfuscated    : ");
  for (byte i = 0; i < 7; i++)
  {
    if (frame[i] >> 4 == 0)
    {
      //Serial.print("0");
    }
    //Serial.print(frame[i], HEX);
    //Serial.print(" ");
  }
  //Serial.println("");
  //Serial.print("Rolling Code  : ");
  //Serial.println(code);
  EEPROM.put(myEEPROM_ADDRESS, code + 1); //  We store the value of the rolling code in the
                                          // EEPROM. It should take up to 2 adresses but the
                                          // Arduino function takes care of it.
  EEPROM.commit();
  //Serial.println("After : Adress for remote " + String(remote.number) + ": >" + String(myEEPROM_ADDRESS) + "< has value >" + code + "<");
  
}
