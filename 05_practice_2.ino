#define PIN_LED 7 

int count; 
bool light;
bool initiated;

void setup() { 
  pinMode(PIN_LED,OUTPUT);
  // put your setup code here, to run once:
  light = HIGH;
  initiated = false;
}

void loop() {
  

  if (!initiated) {
    digitalWrite(PIN_LED, LOW);
    delay(1000);
    
    int n = 10;
    while (n--) {
      light = !light;
      digitalWrite(PIN_LED, light);
      delay(100); 
    }
    initiated = true;
  
  }
  
}