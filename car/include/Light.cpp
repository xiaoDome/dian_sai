#include "PinDefine.hpp"
#include "FastLED.h"

#define LED_TYPE WS2812  //灯带类型
#define LOGO_NUM_LEDS 2
#define DEBUG_NUM_LEDS 1
#define COLOR_ORDER GRB  //灯珠顺序
#define BRIGHTNESS 200
uint8_t max_brightness = 250;  //最大亮度

CRGB Led_Logo[LOGO_NUM_LEDS];
CRGB Led_Debug[DEBUG_NUM_LEDS];

CHSV DebugHSVcolor(0, 200, 200);


void Light_Init() {
  pinMode(LED_SYS, OUTPUT);

  digitalWrite(LED_SYS, HIGH);

  FastLED.addLeds<LED_TYPE, LED_LOGO, COLOR_ORDER>(Led_Logo, LOGO_NUM_LEDS);
  //   FastLED.addLeds<LED_TYPE, LED_DEBUG, COLOR_ORDER>(Led_Debug, DEBUG_NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  //   Led_Debug[0] = CRGB::Black;
  Led_Logo[0] = CRGB::White;
  Led_Logo[1] = CRGB::White;
  // Led_Debug[0] = CRGB::Pink;

  FastLED.show();
  delay(100);
  strcpy(Esp_Data.DebugMes, "Light_Init OK");
}

void Light_System_Loop() {
  for (int _led_sys_brightness = 0; _led_sys_brightness < 254; _led_sys_brightness++) {
    analogWrite(LED_SYS, _led_sys_brightness);
    delay(3);
  }
  delay(100);
  for (int _led_sys_brightness = 255; _led_sys_brightness > 1; _led_sys_brightness--) {
    analogWrite(LED_SYS, _led_sys_brightness);
    delay(3);
  }
  delay(100);
}

void Light_Debug_Loop() {
  //     DebugHSVcolor.h++;
  //   DebugHSVcolor.h=DebugHSVcolor.h%255;
  //   fill_solid(Led_Debug,1,DebugHSVcolor);
  //   FastLED.show();
  // delay(10);
}

void Light_Logo_Loop() {

  Fire2012();  // run simulation frame

  FastLED.show();  // display this frame
  FastLED.delay(1000 / 60);

  // delay(10);
}



void Fire2012() {
  static uint8_t heat[LOGO_NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < LOGO_NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / LOGO_NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = LOGO_NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < 120) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < LOGO_NUM_LEDS; j++) {
    CRGB color = HeatColor(heat[j]);
    int pixelnumber;
    pixelnumber = j;
    Led_Logo[pixelnumber] = color;
  }
}