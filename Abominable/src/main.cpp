#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
// pros::MotorGroup left_mg({9, 20, 5}, pros::v5::MotorGears::blue); 
// pros::MotorGroup right_mg({-10, -19, -2}, pros::v5::MotorGears::blue);
pros::ADIDigitalOut piston(1);
pros::Motor intake(-12, pros::v5::MotorGears::blue);
pros::Motor hook(6, pros::v5::MotorGears::green);


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
		drive(0, 0);
	}

	void turnRightFor(int milliseconds) {
		drive(127, -127);
		pros::delay(milliseconds);
		drive(0, 0);
	}

	void turnLeftFor(int milliseconds) {
		drive(-127, 127);
		pros::delay(milliseconds);
		drive(0, 0);
	}


};

DriveTrain drivetrain({9, 20, 5}, {-10, -19, -2}, pros::v5::MotorGear::blue);


void initialize() {
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	master.rumble(".- ");
	pros::delay(1000);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}



void runIntake(int milliseconds) {
	intake.move(-127);
	hook.move(-127);
    pros::delay(milliseconds);
    intake.move(0);
	hook.move(0);
}

void autonomous() {
	piston.set_value(false);
	drivetrain.driveFor(550, -127);
	piston.set_value(true);
	runIntake(3000);
}

void toggle_piston_fn() {
	bool toggle = true;
	piston.set_value(toggle);
	while (true) {
		// if (master.get_digital(DIGITAL_A)) {
		// 	toggle = !toggle;
		// 	piston.set_value(toggle);
		// }

		if (master.get_digital(DIGITAL_A)) {
			piston.set_value(false);
		} else if (master.get_digital(DIGITAL_B)) {
			piston.set_value(true);
		}
		
		// pros::delay(115);   
	}
		
}

void toggle_intake_fn() {
	while (true) {
		if (master.get_digital(DIGITAL_L2)) 
		{
			intake.move(127);
			hook.move(127);
		}
		else if (master.get_digital(DIGITAL_L1)) 
		{
			intake.move(-127);
			hook.move(-127);
		}
		else
		{
			intake.move(0);
			hook.move(0);
		}
		
	}
}

void opcontrol() {
	

	pros::Task piston_toggle(toggle_piston_fn);
	pros::Task intake_toggle(toggle_intake_fn);
	

	while (true) {
		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X); 

		drivetrain.drive(dir - turn, dir + turn);

		// left_mg.move(dir - turn);                      
		// right_mg.move(dir + turn);                     

		
		

		pros::delay(20);                              
	}
}