#ifndef CONFIG_H
#define CONFIG_H

// Пины для NEMA шаговых двигателей (верхний и нижний сортировочные диски)
#define STEPPER_UPPER_STEP_PIN  18 //19
#define STEPPER_UPPER_DIR_PIN   19 //18

#define STEPPER_UPPER_STEPS_PER_REVOLUTION 1600

#define STEPPER_UPPER_REVERSE true
#define STEPPER_UPPER_SPEED_DEG_PER_SEC 2000
#define STEPPER_UPPER_ACC_DEG_PER_SEC2 5000



#define STEPPER_LOWER_STEP_PIN  22
#define STEPPER_LOWER_DIR_PIN   23

#define STEPPER_LOWER_STEPS_PER_REVOLUTION 1600

#define STEPPER_LOWER_REVERSE false
#define STEPPER_LOWER_SPEED_DEG_PER_SEC 2000
#define STEPPER_LOWER_ACC_DEG_PER_SEC2 5000



#define STEPPER_PICKER_STEP_PIN  16
#define STEPPER_PICKER_DIR_PIN   17

#define STEPPER_PICKER_STEPS_PER_REVOLUTION 1600

#define STEPPER_PICKER_REVERSE false
#define STEPPER_PICKER_SPEED_DEG_PER_SEC 3000
#define STEPPER_PICKER_ACC_DEG_PER_SEC2 10000


// Пин для вибромотора
#define VIBR_PIN 14
#define VIBR_BASE_SPEED 255

// Пины для оптодатчиков и индивидуальные пороги срабатывания
#define OPTO_UPPER_PIN 13
#define OPTO_LOWER_PIN 14
#define OPTO_CAP_PIN   27

#define OPTO_UPPER_THRESHOLD 50
#define OPTO_LOWER_THRESHOLD 50
#define OPTO_CAP_THRESHOLD   100

// Параметры светодиодной ленты
#define LED_DATA_PIN 5
#define LED_NUM_LEDS 10
#define LED_TYPE WS2812B
#define LED_COLOR_ORDER GRB

#endif // CONFIG_H
