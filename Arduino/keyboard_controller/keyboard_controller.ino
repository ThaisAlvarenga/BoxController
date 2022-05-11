
//#include "Mouse.h"
#include "Keyboard.h"

//set pin # 
const int xAxis = A0;         // joystick X axis
const int jumpButton = 2;

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value

bool mouseIsActive = true;    // whether or not to control the mouse


void setup() {
  // put your setup code here, to run once

  // initialize the buttons' inputs
  // instead of "INPUT", we're using "INPUT_PULLUP"
  // this uses internal pull up resistors for the inputs
   pinMode(jumpButton, INPUT_PULLUP);

   //use inputs as keyboard
   Keyboard.begin();

  // take control of the mouse:
//  Mouse.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  // use the pushbutton to control the jump using keyboard space key
  // note how we are checking for LOW instead of the usual HIGH
  // this is how we check to see if a button is pressed when using INPUT_PULLUP mode


  //this version of the pullup is glitchy because the button is constantly being press if you hold it
  //  if (digitalRead(jumpButton) == LOW) {
  //    Keyboard.write(' ');
  //  }

  //for better player control, we want a "press and hold" type functionality
  // in that case, we would use Keyboard.press() and Keyboard.release()
  
  if (digitalRead(jumpButton) == LOW) {
    Keyboard.press(' ');
  } else {
    Keyboard.release(' ');   
  }

  // read and scale the two axes:
  int xReading = readAxis(A0);
  int yReading = readAxis(A1);

  Serial.print("X-Reading:");
  Serial.print(xReading);
  Serial.print("Y-Reading");
  Serial.print(yReading);

  if(xReading<0){
    Keyboard.write('a');
  }
  else if(xReading > 0)
  {
    Keyboard.write('d');
  }

   // if the mouse control state is active, move the mouse:
   //  if (mouseIsActive) {
   //    Mouse.move(xReading, yReading, 0);
   //  }

}

/*
  reads an axis (0 or 1 for x or y) and scales the analog input range to a range
  from 0 to <range>
*/
int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

/*Test Key press
 *
 *                                                                                                                                                                                                            

*/
