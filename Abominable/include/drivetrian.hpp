#include <vector>
#include "pros/motor_group.hpp"

class DriveTrain {
private:
    std::vector<std::int8_t> left_ports;
    std::vector<std::int8_t> right_ports;
	pros::v5::MotorGear gear_ratio;
	
	pros::MotorGroup left_mg;
	pros::MotorGroup right_mg;

public:

    DriveTrain(const std::vector<std::int8_t>& x, const std::vector<std::int8_t>& y, pros::v5::MotorGear z) 
	: left_ports(x), right_ports(y), gear_ratio(z) ,
	  left_mg(left_ports, gear_ratio), right_mg(right_ports, gear_ratio) {}


	void drive(int left_speed, int right_speed) {
        left_mg.move(left_speed);
        right_mg.move(right_speed);
    }

	void driveFor(int milliseconds, int velocity) {
		drive(velocity, velocity);
		pros::delay(milliseconds);
		stop();
	}

	void turnRightFor(int milliseconds) {
		drive(127, -127);
		pros::delay(milliseconds);
		stop();
	}

	void turnLeftFor(int milliseconds) {
		drive(-127, 127);
		pros::delay(milliseconds);
		stop();
	}

	void stop() {
        drive(0, 0);
    }


};