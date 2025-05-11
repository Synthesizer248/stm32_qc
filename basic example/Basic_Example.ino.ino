#include <stm32_qc.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

STM32_QC qcController(
    PA0, PA1, PA2,  // Encoder pins
    PA4, PA5,        // DAC pins
    u8g2            // Display instance
);

void setup() {
    qcController.begin();
}

void loop() {
    qcController.update();
}