#include "pwm.h"
#include "string"
#include <fstream>

const std::string PWM0_PATH = "/sys/class/pwm/pwmchip3/pwm0/";

PWM::PWM(ShutdownManager *shutdownManager) {
    this->shutdownManager = shutdownManager;
    this->thread = std::thread(&PWM::run, this);
}

void PWM::waitForShutdown() {
    this->thread.join();
}

void PWM::writeToFile(std::string filename, std::string content) {
    std::ofstream file;
    file.open(filename);
    file << content;
    file.close();
}

void PWM::setPeriod(int period) {
    writeToFile(PWM0_PATH + "period", std::to_string(period));
}

void PWM::setDutyCycle(int dutyCycle) {
    writeToFile(PWM0_PATH + "duty_cycle", std::to_string(dutyCycle));
    this->currentDutyCycle = dutyCycle;
}

void PWM::enablePWM() {
    writeToFile(PWM0_PATH + "enable", "1");
}

void PWM::disablePWM() {
    writeToFile(PWM0_PATH + "enable", "0");
}

void PWM::moveLeft() {
    if (this->currentDutyCycle - DUTY_CYCLE_CHANGE_DIRECTION < DUTY_CYCLE_0) {
        setDutyCycle(DUTY_CYCLE_0);
    } else {
        setDutyCycle(this->currentDutyCycle - DUTY_CYCLE_CHANGE_DIRECTION);
    }
}

void PWM::moveRight() {
    if (this->currentDutyCycle + DUTY_CYCLE_CHANGE_DIRECTION > DUTY_CYCLE_180) {
        setDutyCycle(DUTY_CYCLE_180);
    } else {
        setDutyCycle(this->currentDutyCycle + DUTY_CYCLE_CHANGE_DIRECTION);
    }
}

void PWM::run() {
    setPeriod(PERIOD_NS);
    enablePWM();
    // for (int i = 0; i < 10; i++) {
    //     moveLeft();
    //     sleepForMs(1000);
    //     moveRight();
    //     sleepForMs(1000);
    // }
    if (shutdownManager->isShutdownRequested()) {
        disablePWM();
    }
}