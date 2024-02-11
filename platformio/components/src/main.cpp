#include <M5GFX.h>
#include <M5Unified.h>
#include <SD.h>

#include "components/status-bar.h"
#include "components/wifi-status.h"
#include "const.h"
#include "features/brightness.h"
#include "features/display.h"
#include "features/env.h"
#include "features/font.h"
#include "features/rtc.h"
#include "features/scheduler.h"
#include "features/wifi.h"

M5GFX display;
Scheduler scheduler;

void setup() {
  M5.begin();

  // init
  initDisplay();
  initFont();
  initEnv(SD, "/env.txt");
  initWifi();
  initRtc();
  initAutoBrightness();

  // draw
  display.fillScreen(GRAY_COLOR);
  drawStatusBar();
}

void loop() {
  M5.update();

  display.waitDisplay();

  if (scheduler.intervalMs(1000)) {
    autoBrightness();
  }

  if (scheduler.intervalMs(10000)) {
    drawStatusBar();
  }

  display.display();
}
