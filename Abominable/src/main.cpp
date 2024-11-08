#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({10,19}); 
pros::MotorGroup right_mg({9, 20});
pros::ADIDigitalOut piston(1);
pros::Motor intake(16, pros::v5::MotorGears::blue);
pros::Motor hook(6, pros::v5::MotorGears::green);



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
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

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void toggle_piston_fn() {
	bool toggle = true;
	piston.set_value(toggle);
	while (true) {
		if (master.get_digital(DIGITAL_A)) {
			toggle = !toggle;
			piston.set_value(toggle);
		}
		
		pros::delay(115);   
	}
		
}

void toggle_intake_fn() {
	while (true) {
		if (master.get_digital(DIGITAL_L2)) 
		{
			intake.move_velocity(pros::E_MOTOR_GEAR_BLUE);
			hook.move_velocity(pros::E_MOTOR_GEAR_BLUE);
		}
		else if (master.get_digital(DIGITAL_L1)) 
		{
			intake.move_velocity(-pros::E_MOTOR_GEAR_BLUE);
			hook.move_velocity(-pros::E_MOTOR_GEAR_BLUE);
		}
		else
		{
			intake.brake();
			hook.brake();
		}
		
	}
}

void opcontrol() {
	

	pros::Task piston_toggle(toggle_piston_fn);
	pros::Task intake_toggle(toggle_intake_fn);
	

	while (true) {
		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage

		

		pros::delay(20);                               // Run for 20 ms then update
	}
}