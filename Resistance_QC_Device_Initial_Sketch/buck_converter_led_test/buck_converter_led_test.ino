const int analogPinLED[] = {A8, A9, A10, A11, A0, A1, A2, A3};

int raw = 0;
int Vin = 5;
float Vout1 = 0;
float known_R1 = 680; //100;
float buffer = 0;
float green = 0;
float blue = 0;
float yellow = 0;
float purple = 0;

int buttonPress = 13;
int buttonState;
int pastState = HIGH;
bool colorToggle = false;
int i;
int allGood = 0;

#define RED_PIN 7
#define GREEN_PIN 6
#define BLUE_PIN 5

int RED_PIN2 = 4;
int GREEN_PIN2 = 3;
int BLUE_PIN2 = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(RED_PIN2, OUTPUT);
  pinMode(GREEN_PIN2, OUTPUT);
  pinMode(BLUE_PIN2, OUTPUT);

  pinMode(buttonPress, INPUT_PULLUP);
  //digitalWrite(buttonPress, HIGH);
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
        i = 4;
        analogWrite(RED_PIN2, 255);
        analogWrite(GREEN_PIN2, 25);
        analogWrite(BLUE_PIN2, 60);
        Serial.println(buttonState);
      }
    }
    // Update the last button state
    pastState = buttonState;
  }

  // Serial.println("made it here");
  // delay(500);

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
// the error occuring here is because of the if/else statement above.maybe out of scope?
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

  // Serial.println("set led values");
  // delay(500);
  
  Serial.println(String("Green: ") + green);
  delay(100); // Wait for 1 second
  Serial.println(String("Blue: ") + blue);
  delay(100);
  Serial.println(String("Yellow: ") + yellow);
  delay(100);
  Serial.println(String("Purple: ") + purple);
  delay(100);

  if (green >= 10 && blue >= 10 && yellow >= 10 && purple >= 10){
    setColor(255, 0, 0);
    delay(1000); // Wait for 1 second
  }
  
  else if (green < 10 && blue < 10 && yellow < 10 && purple < 10){
    setColor(255, 255, 255);
    allGood = 1;
    delay(1000); // Wait for 1 second
  }
  
  // Serial.println("all working");
  // delay(500);
  
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
  // put your main code here, to run repeatedly:
  allGood = 0;

}

 void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  }
