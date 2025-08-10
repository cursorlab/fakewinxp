#include <TFT_eSPI.h>
#include <SPI.h>

#include "frame.h"
#include "welcome.h"
#include "error.h"
#include "bsod.h"

/*
 * ESP32 与 TFT_eSPI 屏幕引脚对应关系
 * ┌────────┬────────────┐
 * │ ESP32  │   TFT_eSPI │
 * ├────────┼────────────┤
 * │   23   │   SDA      │
 * │   18   │   SCL      │
 * │   5    │   CS       │
 * │   2    │   DC       │
 * │   4    │   RES      │
 * │   15   │   BLK      │
 * └────────┴────────────┘
 *
 * 说明：
 * - SDA: 数据线
 * - SCL: 时钟线
 * - CS : 片选
 * - DC : 数据/命令选择
 * - RES: 复位
 * - BLK: 背光
 */

#define PWM_FREQ        5000    // 5KHz
#define PWM_RESOLUTION  8       // 8bit 0-255，可调整背光亮度
#define PWM_CHANNEL     0       // 通道0
#define PWM_PIN         15      // 引脚15，对应TFT_BL
#define PWM_VAL         255     // 255为最亮

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true);

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, PWM_VAL);
}

void loop() {
    // frame
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 12; j++) {
            tft.pushImage(52, 56, 150, 151, frames[j]);
            delay(100);
    }

    // welcome
    tft.pushImage(0, 0, 240, 240, welcome);
    delay(6000);

    // error
    tft.fillScreen(TFT_BLACK);
    tft.pushImage(40, 50, 160, 102, error);
    delay(4000);
    
    // bsod
    tft.pushImage(0, 0, 240, 240, bsod);
    delay(7000);

    tft.fillScreen(TFT_BLACK);
}