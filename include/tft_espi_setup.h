#define ST7789_DRIVER

#define TFT_CS       5
#define TFT_MOSI     23
#define TFT_SCLK     18
#define TFT_DC       2
#define TFT_RST      4
#define TFT_BL       15
#define TOUCH_CS     16

#define TFT_WIDTH    240
#define TFT_HEIGHT   240 

#define LOAD_GLCD       // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2      // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4      // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6      // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7      // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8      // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF      // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT

#define SPI_FREQUENCY       80000000
#define SPI_READ_FREQUENCY  6000000
#define SPI_TOUCH_FREQUENCY 2500000

#define TFT_RGB_ORDER TFT_BGR  // TFT_BGR FOR Blue-Green-Red + TFT_RGB FOR Red-Green-Blue