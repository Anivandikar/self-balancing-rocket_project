#include <Wire.h>
#include <Servo.h>

Servo servo1, servo2; // X-axis
Servo servo3, servo4; // Y-axis

const int MPU = 0x68;
int16_t AcX, AcY, AcZ;

// Gains (increase authority)
float Kx = 6.0; // was 1.8 → too weak
float Ky = 6.0;

// Servo microseconds
const int SERVO_CENTER = 1500;
const int SERVO_RANGE = 350; // movement authority

void setup()
{
    Serial.begin(115200); // MUCH faster
    Wire.begin();

    // Wake up MPU6050
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    servo1.attach(9);
    servo2.attach(10);
    servo3.attach(11);
    servo4.attach(6);

    // Center servos
    servo1.writeMicroseconds(SERVO_CENTER);
    servo2.writeMicroseconds(SERVO_CENTER);
    servo3.writeMicroseconds(SERVO_CENTER);
    servo4.writeMicroseconds(SERVO_CENTER);

    Serial.println("Fast X + Y control running");
}

void loop()
{
    // Read accelerometer
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);

    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();

    // Compute angles
    float angleX = atan2(AcX, sqrt((long)AcY * AcY + (long)AcZ * AcZ)) * 57.2958;
    float angleY = atan2(AcY, sqrt((long)AcX * AcX + (long)AcZ * AcZ)) * 57.2958;

    // Clamp angles
    angleX = constrain(angleX, -40, 40);
    angleY = constrain(angleY, -40, 40);

    // Convert to microseconds
    int dx = angleX * Kx;
    int dy = angleY * Ky;

    int s1 = SERVO_CENTER + dx;
    int s2 = SERVO_CENTER - dx;
    int s3 = SERVO_CENTER + dy;
    int s4 = SERVO_CENTER - dy;

    // Safety limits
    s1 = constrain(s1, 1100, 1900);
    s2 = constrain(s2, 1100, 1900);
    s3 = constrain(s3, 1100, 1900);
    s4 = constrain(s4, 1100, 1900);

    // Write immediately (FAST)
    servo1.writeMicroseconds(s1);
    servo2.writeMicroseconds(s2);
    servo3.writeMicroseconds(s3);
    servo4.writeMicroseconds(s4);

    // Debug (comment out once confirmed)
    Serial.print(s1);
    Serial.print(" ");
    Serial.print(s2);
    Serial.print(" ");
    Serial.print(s3);
    Serial.print(" ");
    Serial.println(s4);
}
