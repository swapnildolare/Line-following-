#include "line_sensor.h"
#include "motor.h"
#include "kinematics.h"
#include "odometry.h"

line_sensor ls;
Motor motor;
Kinematics_c kina;

#define kp 200
#define LINE_UPDATE 30
#define MOTOR_UPDATE 30

unsigned long  sensor_timer = millis();
unsigned long motor_timer = millis();

int stage = 1;
#define state_comeoutofbox   1
#define state_joinline       2
#define state_linefollowing  3
#define state_home           4

void state() {
  stage += 1;
}

void lineFollowing() {
  unsigned long elapsed_t;
  unsigned long current_ts;
  current_ts = millis();
  float e_line;
  elapsed_t = current_ts - sensor_timer;
  if (elapsed_t > LINE_UPDATE) {
    ls.read_all_sensor();
    error = ls.eline();
    sensor_timer = millis();
  }
  float motor_compensate = error * kp;
  elapsed_t = current_ts - motor_timer;
  if (elapsed_t > MOTOR_UPDATE) {
    motor.motorpower((17 - motor_compensate), (17 + motor_compensate));
    motor_timer = millis();
    Serial.println("line");
  }
}

void setup() {
  Serial.begin(9600);
  setupEncoder0();
  setupEncoder1();
}

void loop() {
  if (stage == state_comeoutofbox) {
    bool line = false;
    while (!line) {
      line = ls.line_detected();
      motor.motorpower(20, 20);
    }
    motor.motorpower(0, 0);
    delay(1000);
    state();
  }

  else if (stage == state_joinline) {
    motor.motorpower(0, 30);
    delay(300);
    state();
  }
  else if (stage == state_linefollowing) {
    kina.update();
    unsigned long lineTime;
    bool follow = true;
    while (true) {
      kina.update();
      float dist = sqrt(pow(kina.x_post, 2) + pow(kina.y_post, 2));
      if (dist < 400) {
        if (ls.line_detected()) {
          lineTime = millis();
        }
        if (millis() - lineTime > 600) {
          motor.motorpower(-20, 20);
        }
      }
      else if (dist > 2000) {
        if (ls.line_detected()) {
          lineTime = millis();
        }
        if (millis() - lineTime > 1000) {
          motor.motorpower(0, 0);
          state();
          break;
        }
      }
      lineFollowing();
    }
  }

  else if (stage == state_home) {
    delay(1000);
    motor.motorpower(0, 0);
    kina.update();
    float alpha = atan2(kina.y_post, kina.x_post);

    if (alpha > 0) {
      motor.motorpower(20, -20);
      delay(300);
      motor.motorpower(40, 40);
      delay(7500);
      motor.motorpower(0, 0);
      state();
    }
    else if (alpha < 0) {
      motor.motorpower(-20, 20);
      delay(1400);
      motor.motorpower(40, 40);
      delay(7500);
      motor.motorpower(0, 0);
      state();
    }
  }
  else {
    motor.motorpower(0, 0);
  }
}
}
