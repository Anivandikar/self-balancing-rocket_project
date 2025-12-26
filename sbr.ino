#include <Wire.h>
#include <Servo.h>

Servo servo1, servo2; // X-axis
Servo servo3, servo4; // Y-axis

const int MPU = 0x68;
long AcX = 0, AcY = 0, AcZ = 0;

// Gains
float Kx = 1.8; // X-axis
float Ky = 1.5; // Y-axis

void setup()
{
    Serial.begin(9600);
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
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);

    Serial.println("Starting fast X+Y demo...");
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
    float angleX = atan(AcX / sqrt((double)AcY * AcY + (double)AcZ * AcZ)) * 180.0 / PI;
    float angleY = atan(AcY / sqrt((double)AcX * AcX + (double)AcZ * AcZ)) * 180.0 / PI;

    // Compute servo positions directly (fast)
    int s1 = constrain(90 + angleX * Kx, 0, 180);
    int s2 = constrain(90 - angleX * Kx, 0, 180);
    int s3 = constrain(90 + angleY * Ky, 0, 180);
    int s4 = constrain(90 - angleY * Ky, 0, 180);

    // Write servos immediately
    servo1.write(s1);
    servo2.write(s2);
    servo3.write(s3);
    servo4.write(s4);

    // Debug print (optional, slows loop)
    // Serial.print("X="); Serial.print(angleX,2);
    // Serial.print(" Y="); Serial.print(angleY,2);
    // Serial.print(" | S1=");
    Serial.print(s1);
    // Serial.print(" S2=");
    Serial.print(" ");
    Serial.print(s2);
    Serial.print(" ");
    // Serial.print(" | S3=");
    Serial.print(s3);
    Serial.print(" ");
    // Serial.print(" S4=");
    Serial.println(s4);
}