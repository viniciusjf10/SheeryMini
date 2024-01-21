#ifdef PIO_UNIT_TESTING
// #include "ArduinoFake.h"
// #include "EspFake.h"
#include "Arduino.h"
#else
#include "Arduino.h"
#endif
#include "motor_control.h"

MotorConfig::MotorConfig(int pwm_pin, int in1_pin, int in2_pin, float speed_const, int channel)
{
    this->in1_pin = in1_pin;
    this->in2_pin = in2_pin;
    this->pwm_pin = pwm_pin;
    this->speed_const = speed_const;
    this->channel = channel;
}

MotorControl::MotorControl(int pwm_pin, int in1_pin, int in2_pin, float speed_const, int channel) : config(pwm_pin, in1_pin, in2_pin, speed_const, channel) {}

/* power should be between -100 and 100*/
void MotorControl::setPower(int power)
{
    this->power = power;
    float fixed_power = power * this->config.speed_const;

    // convert power to pwm between 0 and 255
    int pwm = (int)(this->max_power / (100.0) * 255.0 * ((float)abs(fixed_power) / 100.0));
    if (fixed_power < 0 && fixed_power >= -100)
    {
        digitalWrite(this->config.in1_pin, HIGH);
        digitalWrite(this->config.in2_pin, LOW);
    }
    else if (fixed_power >= 0 && fixed_power <= 100)
    {
        digitalWrite(this->config.in1_pin, LOW);
        digitalWrite(this->config.in2_pin, HIGH);
    }
    else
    {
        Serial.println("INVALID POWER VALUE");
        return;
    }
    Serial.print("PWM: ");
    Serial.println(pwm);
    Serial.print("Chanell: ");
    Serial.println(this->config.channel);
    // dacWrite(this->config.pwm_pin, pwm);
    ledcWrite(this->config.channel, pwm);
}