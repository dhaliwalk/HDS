#include <string>
#include <stdexcept>
#include "lock.h"
#include "adc.h"

Adc::Adc(uint8 analogInputNum)
{
    if (analogInputNum > maxAnalogInputNum) {
        throw std::invalid_argument(
            "Invalid analogInputNum -- must be in range [0," + std::to_string(maxAnalogInputNum) + "]."
        );
    }
    filePath =
        "/sys/bus/iio/devices/iio:device0/in_voltage" +
        std::to_string(analogInputNum) +
        "_raw";
}

int16 Adc::read()
{
    adc_lock.lock();
    analogInputFile.open(filePath);
    int16 val = -1;
    analogInputFile >> val;
    // TODO: Check if val is in valid range [0, 4095] and log error if not.

    // Clear EOF flag.
    analogInputFile.clear();
    // Set file position back to start of the file.
    analogInputFile.seekg(0, analogInputFile.beg);
    analogInputFile.close();
    adc_lock.unlock();
    return val;
}

double Adc::convertToVolts(double analogInputVal)
{
    if (analogInputVal > maxInputVal || analogInputVal < minInputVal) {
        throw std::invalid_argument(
            "analogInputVal = " +
            std::to_string(analogInputVal) +
            ", but analogInputVal must be in range [" +
            std::to_string(minInputVal) +
            ", " +
            std::to_string(maxInputVal) +
            "]"
        );
    }

    return (analogInputVal / maxInputVal) * maxInputVoltage;
}

Adc::~Adc()
{
}