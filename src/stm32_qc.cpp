#include "stm32_qc.h"

STM32_QC::STM32_QC(uint8_t encA, uint8_t encB, uint8_t encSW,
                 uint8_t dacDP, uint8_t dacDM,
                 U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display)
    : u8g2(display) {
    _encA = encA;
    _encB = encB;
    _encSW = encSW;
    _dacDP = dacDP;
    _dacDM = dacDM;
    
    // Default QC3.0 profiles
    profiles[0] = {5000, 322, 322};
    profiles[1] = {9000, 322, 186};
    profiles[2] = {12000, 322, 124};
    profiles[3] = {20000, 322, 74};
}

void STM32_QC::begin() {
    // DAC Setup
    pinMode(_dacDP, OUTPUT);
    pinMode(_dacDM, OUTPUT);
    initDAC();

    // Encoder Setup
    pinMode(_encA, INPUT_PULLUP);
    pinMode(_encB, INPUT_PULLUP);
    pinMode(_encSW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_encA), 
                  [this] { this->encoderISR(); }, CHANGE);

    // Display Setup
    u8g2.begin();
    u8g2.setFont(u8g2_font_profont10_tf);
}

void STM32_QC::initDAC() {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5);
    DAC->CR |= DAC_CR_EN1 | DAC_CR_EN2;
}

void STM32_QC::encoderISR() {
    static uint8_t last_AB = 0;
    uint8_t new_AB = (digitalRead(_encA) << 1) | digitalRead(_encB);
    if(new_AB == last_AB) return;
    
    enc_dir = (last_AB == 0x01 && new_AB == 0x03) ? 1 : 
             (last_AB == 0x03 && new_AB == 0x02) ? 1 : 
             (last_AB == 0x02 && new_AB == 0x00) ? 1 : -1;
    last_AB = new_AB;
}

void STM32_QC::update() {
    static uint32_t last_update = 0;
    
    if(enc_dir != 0) {
        selected_qc = (selected_qc + enc_dir + 4) % 4;
        enc_dir = 0;
        last_update = millis();
    }

    if(!digitalRead(_encSW)) {
        if(!btn_pressed) {
            btn_pressed = true;
            setVoltageProfile(selected_qc);
        }
    } else {
        btn_pressed = false;
    }

    if(millis() - last_update > 100) {
        updateDisplay();
        last_update = millis();
    }
}

void STM32_QC::setVoltageProfile(uint8_t profile) {
    // Assembly-optimized DAC write
    asm volatile(
        "movw r0, %0\n\t"
        "movw r1, %1\n\t"
        "strh r0, [%2]\n\t"
        "strh r1, [%3]"
        :
        : "r" (profiles[profile].dacDP), 
          "r" (profiles[profile].dacDM),
          "r" (&DAC->DHR8R1), 
          "r" (&DAC->DHR8R2)
        : "r0", "r1"
    );
}

// Other member functions...