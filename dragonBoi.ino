//#define DEBUG
int red = 100;
int redPin = 9;
int blue = 100;
int bluePin = 10;
int green = 255;
int greenPin = 11;
int mode = 0;
int modePin = 8;
int setModes[7][3] = { { 255, 0, 0 }/*Red*/, { 0, 255, 0 }/*Green*/, { 0, 0, 255 }/*Blue*/, { 255, 70, 0 }/*Purple*/, { 150, 10, 200 }/*Gold*/, { 255, 0, 80 }/*Copper*/, { 100, 100, 255 }/*White*/ };
int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setRGB(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(bluePin, blue);
  analogWrite(greenPin, green);
}

void cycleMode() {
  if (mode == sizeof(setModes) / sizeof(setModes[0])) {
    mode = 0;
  }
  red = setModes[mode][0];
  blue = setModes[mode][1];
  green = setModes[mode][2];
  mode++;
}

void setup() {  // this sets the output pins
  Serial.begin(9600);

  pinMode(modePin, INPUT_PULLUP);  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(redPin, OUTPUT);         // Pin 13 has an LED connected on most Arduino boards:
  pinMode(greenPin, OUTPUT);       // Pin 13 has an LED connected on most Arduino boards:
  pinMode(bluePin, OUTPUT);        // Pin 13 has an LED connected on most Arduino boards:
}


void loop() {
  setRGB(red, green, blue);


  int sensorVal = digitalRead(modePin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (sensorVal != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (sensorVal != buttonState) {
      buttonState = sensorVal;

      // only toggle the LED if the new button state is HIGH
      if (sensorVal == LOW) {

        cycleMode();
      }
    }
  }


  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = sensorVal;

#ifdef DEBUG
  //Manually enter RGB values into serial monitor separated by commas to test new colors (Ex for white: 255,255,255)
   String colorArray = Serial.readStringUntil('\n');
   if (colorArray.length()<1)
   {                // bail if we got an empty line
     return;
   }

   red = getValue(colorArray, ',', 0).toInt();
   green = getValue(colorArray, ',', 1).toInt();
   blue = getValue(colorArray, ',', 2).toInt();
  #endif
}
