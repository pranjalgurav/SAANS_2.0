
#include "ble.h"

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  4        /* Time ESP32 will go to sleep (in seconds) */


RTC_DATA_ATTR int bootCount = 0;

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button_33 = {33, 0, false};
Button button_38 = {38, 0, false};

void ARDUINO_ISR_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = 0;
const long interval = 8000;           // interval at which to blink (milliseconds)


void setup(){
   Serial.begin(115200);
   ble_initialization();
  //delay(1000); //Take some time to open up the Serial Monitor

    pinMode(button_33.PIN, INPUT_PULLUP);
    attachInterruptArg(button_33.PIN, isr, &button_33, FALLING);

    pinMode(button_38.PIN, INPUT_PULLUP);
    attachInterruptArg(button_38.PIN, isr, &button_38, FALLING);

 /// NeoPixel Setup  starts  ///
#if defined(NEOPIXEL_POWER)
  // If this board has a power control pin, we must set it to output and high
  // in order to enable the NeoPixels. We put this in an #if defined so it can
  // be reused for other boards without compilation errors
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif


pixel_config();
pixel_show();


//Print the wakeup reason for ESP32
 print_wakeup_reason();


///    wait for some seconds ...Find my device feature /////
  previousMillis = millis();
       while(1)
          {
      currentMillis = millis();
      if (currentMillis - previousMillis >= 12000) {
         Serial.print(currentMillis); Serial.print("\t"); Serial.println(previousMillis);
         break;
              }
              if(button_33.pressed)    // Dosage interrupt
              {
                button_33.pressed = false;
                Serial.println("Dosage Interrupt");
                //Read_Sensor();
                //Write_eeprom();
              }
              if(button_38.pressed)   //  Accelerometer interrupt
              {
                 button_38.pressed = false;
                 Serial.println("Accelerometer Interrupt");
                 //Read_eeprom();
                 //Notify_BLE();
              }
           }



////   Wait for some time over   /////////

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_38,0); //1 = High, 0 = Low

  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  //Go to sleep now

  esp_deep_sleep_start();
}



void loop(){
  
}
