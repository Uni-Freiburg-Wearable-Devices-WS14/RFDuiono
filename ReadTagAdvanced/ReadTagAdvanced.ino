/*
UART_DEFAULT_RX_PIN 0
UART_DEFAULT_TX_PIN 1

for RFduino
PIN_WIRE_SDA         (6u)
PIN_WIRE_SCL         (5u)
*/
#define I2C

#include <RFduinoBLE.h>

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

void bleSendString(String sendstring);


PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
PN532 pn532element(pn532_i2c);

// flag that shows connection status
bool isConnected = false;
// flag to prevent simultanious access to nfc chip
bool nfcIsBusy = false;

void setup(void) {
    Serial.begin(9600);
    Serial.println("NDEF Reader");       
    
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
  Serial.println("\nScan a NFC tag\n");    
  if (nfc.tagPresent())
  {
      NfcTag tag = nfc.read();
      RFduinoBLE.send(1); // send out value 2 to inform about nfc detection
      tag.print();
  }
  else
    Serial.println("\nNo tag detected\n");        
*/    
  String uid;
  if(isConnected)
  {
   if(!nfcIsBusy)
   {
     nfcIsBusy = true;
     if (nfc.tagPresent())
      {            
        NfcTag tag = nfc.read();
        uid = tag.getUidString();
        nfcIsBusy = false;        
        // uid_array = uid;       
        Serial.println(uid);
        bleSendString(uid);
      }
      else
      {
        nfcIsBusy = false;
        Serial.println("No tag found");
      }
    }
  }
  // wait for next cycle of reading
  delay(5000);
}

void RFduinoBLE_onDisconnect()
{
  isConnected = false;
  Serial.println("disconnected\n");   
}

void RFduinoBLE_onConnect()
{
  isConnected = true;
  Serial.println("connected\n"); 
}

void RFduinoBLE_onReceive(char *data, int len)
{
  if(!nfcIsBusy)
  {
    String uid;
    if (data[0])
    {
      nfcIsBusy = true;
      // Value other than 0 triggers a nfc read operation
      if (nfc.tagPresent())
      {      
        NfcTag tag = nfc.read();
        uid = tag.getUidString();
        nfcIsBusy = false;
        // uid_array = uid;       
        Serial.println(uid);
        bleSendString(uid);
      }
      else
      {
        nfcIsBusy = false;
        String string1="No tag found";
        Serial.println(string1);
        bleSendString(string1);    
      }
    }
    else
    {
      // For 0 value we respond with Version number
      uint32_t versiondata;
      
      nfcIsBusy = true;
      versiondata = pn532element.getFirmwareVersion();    
      nfcIsBusy = false;
      if (!versiondata)
      {
          Serial.println(F("Didn't find PN53x board"));
          Serial.println(versiondata);
      }
      else
      {
        String string1 = String("\nFound chip PN5" + String((versiondata>>24) & 0xFF, HEX));
        Serial.println(string1);
        bleSendString(string1);
        
        String string2 = String("Firmware ver. " + String((versiondata>>16) & 0xFF, DEC) + '.' + String((versiondata>>8) & 0xFF, DEC));
        Serial.println(string2);
        bleSendString(string2);      
      }
    }
  }
}

// function to easily send strings over BLE
void bleSendString(String sendstring) 
{
  RFduinoBLE.send(sendstring.cstr(),sendstring.length());  
}

