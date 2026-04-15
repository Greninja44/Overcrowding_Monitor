#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define IR1 2
#define IR2 3
#define TRIG 4
#define ECHO 5
#define MQ A0
#define BUZZER 10
int count = 0;
int maxLimit = 10;
int state = 0;
unsigned long lastTime = 0;
int lastIR1 = HIGH;
int lastIR2 = HIGH;
void setup() {
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  lcd.setCursor(0, 0);
  lcd.print("Smart Monitor");
  delay(1500);
  lcd.clear();
}
long getDistance() {
  long total = 0;
  int valid = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    long duration = pulseIn(ECHO, HIGH, 30000);
    long d = duration * 0.034 / 2;
    if (d > 2 && d < 400) {
      total += d;
      valid++;
    }
    delay(5);
  }
  if (valid == 0) return 999;
  return total / valid;
}
int getGasValue() {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(MQ);
    delay(2);
  }
  return total / 10;
}
void updateCount() {
  int ir1 = digitalRead(IR1);
  int ir2 = digitalRead(IR2);
  if (lastIR1 == HIGH && ir1 == LOW) {
    state = 1;
    lastTime = millis();
  }
  else if (lastIR2 == HIGH && ir2 == LOW) {
    state = 2;
    lastTime = millis();
  }
  if (state == 1 && ir2 == LOW) {
    count++;
    state = 0;
    delay(200);
  }
  if (state == 2 && ir1 == LOW) {
    count--;
    state = 0;
    delay(200);
  }
  if (millis() - lastTime > 3000) {
    state = 0;
  }
  if (count < 0) count = 0;
  if (count > 99) count = 99;
  lastIR1 = ir1;
  lastIR2 = ir2;
}
void updateBuzzer(bool overcrowd, bool gasAlert, bool densityAlert) {
  digitalWrite(BUZZER, LOW);
  if (overcrowd) {
    digitalWrite(BUZZER, HIGH);
  }
  else if (gasAlert || densityAlert) {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
  }
}
void loop() {
  updateCount();
  float temp = dht.readTemperature();
  if (isnan(temp)) temp = 0;
  int gasValue = getGasValue();
  long distance = getDistance();
  bool overcrowd = count > maxLimit;
  bool gasAlert = gasValue > 470;
  bool densityAlert = (distance < 5);
  updateBuzzer(overcrowd, gasAlert, densityAlert);
  lcd.setCursor(0, 0);
  lcd.print("C:");
  lcd.print(count);
  lcd.print("/");
  lcd.print(maxLimit);
  lcd.print(" T:");
  lcd.print((int)temp);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  if (overcrowd) {
    lcd.print("OVER CROWD!    ");
  }
  else if (gasAlert) {
    lcd.print("AIR BAD!       ");
  }
  else if (densityAlert) {
    lcd.print("HIGH DENSITY   ");
  }
  else {
    lcd.print("NORMAL         ");
  }
  Serial.print("Count: "); Serial.print(count);
  Serial.print(" Dist: "); Serial.print(distance);
  Serial.print(" Gas: "); Serial.println(gasValue);
  delay(120);
}
