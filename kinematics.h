// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _KINEMATICS_H
#define _KINEMATICS_H
#include "odometry.h"

// Class to track robot position.
class Kinematics_c {
  public:

    // Constructor, must exist.
    Kinematics_c() {
    }
    const float Pi = 3.141593;
    float count_per_rot = 358.3;
    float wheel_radius = 16;       // in mm
    float wheel_distance = 40.75;  // l in mm
    float dst_per_rotation = 2 * Pi * wheel_radius;
    float dst_per_count = dst_per_rotation / count_per_rot;
    float x_pos;
    float x_post;
    float y_post;
    float y_pos = 0.0;
    float theta_r;

    void update() {
      float lft_encoder_dst = dst_per_count * count_e0 * (-1);
      float rgt_encoder_dst = dst_per_count * count_e1 * (-1);

      float x_r = lft_encoder_dst / 2 + rgt_encoder_dst / 2;

      theta_r = lft_encoder_dst / (2 * wheel_distance) - rgt_encoder_dst / (2 * wheel_distance);

      float x_pos = x_pos + x_r * cos(theta_r);
      float y_pos = y_pos + x_r * sin(theta_r);

      x_post = x_pos;
      y_post = y_pos;

      //      Serial.print(x_pos);
      //      Serial.print(", ");
      //      Serial.print(y_pos);
      //      Serial.print(", ");
      //      Serial.println(theta_r);
    }
};

#endif
  
