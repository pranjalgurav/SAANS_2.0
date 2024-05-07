
#include "ble.h"

extern Adafruit_NeoPixel  pixels_1(NUMPIXELS, 0, NEO_GRB + NEO_KHZ800);

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;

String valor;
volatile int device_disconnect;
int device_disconnect_start;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      Serial.println(value.data());
      if (value.length() > 0) {
        valor = "";
        for (int i = 0; i < value.length(); i++){
          // Serial.print(value[i]); // Presenta value.
          valor = valor + value[i];
        }
        if(valor == "red")
        {
          // set color to red
          
          pixel_config();
          pixel_show();
          
          //delay(2000); // wait half a second
        }
        if(valor == "on")
        {
        // set color to red
        //Serial.println(valor);
        
          pixel_config();
        
        pixel_show();               
          //deviceConnected=false;
          //tone(27,234);
          //delay(2000); // wait half a second
        }
        if(valor == "blue")
        {
        // set color to red
       
          pixel_config();
          
          pixel_show();
          //delay(2000); // wait half a second
        }
        else
        {
          if(valor.length() > 10)
          {
          //string_parse(valor);
          valor="";
          }
        }
        
       /* Serial.println("*********");
        Serial.print("valor = ");
        Serial.println(valor); // Presenta valor.*/
      }
    }
};



class MyServerCallbacks1: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    //ble_initialization();
    device_disconnect_start=1;
    device_disconnect++;
    deviceConnected = false;
  }
};

void ble_initialization()
{
  // Create the BLE2 Device
  if(device_disconnect_start == 0)
  {
  BLEDevice::init("SAANS");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks1());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  }
  if(device_disconnect_start == 1)
  {
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  }
}
void pixel_config(){
 pixels_1.begin();
          pixels_1.setBrightness(2); 
}
void pixel_show(){
  if(valor == "red")
  {
   pixels_1.fill(0xFF0000);
          pixels_1.show();
  }
  if(valor == "on")
  {
   pixels_1.fill(0x00FF00);
          pixels_1.show();
  }
  
}
