#include <Servo.h>

// Servo objects
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo nozzleServo;

// Pins
int basePin = 3;
int shoulderPin = 5;
int elbowPin = 6;
int nozzlePin = 9;

int relayPin = 7;

// Soil sensors
int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;

// Moisture threshold
int threshold = 500;

void setup()
{
  Serial.begin(9600);

  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  nozzleServo.attach(nozzlePin);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // pump OFF

  Serial.println("System Started");
}

void loop()
{
  // Read sensors
  int m1 = analogRead(sensor1);
  int m2 = analogRead(sensor2);
  int m3 = analogRead(sensor3);
  int m4 = analogRead(sensor4);

  Serial.print("Sensor1: "); Serial.print(m1);
  Serial.print(" Sensor2: "); Serial.print(m2);
  Serial.print(" Sensor3: "); Serial.print(m3);
  Serial.print(" Sensor4: "); Serial.println(m4);

  // Plant 1
  if (m1 > threshold)
  {
    Serial.println("Plant 1 Dry → Watering");

    baseServo.write(30);
    shoulderServo.write(80);
    elbowServo.write(100);
    nozzleServo.write(120);

    delay(2000);

    digitalWrite(relayPin, LOW); // pump ON
    delay(3000);
    digitalWrite(relayPin, HIGH); // pump OFF
  }

  // Plant 2
  else if (m2 > threshold)
  {
    Serial.println("Plant 2 Dry → Watering");

    baseServo.write(60);
    shoulderServo.write(80);
    elbowServo.write(100);
    nozzleServo.write(120);

    delay(2000);

    digitalWrite(relayPin, LOW);
    delay(3000);
    digitalWrite(relayPin, HIGH);
  }

  // Plant 3
  else if (m3 > threshold)
  {
    Serial.println("Plant 3 Dry → Watering");

    baseServo.write(120);
    shoulderServo.write(80);
    elbowServo.write(100);
    nozzleServo.write(120);

    delay(2000);

    digitalWrite(relayPin, LOW);
    delay(3000);
    digitalWrite(relayPin, HIGH);
  }

  // Plant 4
  else if (m4 > threshold)
  {
    Serial.println("Plant 4 Dry → Watering");

    baseServo.write(150);
    shoulderServo.write(80);
    elbowServo.write(100);
    nozzleServo.write(120);

    delay(2000);

    digitalWrite(relayPin, LOW);
    delay(3000);
    digitalWrite(relayPin, HIGH);
  }

  else
  {
    Serial.println("All Plants Moist");
    digitalWrite(relayPin, HIGH);

    baseServo.write(0);
    shoulderServo.write(90);
    elbowServo.write(90);
    nozzleServo.write(90);
  }

  delay(2000);
}