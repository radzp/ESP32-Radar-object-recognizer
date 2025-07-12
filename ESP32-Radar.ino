#include <ESP32Servo.h>
#include <TFT_eSPI.h>

#define trigPin 26
#define echoPin 25
#define PIN_SG90 22

#define SOUND_SPEED 0.034
#define ANGLE_STEP 2
#define RADAR_CENTER_X 160
#define RADAR_CENTER_Y 225
#define RADAR_RADIUS 150
#define MAX_DISTANCE_CM 150

#define COLOR_BG TFT_BLACK
#define COLOR_TEXT TFT_GREEN
#define COLOR_ANGLE TFT_CYAN
#define COLOR_DISTANCE TFT_ORANGE
#define COLOR_POINT TFT_GREEN
#define COLOR_CIRCLE TFT_DARKGREEN

Servo sg90;
TFT_eSPI tft = TFT_eSPI();

long duration;
float distanceCm;
float scale;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  sg90.setPeriodHertz(50);
  sg90.attach(PIN_SG90, 500, 2400);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(COLOR_BG);

  // Wynikiem skalowania jest ile px na ekranie odpowiada 1 cm z czujnika
  scale = RADAR_RADIUS / MAX_DISTANCE_CM;

  drawRadarBackground();
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 10000); //10 ms * SOUND_SPEED = ~ 340/2 = 170 cm
  if (duration == 0) return -1;

  return duration * SOUND_SPEED / 2;
}

void drawRadarBackground() {

  // rysowanie polokregow 
  for (int r = 30; r <= RADAR_RADIUS; r += 30) {
    for (int angle = 0; angle <= 180; angle++) {
      // zamiana ukladu biegunowego na kartezjanski
      float rad = radians(angle);
      int x = RADAR_CENTER_X + cos(rad) * r;
      int y = RADAR_CENTER_Y - sin(rad) * r;
      tft.drawPixel(x, y, COLOR_CIRCLE);
    }
  }

  // rysowanie linii poziomych dla kolejno 0,45,90,135,180 stopni
  for (int a : { 0, 45, 90, 135, 180 }) {
    float rad = radians(a);
    int x = RADAR_CENTER_X + cos(rad) * RADAR_RADIUS;
    int y = RADAR_CENTER_Y - sin(rad) * RADAR_RADIUS;
    tft.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, x, y, COLOR_CIRCLE);
  }
}

void drawRadarPoint(int angle, float distanceCm) {
  if (distanceCm < 0 || distanceCm > MAX_DISTANCE_CM) return;

  float rad = radians(angle);
  int x = RADAR_CENTER_X + cos(rad) * distanceCm * scale;
  int y = RADAR_CENTER_Y - sin(rad) * distanceCm * scale;

  float dx = x - RADAR_CENTER_X;
  float dy = RADAR_CENTER_Y - y;
  float r = sqrt(dx * dx + dy * dy); // Wzór na długość wektora -> nie rysujemy punktu poza radarem
  if (r > RADAR_RADIUS) return;

  tft.fillCircle(x, y, 2, COLOR_POINT);
}


void displayAngle(int angle) {
  tft.setTextColor(COLOR_ANGLE, COLOR_BG);
  tft.setTextSize(1);
  tft.setCursor(200, 10);
  tft.printf("Kat: %3d", angle);
}

void displayDistance(float distance) {
  tft.fillRect(90, 10, 45, 10, COLOR_BG);
  tft.setTextColor(COLOR_DISTANCE);
  tft.setTextSize(1);
  tft.setCursor(0, 10);
  tft.printf("Dystans (w cm): %.2f", distance);
}

void scanRadar(bool forward) {
  if (forward) {
    for (int pos = 0; pos <= 180; pos += ANGLE_STEP) {
      sg90.write(pos);
      delay(20);

      displayAngle(pos);
      distanceCm = measureDistance();
      displayDistance(distanceCm);

      if (distanceCm >= 0) drawRadarPoint(pos, distanceCm);
    }
  } else {
    for (int pos = 180; pos >= 0; pos -= ANGLE_STEP) {
      sg90.write(pos);
      delay(20);

      displayAngle(pos);
      distanceCm = measureDistance();
      displayDistance(distanceCm);
      if (distanceCm >= 0) drawRadarPoint(pos, distanceCm);
    }
  }
}

void loop() {
  delay(50);
  tft.fillRect(10, 60, 310, 180, COLOR_BG);
  drawRadarBackground();

  scanRadar(true);

  delay(100);

  tft.fillRect(10, 60, 310, 180, COLOR_BG);
  drawRadarBackground();

  scanRadar(false);

  delay(300);
}
