
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Adafruit_NeoPixel.h>

#define DEFAULT_I2C_PORT &Wire
#define NUMPIXELS        1
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

extern bool deviceConnected ;

#ifndef BLE_H
#define BLE_H
void ble_initialization();
void pixel_config();
void pixel_show();

#endif
