#define LED 2
#define BUTTON 0          // boot button
int count = 0;

void setup() {
  pinMode (LED, OUTPUT);
  pinMode (BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(BUTTON) == LOW){

    if (count == 0){
      Serial.print('o');
      count++;
      delay(500);
    } else{
      Serial.print('f');
      count = 0;
      delay(500);
    }
  }
  
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'o') {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  }

}
