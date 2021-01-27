
#define LED_COUNT 31
#define LED_PIN 13
#define BUTTON_PIN1 14

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


int COLOR_COUNTER = 0;

int myEffects[] = {FX_MODE_STATIC, FX_MODE_BREATH, FX_MODE_FIRE_FLICKER};
int myColors[] = {FX_MODE_STATIC, FX_MODE_BREATH, FX_MODE_FIRE_FLICKER};


int button1State;             // the current reading from the input pin

int lastButton1State = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

void led_setup() {
  
  ws2812fx.init();
  ws2812fx.setBrightness(100);
  ws2812fx.setColor(0, 255, 0);
  ws2812fx.setSpeed(250);
  ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
  ws2812fx.start();

  pinMode(BUTTON_PIN1,INPUT);

}

void led_loop() {
  int statusButton1;
  
  ws2812fx.service();
  statusButton1=digitalRead(BUTTON_PIN1);

      // only toggle the LED if the new button state is HIGH
      if (statusButton1 == HIGH) {
        Serial.print ("Knopf 1 gedrueckt");
        COLOR_COUNTER = COLOR_COUNTER + 1;
        
        if (COLOR_COUNTER == 1){
          ws2812fx.setColor(0, 255, 0);
          ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
        }

        if (COLOR_COUNTER == 2){
          ws2812fx.setColor(255, 0, 0);
          ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
        }

        if (COLOR_COUNTER == 3){
          ws2812fx.setColor(0, 0, 255);
          ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
        }

        if (COLOR_COUNTER == 4){
          ws2812fx.setColor(255, 0, 255);
          ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
        }


        if (COLOR_COUNTER == 5){
          ws2812fx.setColor(240, 171, 0);
          ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
        }

        if (COLOR_COUNTER == 6){
          ws2812fx.setColor(255, 255, 255);
          ws2812fx.setSpeed(100);
          ws2812fx.setMode(FX_MODE_RUNNING_LIGHTS);
        }
        
        if (COLOR_COUNTER == 7){
          ws2812fx.setColor(255, 255, 255);
          ws2812fx.setSpeed(5);
          ws2812fx.setMode(FX_MODE_DUAL_SCAN);
          COLOR_COUNTER = 0;
        }
        
      }
 
}
