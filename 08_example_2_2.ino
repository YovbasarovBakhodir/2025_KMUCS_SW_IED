// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance (mm)
#define _DIST_MID 200.0   // middle distance (mm)
#define _DIST_MAX 300.0   // maximum distance (mm)

// Slayddagi talablar:
// 100 mm va 300 mm da minimal (o'chiq)
// 200 mm da maksimal (eng yorug')

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficient to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  
  // Serial Plotter ham, Serial Monitor ham bir xil tezlikda bo'lishi kerak.
  Serial.begin(115200);
}

void loop() { 
  float distance;

  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance
  int brightness = 0; // Bu LED Duty Cycle qiymati, 0 = eng yorug', 255 = o'chiq

  // Slayddagi talab bo'yicha yorug'lik teskari (Active Low) ekanligini hisobga olamiz:
  // Yorug'lik kuchayishi uchun duty cycle (brightness) 255 dan 0 ga kamayishi kerak.

  if (distance <= _DIST_MIN) {
    // 0 ~ 100 mm: LED o'chiq.
    brightness = 255; 
  } 
  else if (distance > _DIST_MIN && distance <= _DIST_MID) {
    // 100 ~ 200 mm: Yorug'lik oshadi (255 → 0)
    // 100 mm (o'chiq) -> 200 mm (eng yorug')
    brightness = map(distance, _DIST_MIN, _DIST_MID, 255, 0);
  } 
  else if (distance > _DIST_MID && distance <= _DIST_MAX) {
    // 200 ~ 300 mm: Yorug'lik kamayadi (0 → 255)
    // 200 mm (eng yorug') -> 300 mm (o'chiq)
    brightness = map(distance, _DIST_MID, _DIST_MAX, 0, 255);
  } 
  else {
    // 300 mm dan katta: LED o'chiq.
    brightness = 255;
  }

  // Slaydga ko'ra:
  // analogWrite(pin, 0) : eng yorug' (active low)
  // analogWrite(pin, 255): o'chiq
  analogWrite(PIN_LED, brightness);

  // Serial Plotter uchun yangi chiqish formati
  // Serial Plotter faqat raqamlarni qabul qiladi. Ular vergul (,) bilan ajratilgan bo'lishi kerak.
  
  // 1. Distance (Masofa) qiymatini uzatish
  Serial.print(distance); 
  // 2. Ajratuvchi belgi
  Serial.print(","); 
  // 3. Brightness (Yorug'lik) qiymatini uzatish va yangi qatorga o'tish
  Serial.println(brightness); 

  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // mm
}