//
// Created by ljw98 on 11/22/2018.
//

#ifndef SMARTDESK_MOTORCONTROLLER_H
#define SMARTDESK_MOTORCONTROLLER_H

#include <Arduino.h>

/*
 * General idea: uses 4 relays to apply forward / reverse the motion to a motor.
 *                  ------------
 *                +              -
 * 1           pwrPin1        gndPin1
 * 2           gndPin2        pwrPin2
 *
 * Enable pins on line 1 & disable pins on line 2 should create forward motion.
 * Enable pins on line 2 & disable pins on line 1 should create reverse motion.
 * Disable all pins should stop all motion.
 */

class MotorController {
public:
    MotorController(int pwrPinOne, int pwrPinTwo, int gndPinOne, int gndPinTwo);
    void forward();
    void reverse();
    void stop();
private:
    int powerPinOne;
    int powerPinTwo;
    int groundPinOne;
    int groundPinTwo;
};

#endif //SMARTDESK_MOTORCONTROLLER_H
