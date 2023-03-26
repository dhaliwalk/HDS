#include <iostream>

#include "heat_sampler.h"
#include "capture.h"
#include "webcam.h"
#include "pwm.h"

int main() {
    std::cout << "Hello BeagleBone!\n";
    ShutdownManager shutdownManager;

    // Sample the temperature at 1 Hz and calculate window average over the last 10 samples.
    HeatSampler heatSampler(&shutdownManager, 1, 10, true);
    // Construct like this instead if you want the sampler to not print anything:
    // HeatSampler heatSampler(&shutdownManager, 1, 10);
    Webcam webcam(&shutdownManager);

    PWM pwm(&shutdownManager);
    webcam.waitForShutdown();
    pwm.waitForShutdown();
    heatSampler.waitForShutdown();

    return 0;
}