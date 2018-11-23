//
// Created by ljw98 on 11/22/2018.
//

#include "MotorController.h"

MotorController::MotorController(int pwrPinOne, int pwrPinTwo, int gndPinOne, int gndPinTwo) {
    powerPinOne = pwrPinOne;
    powerPinTwo = pwrPinTwo;
    groundPinOne = gndPinOne;
    groundPinTwo = gndPinTwo;
    pinMode(powerPinOne, OUTPUT);
    pinMode(powerPinTwo, OUTPUT);
    pinMode(gndPinOne, OUTPUT);
    pinMode(gndPinTwo, OUTPUT);
}

void MotorController::forward() {
    digitalWrite(powerPinOne, HIGH);
    digitalWrite(powerPinTwo, LOW);
    digitalWrite(groundPinOne, HIGH);
    digitalWrite(groundPinTwo, LOW);
}

void MotorController::reverse() {
    digitalWrite(powerPinOne, LOW);
    digitalWrite(powerPinTwo, HIGH);
    digitalWrite(groundPinOne, LOW);
    digitalWrite(groundPinTwo, HIGH);
}

void MotorController::stop() {
    digitalWrite(powerPinOne, LOW);
    digitalWrite(powerPinTwo, LOW);
    digitalWrite(groundPinOne, LOW);
    digitalWrite(groundPinTwo, LOW);
}
