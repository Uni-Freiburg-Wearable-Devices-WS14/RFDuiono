/*
UART_DEFAULT_RX_PIN 0
UART_DEFAULT_TX_PIN 1
*/


#include <RFduinoBLE.h>

#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
/*
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif
*/

void setup(void) {
    //Serial.begin(9600);
    //Serial.println("NDEF Reader");
    
    
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
    Serial.println("\nScan a NFC tag\n");
    if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        RFduinoBLE.send(1); // send out value 2 to inform about nfc detection
        tag.print();
    }
    delay(5000);
}

void RFduinoBLE_onDisconnect()
{
  // don't leave the led on if they disconnect  
}

void RFduinoBLE_onReceive(char *data, int len)
{
  // if the first byte is 0x01 / on / true
  for(int i= 0;i<len; i++)
  {
    Serial.println(data[i]);
  }
  if (data[0])
  {
    if (nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        RFduinoBLE.send(1); // send out value 2 to inform about nfc detection
        tag.print();
    }
  }
  else
  {
    RFduinoBLE.send(0);
  }
}


