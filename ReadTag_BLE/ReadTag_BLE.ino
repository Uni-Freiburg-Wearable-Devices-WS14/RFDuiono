/*
UART_DEFAULT_RX_PIN 0
UART_DEFAULT_TX_PIN 1

for RFduino
PIN_WIRE_SDA         (6u)
PIN_WIRE_SCL         (5u)
*/
#define I2C

#include <RFduinoBLE.h>

#ifdef UART
#include <PN532_HSU.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_HSU pn532uart(Serial);
PN532 pn532element(pn532uart);
NfcAdapter nfc = NfcAdapter(pn532uart);

#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
PN532 pn532element(pn532_i2c);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

void bleSendString(String sendstring);

void setup(void) {
  #ifdef I2C
    Serial.begin(9600);
    Serial.println("NDEF Reader");
    
  #endif
    
    
    // this is the data we want to appear in the advertisement
    // (if the deviceName and advertisementData are too long to fix into the 31 byte
    // ble advertisement packet, then the advertisementData is truncated first down to
    // a single byte, then it will truncate the deviceName)
    RFduinoBLE.advertisementData = "NFC_reader";
  
    // start the BLE stack
    RFduinoBLE.begin();    
    
    nfc.begin();
}

void loop(void) {
/*  
  #ifdef I2C

    uint32_t versionnumber;
    versionnumber = pn532element.getFirmwareVersion();
    Serial.print("\nVers.Num:");
    Serial.println(versionnumber);
    Serial.println("\nScan a NFC tag\n");    
    if(nfc.tagPresent())
      Serial.println("\nTag detected\n");        
    else
      Serial.println("\nNo tag detected\n");        
  #endif  
    if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        RFduinoBLE.send(1); // send out value 2 to inform about nfc detection
        tag.print();
    }
*/    
    delay(5000);
}

void RFduinoBLE_onDisconnect()
{

}

void RFduinoBLE_onReceive(char *data, int len)
{
  uint32_t versionnumber;
  String uid;
//  char uid_array[];
  // if the first byte is 0x01 / on / true
  for(int i= 0;i<len; i++)
  {
    //Serial.println(data[i]);
  }
  if (data[0])
  {
        //versionnumber = 5;
        //versionnumber = pn532element.getFirmwareVersion();
        //RFduinoBLE.send((char*) &versionnumber,4);
    if (nfc.tagPresent())
    {      

      NfcTag tag = nfc.read();
      uid = tag.getUidString();
      // uid_array = uid;       
      Serial.print(uid);
      bleSendString(uid);
//    RFduinoBLE.send((char*)&uid,uid.length()); // send out value 2 to inform about nfc detection
 //   RFduinoBLE.send(uid);
    }
  }
  else
  {
    char string1[]="No tag found";
    RFduinoBLE.send(string1,12);    
  }
}

void bleSendString(String sendstring) {
  char chararray[sendstring.length()];
  for(int i=0;i<sendstring.length();i++)
  {
    chararray[i] = sendstring.charAt(i);
  }
  RFduinoBLE.send(chararray,sendstring.length());  
}

