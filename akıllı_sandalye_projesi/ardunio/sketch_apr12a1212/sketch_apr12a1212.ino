// Motor Pin Tanımlamaları
int motorPin1 = 2;  // Motor 1, IN1 pin
int motorPin2 = 3;  // Motor 1, IN2 pin
int motorPin3 = 4;  // Motor 2, IN3 pin
int motorPin4 = 5;  // Motor 2, IN4 pin
int motorPin5 = 6;  // Motor 3, IN5 pin
int motorPin6 = 7;  // Motor 3, IN6 pin
int motorPin7 = 8;  // Motor 4, IN7 pin
int motorPin8 = 9;  // Motor 4, IN8 pin

void setup() {
  Serial.begin(9600);  // Seri haberleşme başlat

  // Motor Pinlerini Çıkış Olarak Ayarlıyoruz
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int pin = Serial.parseInt();  // Python'dan gelen pin numarasını al
    Serial.print("Gelen pin: ");
    Serial.println(pin);  // Seri monitöre gelen pin numarasını yazdır

    if (pin == 4) {
      // Geri git (tüm motorlar aynı anda hareket edecek)
      moveBackward();
    }
    else if (pin == 5) {
      // Sağ dön (tüm motorlar aynı anda hareket edecek)
      turnRight();
    }
    else if (pin == 6) {
      // Duruş (tüm motorlar aynı anda duracak)
      stopMotors();
    }
    else if (pin == 7) {
      // Sol dön (tüm motorlar aynı anda hareket edecek)
      turnLeft();
    }
    else if (pin == 8) {
      // İleri git (tüm motorlar aynı anda hareket edecek)
      moveForward();
    }
  }
}

// Motorları sağa hareket ettirme
void turnRight() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, HIGH);
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, HIGH);
  delay(1200);  // 2 saniye boyunca hareket et
  stopMotors();  // Dur
}

// Motorları sola hareket ettirme
void turnLeft() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);
  delay(1200);  // 2 saniye boyunca hareket et
  stopMotors();  // Dur
}

// Motorları ileri hareket ettirme
void moveForward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, HIGH);
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, HIGH);
  delay(1200);  // 2 saniye boyunca hareket et
  stopMotors();  // Dur
}

// Motorları geri hareket ettirme 
void moveBackward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);
  delay(1200);  // 2 saniye boyunca hareket et
  stopMotors();  // Dur
}

// Motorları durdurma
void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, LOW);
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, LOW);
}


