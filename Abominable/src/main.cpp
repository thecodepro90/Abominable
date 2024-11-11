#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({9, 20, 5}, pros::v5::MotorGears::blue); 
pros::MotorGroup right_mg({-10, -19, -2}, pros::v5::MotorGears::blue);
pros::ADIDigitalOut piston(1);
pros::Motor intake(-12, pros::v5::MotorGears::blue);
pros::Motor hook(6, pros::v5::MotorGears::green);



void initialize() {
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

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

/* 
* Runs the drivetrain for int milliseconds and at the speed of int velocity then stops
* Make int velocity negative to go backwards
*/
void driveFor(int milliseconds, int velocity) {
	left_mg.move(velocity);
	right_mg.move(velocity);
	pros::delay(milliseconds);
	left_mg.move(0);
	right_mg.move(0);
}

void turnRightFor(int milliseconds) {
	left_mg.move(127);
    right_mg.move(-127);
    pros::delay(milliseconds);
    left_mg.move(0);
    right_mg.move(0);
}

void turnLeftFor(int milliseconds) {
	left_mg.move(-127);
    right_mg.move(127);
    pros::delay(milliseconds);
    left_mg.move(0);
    right_mg.move(0);
}

void runIntake(int milliseconds) {
	intake.move(-127);
	hook.move(-127);
    pros::delay(milliseconds);
    intake.move(0);
	hook.move(0);
}

void autonomous() {
	piston.set_value(false);
	driveFor(550, -127);
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
		left_mg.move(dir - turn);                      
		right_mg.move(dir + turn);                     

		
		

		pros::delay(20);                              
	}
}