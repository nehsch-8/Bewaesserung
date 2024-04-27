/*
  bewaesserung.ino - Code for an automatic watering system with two optional moisture sensors.

  Created by Nehir Schmid, October 2022.

  Board: Arduino Nano
  Processor: ATmega328P
*/

int sensor_pin_1 = A7; // lower sensor connector on case
int sensor_pin_2 = A6; // upper sensor connector on case
int moist_pin_1 = 2; // Using D2 on Arduino NANO
int moist_pin_2 = 3; // Using D3 on Arduino NANO
int water_pin = 4; // using d4 on Arduino NANO
int moisture_value_1;
int moisture_value_2;
int moist_1;
int moist_2;
int t = 0;
bool sensor_mode = true;
bool watering = false;

/*
 * First function to run.
 */
void setup() {
  pinMode(sensor_pin_1, INPUT);
  pinMode(moist_pin_1, INPUT);
  pinMode(moist_pin_2, INPUT);
  pinMode(water_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Reading From the Sensor ...");
  delay(2000);
}

/*
 * Turning the water pump on
 */
void water_on() {
  digitalWrite(water_pin, HIGH);
  watering = true;
  Serial.println("Watering on");
  t=0;
}

/*
 * Turning the water pump off
 */
void water_off() {
  digitalWrite(water_pin, LOW);
  watering = false;
  Serial.println("Watering off");
}

/*
 * This loop runs with sensors with reasonable resistance values.
 */
void sensor_loop() {  
  if (!watering && (moisture_value_1 > 500 && moisture_value_2 > 500)) {
    water_on();
    delay(500);
  }
  else if (watering && (moisture_value_1 <= 300 || moisture_value_2 <= 300)) {
    water_off();
    delay(500);
  }
  delay(100);

  // moist_1 = digitalRead(moist_pin_1);
  // Serial.print("moist 1: ");
  // Serial.println(moist_1);
  // moist_2 = digitalRead(moist_pin_2);
  // Serial.print("moist 2: ");
  // Serial.println(moist_2);
  // delay(1000);
}

/*
 * This runs if the sensor resistances are so high that the sensors are most likely outside of the ground or missing completely.
 */
void timer_loop() {
  delay(1000);
  t = t+1;
  Serial.print("t = ");
  Serial.print(t);
  Serial.println(" min");
  if (t >= 10) {   // 24h = 86400s -> 89280 = 24h-2min
    water_on();
    delay(60000);     // water for 1 minute
    water_off();
    t = 0;
  }
}

/*
 * Normal loop that runs right after setup()
 */
void loop() {
  /* Reading the sensor values */
  moisture_value_1 = analogRead(sensor_pin_1); // air/open = 1023, water = >300
  moisture_value_2 = analogRead(sensor_pin_2); // air/open = 1023, water = >300
  /* logging the sensor values for debugging purposes */
  Serial.print("Mositure 1 : ");
  Serial.print(moisture_value_1);
  Serial.println("%");
  moisture_value_2 = analogRead(sensor_pin_2);
  Serial.print("Mositure 2 : ");
  Serial.print(moisture_value_2);
  Serial.println("%");

  /* checking whether the sensor values are in reasonable range */
  if (moisture_value_1 < 1000 || moisture_value_2 < 1000) {
    sensor_mode = true;
    sensor_loop();
  }
  
   /* too high resistance means non-working sensors *
    * in this case just water the plants every 24h  */
  else {
    if (sensor_mode == true) {
      water_off();
      sensor_mode = false;
      Serial.println("sensor mode off");
    }
    timer_loop();
  } 
}
