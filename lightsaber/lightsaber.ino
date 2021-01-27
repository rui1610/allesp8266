#include <WS2812FX.h>
#include "ota.h"
#include "leds.h"

void setup() {
  ota_setup();
  led_setup();

}

void loop() {
  ota_loop();
  led_loop();  
}
