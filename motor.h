#ifndef _MOTOR_H
#define _MOTOR_H
#define left_motor    10
#define right_motor   9
#define left_dir      16
#define Right_dir     15
#define fwd LOW
#define rev HIGH
#define upper_limit 30
#define lower_limit 15


class Motor {
  public:
    Motor () {
    }
    void startmotor() {
      pinMode(left_motor, OUTPUT);
      pinMode(right_motor, OUTPUT);
      pinMode(left_dir, OUTPUT);
      pinMode(Right_dir, OUTPUT);

    }
    void motorpower(float lpwm, float rpwm) {
      lpwm = ls_limit(lpwm);
      rpwm = rs_limit(rpwm);
      if (lpwm < 0 && rpwm < 0) {
        digitalWrite(left_dir, rev);
        digitalWrite(Right_dir, rev);
      }

      else if (lpwm > 0 && rpwm > 0) {
        digitalWrite(left_dir, fwd);
        digitalWrite(Right_dir, fwd);
      }

      else if (lpwm > 0 && rpwm < 0) {
        digitalWrite(left_dir, fwd);
        digitalWrite(Right_dir, rev);
      }

      else if (lpwm < 0 && rpwm > 0) {
        digitalWrite(left_dir, rev );
        digitalWrite(Right_dir, fwd);
      }
      analogWrite(left_motor, abs(lpwm));

      analogWrite(right_motor, abs(rpwm));

    }

    float ls_limit(float lpwm) {
      if (lpwm == 0) {
        lpwm = 0;
      }
      else {
        if (lpwm >= upper_limit) {

          lpwm = upper_limit;
        }
        else if (lpwm <= -upper_limit) {
          lpwm = - upper_limit;
        }

        else if (lpwm > 0 && lpwm < lower_limit) {
          lpwm = lower_limit;
        }

        else if (lpwm < 0 && lpwm > lower_limit) {
          lpwm = lower_limit;

        }
      }

      return lpwm;
    }

    float rs_limit(float rpwm) {

      if (rpwm == 0) {
        rpwm = 0;
      }
      else {
        if (rpwm >= upper_limit) {
          rpwm = upper_limit;
        }
        else if (rpwm <= -upper_limit) {
          rpwm = - upper_limit;
        }

        else if (rpwm > 0 && rpwm < lower_limit) {
          rpwm = lower_limit;
        }

        else if (rpwm < 0 && rpwm > lower_limit) {
          rpwm = lower_limit;

        }

      }

      return rpwm;

    }
};

#endif
