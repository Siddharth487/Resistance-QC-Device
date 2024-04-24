#include <LiquidCrystal.h>

// Define the LCD pins and analog resistance pins
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
const int analogPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int analogPinLED[] = {A12, A13, A14, A15};

int raw = 0;
int Vin = 5;
float Vout1 = 0;
float known_R1 = 680;
float buffer = 0;
float unknown_R[] = {0, 0, 0, 0, 0, 0, 0, 0};
float green = 0;
float blue = 0;
float yellow = 0;
float purple = 0;

#define RED_PIN 8
#define GREEN_PIN 9
#define BLUE_PIN 10

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);

  // Print a welcome message
  lcd.print("Resistances:");

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Wait for a moment
  delay(1000);

  // Clear the LCD
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  Serial.println(A5);
  Serial.println(A6);
  Serial.println(A7);
  
  // Unknown voltage is R2 for the resistance tests displayed on the screen (isolate for R2)
  for(int i = 0; i < 7; i+=2){
    raw = analogRead(analogPin[i]);
    buffer = raw * Vin;
    Vout1 = (buffer)/1024.0;
    buffer = (Vin/Vout1) - 1;
    unknown_R[i]=  ((known_R1*Vout1)/(Vin-Vout1));

    lcd.setCursor(0, 0);
    lcd.print("                      ");
    //delay(50);
    lcd.setCursor(0, 0);
    lcd.print(String("R")+String(i+1)+String(": ") + String(unknown_R[i]) + String(" ohms"));
    delay(500);

    raw = analogRead(analogPin[i+1]);
    buffer = raw * Vin;
    Vout1 = (buffer)/1024.0;
    buffer = (Vin/Vout1) - 1;
    unknown_R[i+1]=  ((known_R1*Vout1)/(Vin-Vout1));

    lcd.setCursor(0, 1);
    lcd.print("                      ");
    //delay(50);
    lcd.setCursor(0, 1);
    lcd.print(String("R")+String(i+2)+String(": ") + String(unknown_R[i+1]) + String(" ohms"));
    delay(500);
    }


  raw = analogRead(analogPinLED[0]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  green = (((Vin*known_R1)/Vout1)-known_R1);

  raw = analogRead(analogPinLED[1]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  blue = (((Vin*known_R1)/Vout1)-known_R1);

  raw = analogRead(analogPinLED[2]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  yellow = (((Vin*known_R1)/Vout1)-known_R1);
  
  raw = analogRead(analogPinLED[3]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  purple = (((Vin*known_R1)/Vout1)-known_R1);

  Serial.println(String("Green: ") + green);
  delay(20); // Wait for 1 second
  Serial.println(String("Blue: ") + blue);
  delay(20);
  Serial.println(String("Yellow: ") + yellow);
  delay(20);
  Serial.println(String("Purple: ") + purple);
  delay(20);

  // This now checks for resistances instead of voltage. If R is greater than a certain threshold, LED will not turn on
  // Unknown resistance is R1 for the led test (isolate for R1)
  if (green >= 10 && blue >= 10 && yellow >= 10 && purple >= 10){
    setColor(255, 0, 0);
    delay(2000); // Wait for 1 second
  }

  else if (green < 1 && blue < 10 && yellow < 10 && purple < 10){
  setColor(255, 255, 255);
  delay(2000); // Wait for 1 second
  }
  
  // all LED resistances seem to be hovering around 3 to 9 ohms 
  // all other harness resistances are hovering between 1.3 and 4.1 ohms
  //.all measurements have a margin of error of 1-2 ohms (this margin of error still falls within the max and min resistances stated above)

  else if (true){

    if (green < 10){
      // Green 
      setColor(0, 255, 0);
      delay(2000); // Wait for 1 second
    }

    if (blue < 10){
    
      setColor(0, 0, 255);
      delay(2000); // Wait for 1 second
    }

    if (yellow < 10){

      setColor(240, 222, 67);
      delay(2000); // Wait for 1 second
    }

    if (purple < 10){

      setColor(148, 48, 165);
      delay(2000); // Wait for 1 second
    }
  }
  }

  void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  }

  