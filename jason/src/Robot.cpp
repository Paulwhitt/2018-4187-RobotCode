#include <iostream>
#include <memory>
#include <string>
#include <Spark.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <CameraServer.h>
#include <Encoder.h>
#include "ADXRS450_Gyro.h"
/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will
 * automatically call your Autonomous and OperatorControl methods at the right
 * time as controlled by the switches on the driver station or the field
 * controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're
 * inexperienced, don't. Unless you know what you are doing, complex code will
 * be much more difficult under this system. Use IterativeRobot or Command-Based
 * instead if you're new.
 */
class Robot: public frc::SampleRobot {
	frc::RobotDrive driveMotors { 0, 1 , 2 , 3} ;// robot drive system
	frc::Joystick stick { 0 }; // only joystick
	frc::SendableChooser<std::string> chooser;
    frc::Spark* fourthMotor = new frc::Spark(4);
    frc::Spark* fifthMotor = new frc::Spark(5);
    frc::Encoder* armEncoder = new frc::Encoder( 0 , 1 , true );
    frc::ADXRS450_Gyro* gyro = new frc::ADXRS450_Gyro();
    int armValue;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot() {
	//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
		driveMotors.SetExpiration(0.1);
	}

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		CameraServer::GetInstance()->StartAutomaticCapture();
		armEncoder->Reset();
		gyro->Reset();
	}

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void Autonomous() {
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			driveMotors.SetSafetyEnabled(false);
			driveMotors.Drive(-0.5, 1.0 ); // spin at half speed
			frc::Wait(2.0);                // for 2 seconds
			driveMotors.Drive(0.0, 0.0);  // stop robot
		} else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			driveMotors.SetSafetyEnabled(false);
			driveMotors.Drive(-0.5, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			driveMotors.Drive(0.0, 0.0);  // stop robot
		}
	}

	/*
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl() override {
		driveMotors.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled()) {
			// drive with arcade style (use right stick)
			driveMotors.TankDrive(stick,1,stick,5);
			fourthMotor->Set(stick.GetRawAxis(2));
			fifthMotor->Set(stick.GetRawAxis(3));
			// wait for a motor update time
			frc::Wait(0.005);
		}
	}

	/*
	 * Runs during test mode
	 */
	void Test() override {
		Wait(3);
		while(true){
			armValue = armEncoder->GetRaw();
			frc::SmartDashboard::PutNumber("armEncoder", armValue);
			frc::SmartDashboard::PutNumber("gyroValue", gyro->GetAngle());
			if ( armValue < 4000)
				fifthMotor->Set(0.2);
			else
				fifthMotor->Set(0);
		}
	}
};

START_ROBOT_CLASS(Robot)
