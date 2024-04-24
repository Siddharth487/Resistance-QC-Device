#include <LiquidCrystal.h>

// Define the LCD pins and analog resistance pins and LED pins
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
const int analogPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int analogPinLED[] = {A12, A13, A14, A15, A8, A9, A10, A11}; 

// raw, Vin, Vout, and buffer are used to convert the voltage reading from digital to analog
int raw = 0;
int Vin = 5;
float Vout1 = 0;
float known_R1 = 680;
float buffer = 0;
float unknown_R[] = {0, 0, 0, 0, 0, 0, 0, 0}; // array of the 8 resistances

// resistance value of each LED wire
float green = 0;
float blue = 0;
float yellow = 0;
float purple = 0;

// variables for the button activated state change
int buttonPress = 26;
int buttonState;
int pastState = HIGH;
bool colorToggle = false;
int i; // used in the button state loop

int allGood = 0; // used to ensure only the white light flashes when all LED resistances in spec

// Resistance spec check LED
#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9

// State indicator LED
int RED_PIN2 = 8;
int GREEN_PIN2 = 7;
int BLUE_PIN2 = 6;

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);

  // Print a starting message
  lcd.print("Resistances:");

  // pin initialization
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(RED_PIN2, OUTPUT);
  pinMode(GREEN_PIN2, OUTPUT);
  pinMode(BLUE_PIN2, OUTPUT);

  pinMode(buttonPress, INPUT_PULLUP);

  // Wait for a moment
  delay(500);

  // Clear the LCD
  lcd.clear();
  Serial.begin(9600);
}

void loop() {

  buttonState = digitalRead(buttonPress); // Read the state of the button
  // Check if the button state has changed
  if (buttonState != pastState) {
    // Check if the button is pressed (transition from LOW to HIGH)
    if (buttonState == HIGH) {
      // Toggle the color state
      colorToggle = !colorToggle;
      // Update the RGB LED state based on color state
      if (colorToggle) {
        // Set the RGB LED to orange (mix of red and green)
        i = 0;
        analogWrite(RED_PIN2, 255); //255
        analogWrite(GREEN_PIN2, 20); //20
        analogWrite(BLUE_PIN2, 0); //0
        Serial.println(buttonState);
      } else {
        i = 4; // set the LED to pink
        analogWrite(RED_PIN2, 255);
        analogWrite(GREEN_PIN2, 25);
        analogWrite(BLUE_PIN2, 60);
        Serial.println(buttonState);
      }
    }
    // Update the last button state
    pastState = buttonState;
    delay(500);
  }

//  Serial.println(A5);
//  Serial.println(A6);
//  Serial.println(A7);
  
//Unknown voltage is R2 for the resistance tests displayed on the screen (isolate for R2)
  // digital to analog conversion of voltage, followed by voltage divider (for the paired wire loads)
  for(int i = 0; i < 7; i+=2){
    raw = analogRead(analogPin[i]);
    buffer = raw * Vin;
    Vout1 = (buffer)/1024.0;
    buffer = (Vin/Vout1) - 1;
    unknown_R[i]=  ((known_R1*Vout1)/(Vin-Vout1));

    lcd.setCursor(0, 0);
  }
//Unknown voltage is R2 for the resistance tests displayed on the screen (isolate for R2)
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

  // digital to analog conversion of voltage, followed by voltage divider (for the led wires)
  raw = analogRead(analogPinLED[i]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  if(colorToggle == true){
    green = (((Vin*known_R1)/Vout1)-known_R1);
  }
  else{
    green = ((known_R1*Vout1)/(Vin-Vout1));
  }

  raw = analogRead(analogPinLED[i+1]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  if(colorToggle == true){
    blue = (((Vin*known_R1)/Vout1)-known_R1);
  }
  else{
    blue = ((known_R1*Vout1)/(Vin-Vout1));
  }

  raw = analogRead(analogPinLED[i+2]);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  if(colorToggle){
    yellow = (((Vin*known_R1)/Vout1)-known_R1);
  }
  else{
    yellow = ((known_R1*Vout1)/(Vin-Vout1));
  }
  
  raw = analogRead(analogPinLED[i+3]);
  Serial.println(String("raw: ") + raw);
  buffer = raw * Vin;
  Vout1 = (buffer)/1024.0;
  buffer = (Vin/Vout1) - 1;
  if(colorToggle){
    purple = (((Vin*known_R1)/Vout1)-known_R1);
  }
  else{
    purple = ((known_R1*Vout1)/(Vin-Vout1));
  }

  // print the resistance values of the wires
  Serial.println(String("Green: ") + green);
  delay(100); 
  Serial.println(String("Blue: ") + blue);
  delay(100);
  Serial.println(String("Yellow: ") + yellow);
  delay(100);
  Serial.println(String("Purple: ") + purple);
  delay(100);

  // if all resistances above threshold, set light to red
  if (green >= 10 && blue >= 10 && yellow >= 10 && purple >= 10){
    setColor(255, 0, 0);
    delay(1000); // Wait for 1 second
  }
  
  // if all resistances within spec, set light to white
  else if (green < 10 && blue < 10 && yellow < 10 && purple < 10){
    setColor(255, 255, 255);
    allGood = 1; // this prevents the next nested if statement from running if all within spec
    delay(1000); // Wait for 1 second
  }
  
  // Serial.println("all working");
  // delay(500);
  
  // The statement below is to display the lights of the LED wires that are in spec, while excluding the rest
  if (allGood == 0){

    if (green < 10){
      // Green 
      setColor(0, 255, 0);
      delay(1000); // Wait for 1 second
    }

    if (blue < 10){
    
      setColor(0, 0, 255);
      delay(1000); // Wait for 1 second
    }

    if (yellow < 10){

      setColor(240, 100, 0);
      delay(1000); // Wait for 1 second
    }

    if (purple < 10){

      setColor(255, 0, 255);
      delay(1000); // Wait for 1 second
    }
  }
  allGood = 0;

  delay(500);
  }

  // function to write to the LED
  void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  }

  