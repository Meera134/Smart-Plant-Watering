#include <Servo.h>

/* ===== Servo Objects ===== */
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo nozzleServo;

/* ===== Pins ===== */
const int basePin = 3;
const int shoulderPin = 5;
const int elbowPin = 6;
const int nozzlePin = 9;
const int relayPin = 7;

/* ===== Sensors ===== */
const int sensors[4] = {A0, A1, A2, A3};

/* ===== Base Angles ===== */
int plantAngles[4] = {30, 60, 120, 150};

/* ===== Calibration ===== */
int dryValue = 900;
int wetValue = 350;
int threshold = 40;

/* ===== Moisture Function ===== */
int getMoisture(int pin)
{
  int raw = analogRead(pin);
  int percent = map(raw, dryValue, wetValue, 0, 100);
  percent = constrain(percent,0,100);
  return percent;
}

/* ===== Move Arm To Root ===== */
void moveToRoot(int angle)
{
  digitalWrite(relayPin, HIGH); // Pump OFF while moving

  baseServo.write(angle);
  delay(1500);

  shoulderServo.write(70);
  elbowServo.write(110);
  delay(1000);

  nozzleServo.write(120);
  delay(800);
}

/* ===== Water Plant ===== */
void waterPlant(int index)
{
  moveToRoot(plantAngles[index]);

  int moisture = getMoisture(sensors[index]);

  Serial.print("Watering Plant ");
  Serial.println(index+1);

  if(moisture < threshold)
  {
    digitalWrite(relayPin, LOW);   // Pump ON

    while(moisture < threshold)
    {
      delay(1500);

      moisture = getMoisture(sensors[index]);

      Serial.print("Moisture: ");
      Serial.println(moisture);
    }

    digitalWrite(relayPin, HIGH);  // Pump OFF

    Serial.println("Watering Completed");

    delay(3000);
  }
}

/* ===== Find Driest Plant ===== */
int findDriestPlant()
{
  int driestIndex = -1;
  int lowestMoisture = 101;

  for(int i=0;i<4;i++)
  {
    int moisture = getMoisture(sensors[i]);

    Serial.print("Plant ");
    Serial.print(i+1);
    Serial.print(" Moisture: ");
    Serial.println(moisture);

    if(moisture < threshold && moisture < lowestMoisture)
    {
      lowestMoisture = moisture;
      driestIndex = i;
    }
  }

  return driestIndex;
}

/* ===== Setup ===== */
void setup()
{
  Serial.begin(9600);

  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  nozzleServo.attach(nozzlePin);

  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH); // Pump OFF initially

  baseServo.write(0);
  shoulderServo.write(90);
  elbowServo.write(90);
  nozzleServo.write(90);

  Serial.println("Priority Watering Robot Started");
}

/* ===== Loop ===== */
void loop()
{
  while(true)
  {
    int plantIndex = findDriestPlant();

    if(plantIndex == -1)
    {
      Serial.println("All plants have enough moisture");
      break;
    }

    waterPlant(plantIndex);
  }

  delay(5000);
}