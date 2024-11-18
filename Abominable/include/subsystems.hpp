#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');
inline pros::ADIDigitalOut piston (1);
inline pros::Motor intake(1, pros::v5::MotorGear::blue);
inline pros::Motor hook(1, pros::v5::MotorGear::green);