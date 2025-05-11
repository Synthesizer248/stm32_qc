#ifndef STM32_QC_H
#define STM32_QC_H

#include <Arduino.h>
#include <U8g2lib.h>

class STM32_QC {
public:
    STM32_QC(uint8_t encA, uint8_t encB, uint8_t encSW, 
            uint8_t dacDP, uint8_t dacDM,
            U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display);
    
    void begin();
    void update();
    void setVoltageProfile(uint8_t profile);
    uint16_t getSelectedVoltage();
    uint8_t getQCVersion();

private:
    void initDAC();
    void encoderISR();
    void updateDisplay();
    
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C &u8g2;
    uint8_t _encA, _encB, _encSW;
    uint8_t _dacDP, _dacDM;
    volatile int8_t enc_dir;
    volatile bool btn_pressed;
    uint8_t selected_qc;
    
    typedef struct {
        uint16_t voltage;
        uint16_t dacDP;
        uint16_t dacDM;
    } QCProfile;
    
    QCProfile profiles[4];
};

#endif