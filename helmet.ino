// Library and code lifted from: GIT: https://github.com/jarzebski/Arduino-MPU6050

#include <Wire.h>
#include <MPU6050.h>

// Set mpu as a MPU650 
MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;
int newpitch, newroll, newyaw, mappedroll, mappedyaw;

void setup() 
{
  Serial.begin(38400);
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  } 
  // Calibrate gyroscope.
  mpu.calibrateGyro();
  // Set threshold sensivty.
  mpu.setThreshold(1);
}

 
void loop()
{
  timer = millis();

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  // Typecast values to integers
  newpitch = (int)pitch;
  newroll = (int)roll;
  newyaw = (int)yaw;

  // Map values to my specified ranged
  mappedroll = map(newroll,-90,90,180,0);
  mappedyaw = map(newyaw,-90,90,180,0);

  // If outside of ranges, set to the limit
  if (mappedroll>179){
     mappedroll = 179;
      }
  else if (mappedroll<1){
    mappedroll = 1;
      }
  if (mappedyaw>179){
    mappedyaw = 179;
      }
  else if (mappedyaw<1){
    mappedyaw = 1;
      }

  // Serial output        bitwise all data, ensures 8 bits
  mappedroll = mappedroll & 0xFF;
  mappedyaw = (mappedyaw) & 0xFF;
  Serial.write(mappedroll);
  Serial.write(mappedyaw);

  // Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));

  
}
