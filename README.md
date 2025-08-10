### Fake Windows XP on ESP32 (240x240 ST7789)

一个基于 ESP32 与 1.3/1.54 寸 240×240 ST7789 屏幕的“伪 Windows XP 启动 → 欢迎 → 错误 → 蓝屏”演示项目。使用 Arduino 框架与 `TFT_eSPI` 库进行图像推送，包含背光 PWM 亮度调节与自定义素材替换能力。

---

### 功能概览

- **启动动画**: 循环播放开机进度条帧图
- **欢迎界面**: 显示 Welcome 画面
- **错误提示**: 显示经典错误对话框
- **蓝屏 (BSOD)**: 显示蓝屏画面
- **背光调节**: 通过 PWM 控制 `TFT_BL` 引脚亮度

---

### 硬件需求

- **主控**: ESP32 开发板（`esp32dev`）
- **屏幕**: SPI 接口 ST7789 240×240 TFT（无触控或带触控均可；本工程未使用触控）
- 连接线若干

屏幕与 ESP32 引脚连接（已在代码与 `include/tft_espi_setup.h` 配置）：

| 功能 | 屏幕引脚 | ESP32 引脚 |
| --- | --- | --- |
| MOSI (SDA) | SDA | 23 |
| SCLK | SCL | 18 |
| CS | CS | 5 |
| DC | DC | 2 |
| RST | RES | 4 |
| 背光 | BLK | 15 |

说明：以上与 `src/main.cpp` 注释一致；如果你的屏幕丝印不同（如 `DIN`/`CLK`/`RS`），请对应 MOSI/SCLK/DC 连接。

---

### 软件环境

- 使用 [PlatformIO](https://docs.platformio.org)（VS Code 扩展或 CLI）
- 框架: Arduino
- 依赖库: `TFT_eSPI@^2.5.43`

`platformio.ini` 关键配置：

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = bodmer/TFT_eSPI@^2.5.43
build_flags =
  -DUSER_SETUP_LOADED=1
  -Iinclude
  -include tft_espi_setup.h
```

---

### 快速开始

1) 克隆或下载本仓库，并使用 VS Code 打开工程目录。

2) 安装 VS Code 扩展 “PlatformIO IDE”（或使用 CLI）。

3) 连接 ESP32 到电脑 USB。

4) 构建并烧录：

- VS Code/PlatformIO: 点击状态栏的“对勾”（编译）与“右箭头”（烧录）。
- CLI：

```bash
pio run -t upload
```

5) 串口监视器（可选，115200）：

```bash
pio device monitor -b 115200
```

烧录成功后，屏幕会按顺序显示：启动帧 → 欢迎 → 错误 → 蓝屏。

---

### 屏幕与 `TFT_eSPI` 配置

本项目通过 `include/tft_espi_setup.h` 覆盖 `TFT_eSPI` 的用户配置（由 `platformio.ini` 的 `build_flags` 注入）。关键点：

- 驱动: `ST7789_DRIVER`
- 分辨率: `TFT_WIDTH 240`、`TFT_HEIGHT 240`
- 引脚: `TFT_CS=5`、`TFT_MOSI=23`、`TFT_SCLK=18`、`TFT_DC=2`、`TFT_RST=4`、`TFT_BL=15`
- 频率: `SPI_FREQUENCY 80MHz`
- 颜色顺序: `TFT_RGB_ORDER TFT_BGR`

在 `src/main.cpp` 中：

- `tft.setRotation(0)`：竖屏方向；如需旋转，改为 1/2/3 试试。
- `tft.setSwapBytes(true)`：与素材字节序匹配（常见 RGB565 小端导出时需要）。

---

### 背光亮度调节（PWM）

`src/main.cpp` 使用 LEDC（硬件 PWM）控制 `TFT_BL`：

- 频率: `PWM_FREQ = 5000`（5 kHz）
- 分辨率: `PWM_RESOLUTION = 8`（0–255）
- 通道: `PWM_CHANNEL = 0`
- 引脚: `PWM_PIN = 15`
- 默认亮度: `PWM_VAL = 255`（最亮）

调节亮度：修改 `PWM_VAL`（0–255），数值越小越暗。

---

### 素材说明与替换

素材位于 `include/`：

- `frame.h`：进度条动画帧数组（`frames[...]`）
- `welcome.h`：欢迎图片（`welcome`）
- `error.h`：错误图片（`error`）
- `bsod.h`：蓝屏图片（`bsod`）

在 `loop()` 中使用 `tft.pushImage(x, y, w, h, data)` 推送到屏幕。若要替换素材：

1) 使用图像转换工具（如 `LCD-Image-Converter`）将图片转为 `RGB565`（建议小端），导出为 `const uint16_t name[] PROGMEM` 数组。
2) 确保尺寸与调用时的 `w, h` 匹配（例如欢迎/蓝屏为 240×240）。
3) 如果素材颜色异常，检查：
   - 是否为 RGB565 格式
   - `tft.setSwapBytes(true)` 是否保留
   - `TFT_RGB_ORDER` 是否为 `TFT_BGR`

---

### 代码主流程（简述）

1) 初始化串口、屏幕与背光 PWM
2) 播放启动帧：`tft.pushImage(52, 56, 150, 151, frames[j])`（带延时控制节奏）
3) 显示欢迎图（6 秒）
4) 清屏并显示错误框（4 秒）
5) 显示蓝屏（7 秒）

可在 `loop()` 调整显示顺序、时长与坐标。

---

### 常见问题（FAQ）

- **屏幕无显示或雪花**：
  - 检查接线，尤其是 `CS/DC/RST` 是否与定义一致
  - 降低 `SPI_FREQUENCY`（如 27MHz 或 40MHz）
  - 确认电源供给稳定（建议 USB 口/供电线质量）

- **颜色错乱**：
  - 检查素材导出为 RGB565
  - 保持 `tft.setSwapBytes(true)` 与 `TFT_RGB_ORDER TFT_BGR`

- **方向不对**：
  - 调整 `tft.setRotation(0/1/2/3)`

- **背光无法调节**：
  - 确认 `TFT_BL` 引脚接到 ESP32 的 `GPIO15`
  - `ledcSetup/ledcAttachPin/ledcWrite` 是否按通道一致

---

### 开发与自定义建议

- 需要兼容其他屏幕（如 ST7735/ST7789 不同尺寸），请修改 `include/tft_espi_setup.h` 中驱动与尺寸定义
- 如果素材较多且占用 FLASH，可考虑：
  - 使用 SPIFFS/LittleFS 存储图片数据
  - 或存放在 SD 卡并按需读取