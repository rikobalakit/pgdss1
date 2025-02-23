﻿//
// Created by rico on 5/28/2022.
//

/* USE THESE IN FUTURE, SPECIFY IN PLATFORMIO.INI
 * |-- <FastLED> 3.5.0
|-- <ESP32Servo> 0.11.0
|-- <PS3 Controller Host> 1.1.0
|-- <Adafruit BNO055> 1.5.3
|   |-- <Adafruit BusIO> 1.11.6
|   |   |-- <Wire> 1.0.1
|   |   |-- <SPI> 1.0
|   |-- <Wire> 1.0.1
|   |-- <Adafruit Unified Sensor> 1.1.5
|-- <Wire> 1.0.1
|-- <SPI> 1.0

 */

#include <Arduino.h>
#include <FastLED.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <SPI.h>

#ifndef S1_ROBOTFIRMWARE_MAIN_H
#define S1_ROBOTFIRMWARE_MAIN_H

#endif //S1_ROBOTFIRMWARE_MAIN_H


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Debug Values

#define LOG_CONTROLLER_INPUT false
#define LOG_CONTROLLER_ACCELEROMETER false
#define LOG_ORIENTATION false
#define LOG_VOLTAGE true
#define LOG_CONTROLLER_CONNECTED false
#define EVENT_MODE false


// System values

// Pinout reference
// green 13 GN
// orange 2 OR
// yellow 4 YL
// blue 12 BL
// purple 14 PR
// silver 15 GY
//SDA brown
//SDL white

#define PIN_NUM_NEOPIXEL_OUTPUT 4
#define PIN_DRIVE_MOTOR_LEFT 12
#define PIN_DRIVE_MOTOR_RIGHT 2
#define PIN_WEAPON_MOTOR 14
#define PIN_WEAPON_MOTOR_COPY 13
#define PIN_VOLTAGE_CHECK 27
#define UPDATE_STEP_LENGTH_MILLIS 10

// Orientation Sensor
//VARIABLES I CANT RENAME!??
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int val;    // variable to read the value from the analog pin. can't be renamed because dumb.
sensors_event_t event; //I dont think this can be renamed either
int _analogPinValue = 0; // Don't want to rename or get rid of
float _currentXOrientation = 0;
bool _isUpsideDown = false;
bool _previousCalibrationWasUnderTwo = true;
float _currentXOrientationOffset = 0;
float _previousEventOrientationX = -1;
int _eventOrientationXDelta = 0;
bool _useIMU = true;
uint8_t overallSystem, gyro, accel, _magnetometerCalibrationLevel;

// Controller Button Readings
bool _controllerEmergencyShutdownButtonPressed;
bool _controllerTurboModeButtonPressed;
bool _controllerSlowModeButtonPressed;

bool _controllerDirectionUpPressed;
bool _controllerDirectionDownPressed;
bool _controllerDirectionLeftPressed;
bool _controllerDirectionRightPressed;

int _controllerLeftStickX;
int _controllerLeftStickY;
int _controllerRightStickX;
int _controllerRightStickY;
int _controllerL2;
int _controllerR2;

float _leftTriggerNormalizedValue = 0;
float _rightTriggerNormalizedValue = 0;

// Controller Accelerometer Readings
int _controllerCurrentAx = 0;
int _controllerCurrentAy = 0;
int _controllerCurrentAz = 0;
int _controllerCurrentGz = 0;
int _controllerLastAx = 0;
int _controllerLastAy = 0;
int _controllerLastAz = 0;
int _controllerLastGz = 0;

// LED
#define TOTAL_LED 64
#define MAX_BRIGHTNESS 80
#define HUE_BLUE 171

#define SAT_FORWARD 0
#define HUE_FORWARD 96

#define HUE_NEUTRAL 213
#define SAT_NEUTRAL 255

#define HUE_REVERSE 0
#define SAT_REVERSE 255

#define HUE_DISCONNECTED 45

#define HUE_PURPLE 213
#define STARTING_BRIGHTNESS 40

CRGB leds[TOTAL_LED];
int _driveLeftLEDs[5] = {39, 31, 23, 15, 7};
int _driveRightLEDs[5] = {32, 24, 16, 8, 0};

// ESC Declarations and true values

#define DRIVE_POWER_MAX 100 //Range 0 to 100, 100 being full power
#define WEAPON_POWER_MAX 66 //Range 0 to 100, 100 being full power

#define CENTER_TRIM_DRIVE_ESC -3.0
#define CENTER_TRIM_WEAPON_ESC 0.0

bool _motorsAreAttached = false;

float _worldLeftMotorUnthrottledNormalizedSpeed = 0;
float _worldLeftMotorThrottledNormalizedSpeed = 0;

float _worldRightMotorUnthrottledNormalizedSpeed = 0;
float _worldRightMotorThrottledNormalizedSpeed = 0;

Servo _driveMotorLeft;
float _targetLocalLeftMotorNormalizedSpeed = 0;
float _currentLocalLeftMotorNormalizedSpeed = 0;


Servo _driveMotorRight;
float _targetLocalRightMotorNormalizedSpeed = 0;
float _currentLocalRightMotorNormalizedSpeed = 0;

Servo _weaponMotor;
Servo _weaponMotorCopy;
float _weaponMotorUnthrottledNormalizedSpeed = 0;
float _weaponMotorThrottledNormalizedSpeed = 0;
float _targetWeaponMotorNormalizedSpeed = 0;
float _currentWeaponMotorNormalizedSpeed = 0;
float _currentRollAngle = 0;

// Drive hypothetical or interpretation
#define DETHROTTLE_WEAPON_FACTOR 4 //last 10 // higher = harder throttle. lower = lighter throttle. 0 = none.
#define DETHROTTLE_WEAPON_HARD_MINIMUM 0.4 // last 0.5 // minimum multiplier
#define DETHROTTLE_WEAPON_ON_HARD_TURN true
#define DETHROTTLE_ANGLE_MINIMUM 10

#define SLOW_MULTIPLIER 0.5
#define TURBO_MULTIPLIER 1
#define DEFAULT_MULTIPLIER 0.999

#define VIDEO_GAME_DRIVE_ANGLE_TOLERANCE 3
#define VIDEO_GAME_DRIVE_ANGLE_CORRECT_FACTOR 0.5 // smaller = less correction, bigger = more correction
#define DRIVE_NEUTRAL_CLAMP_TO_ZERO 0.025
#define ANALOG_CONTROLLER_CLAMP_TO_ZERO 2
#define DRIVE_SPEED_SMOOTHING_FACTOR 0.25 // smaller = slower, bigger = faster
#define WEAPON_SPEED_SMOOTHING_FACTOR 0.1 // smaller = slower, bigger = faster
#define DRIVE_PERFECT_FORWARD_CORRECTION_FACTOR 0.3 // smaller = less correction, bigger = more correction
#define GYRO_LIFT_ANGLE_TOLERANCE 5

bool _reverseAllDriveMotors = true; // RPB: This is the case of like if we have a gear
bool _linearDirectionStabilizationEnabled = false;
float _perfectForwardStartAngle = 0;
bool _videoGameStyleDriveControls = true;
bool _currentlyDoingVideoGameStyleControlInsteadOfDPadOrTank = false;
int _videoGameAngleDeltaDegrees = 0;
int _currentRightJoystickAngle = 0;


// Emergency Shutdown
#define TIME_TO_HOLD_SHUTDOWN_BUTTON_MILLIS 3000
ulong _forceShutdownAccumulatedMillis = 0;
ulong _forceShutdownStartedMillis = 0;
float _forceShutdownProgress = 0;
bool _forceShutdownTurnedOn = false;

// Controller Constants
#define CONTROLLER_TIMEOUT_THRESHOLD_MILLIS 1000
#define CONTROLLER_RUMBLE_REFRESH_RATE_MILLIS 25
ulong _lastControllerAccelerometerChangedTime = 0;
ulong _controllerTimeoutDeclaredStartedMillis = 0;

// Battery Status
#define VOLTAGE_CUTOFF_EVERYTHING  3.3 //BS1 13.2v
#define VOLTAGE_CUTOFF_WEAPON  3.5 //BS2 14v
#define VOLTAGE_CUTOFF_HALFWAY  3.75 //BS3 15v
#define VOLTAGE_CUTOFF_FULL  4.1 //BS4 16.4v
#define VOLTAGE_CUTOFF_OVERLIMIT  4.25 //BS5 17v

#define BATTERY_STAGE_UNDEFINED -1
#define BATTERY_STAGE_DEAD 0
#define BATTERY_STAGE_CUTOFF_WEAPON 1
#define BATTERY_STAGE_UNDERHALF 2
#define BATTERY_STAGE_OVERHALF 3
#define BATTERY_STAGE_FULL 4
#define BATTERY_STAGE_OVERFULL 5

int _currentBatteryStage = BATTERY_STAGE_UNDEFINED;
ulong _accumulatedTimeSinceBatteryStageSwitched = 0;
int _voltageResultRawValue = 0;
float _calculatedVoltageVolts;
float _calculatedVoltagePerCell = 0;
int _rawReadings [100];
int _readingsIndex = 0;
int _esp32Id = -1; // -1 is default, -2 is error, 1 is actual start
ulong _lastTimeRumbleUpdated = 0;

void Start();

void Update();

void setup();

void loop();

void AttachMotors(bool shouldAttachMotors);

void SetDriveMotorSpeed(float newSpeed, bool isLeftMotor, int motorLeds[]);

void SetWeaponMotorSpeed(float newSpeed);

void ReadVoltage();

void ReadOrientation();

void SetCenterLEDs(CRGB color);

void UpdateCenterLEDColors();

int GetBrightnessForSpeed(float speed);

void OnForceShutDown(ulong shutdownEventReasonStartTime);

void ReadControllerInput();

void OnControllerDataTimedOut();

void UpdateShutdownTimer();

bool IsControllerDataTimedOut();

float AttenuateWeaponThrottleBasedOnAngleDelta(float inputTargetWeaponSpeed);

void InterpretControllerInput();

void UpdateTargetSignalValues();

void UpdateESCsToTargetValues();

void LogOrientation();

void LogVoltage();

void LogControllerInputReadings();

void LogControllerAccelerometerReadings();

void UpdateAllLEDs();

void ClearAllLEDs();

void InterpretOrientation();

void InterpretVoltage();

void InitializeSerialCommunication();

void InitializeLEDs();

void BootAnimation();

void UpdateDriveLEDs(float newSpeed, bool isLeftMotor, int motorLeds[]);

void UpdateWeaponLEDs(float newSpeed);

void InitializeController();

void CheckAndInitializeOrientationSensor();

void InterpretShutdown();

void InterpretAccelerometerChange();

bool IsAnyDPadButtonPressed();

void InterpretDPadInputToDrive();

void InterpretAnalogSticksAsVideoGameDrive();

void InterpretAnalogSticksAsTankDrive();

void InterpretTriggersForWeaponMotorControl();

void InitializePWMTimers();

void UpdateDriveTargetSignalValues();

void InitializeMotorSpeeds();

void UpdateWeaponTargetSignalValues();