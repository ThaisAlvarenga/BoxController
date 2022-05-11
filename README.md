# BoxController

[Google Slides](https://docs.google.com/presentation/d/1f-3V4PVhcC7Sx5pExYIBedytVGbtZPfy_2iwOybdv8s/edit?usp=sharing)

## Description 
 This is the 4th prototype iteration of a controller I have been working on for the class ITP course Fast Fun: Physical Controllers for UNITY. 
 
 It is made up of a button and a joystick input which are controlled by an Arduino Nano 33 IoT. The joystick controls the direction in which the player is moving while the button is used for the upward jump. All of it is mounted in a custom made cardoboard box, while the handle of the joystick was made using moldable plastic. 
 
 This iteration is based on constant user testing that went on for 3 weeks over 3 iterations of the product and the games used for testing. This final iteration also includes its own game iteration that was designed to fascilitate player exploration of the control and movement system. 

 The UNITY side of the experience uses a library called the [Ultimate 2D Controller by Tarodev](https://github.com/Matthew-J-Spencer/Ultimate-2D-Controller). I decided to use this library because of the smoothness of the player movement and the fact that it basically creates its own physics allowed me to play with the values and cater them to the controller for a better human-computer interaction. 
 
 ## Materials:
 ### Hardware
 - Arduino Nano 33 Iot
 - Joystick
 - Button
 - Jumper wires
 - Cardboard
 - Moldable Plastic
 - Soldering Iron
 - Electrical Tape
 - USB cable
 - USB to USB-C converter

### Software:
- Unity
  - Tarodev Library
- Arduino IDE
  - Keyboard.h
  - Mouse.h (for testing)

## Process

For this iteration, I decided to spend two weeks working on the following:
- Mapping input values and serial communication from the Arduino IDE to UNITY
- Level design for better user testing
- Accessiblity (providing easy use for both experience and inexperience players)
- Fabrication of a more robust device where users could distinguish the key components (inputs) and operate easily

### Managing Input Values
From my third iteration of the controller, many users expressed that they did not feel like the controls of the player movement were accurate 100% of the time, specially the joystick inputs. 

I first tried to use OSC messages, but the wireless nature of the controller was not really adding anything to the game experienced, so I discarded that idea and went back to tailoring my previous code, which used the keyboard library and CC messages. I was able to correctly debounce the button input, however, the joystick kept giving false values every now and then regardless of how I scaled the range of the joystick input.

After some experimentation, I decided to take a shot at using the joytsick to determine keypresses rather than sending CC messages, which involved rewriting most of the code I originally had. I basicallt used the Mouse example from [Professor's Barret GitHub](https://github.com/dombarrett/Fast-Fun-SP2022/blob/main/Accelerometer_Mouse.ino)

What my code does it that it receives the joystick x inputs from the Arduino and maps it out to a range for -6 to 6, with 0 being the value given when the joystick is in the center (not being moved) using the . This allowed for much more accurate reads of the position of the player. I used the Mouse.h library to test the accuracy of the reads, however, this library is not nedded to run game experience with the controller. 


This is the source code for my sketch on the Arduino IDE which can be also accessed through the GitHub repo. 
``` C++

#include "Mouse.h"
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
  Mouse.begin();

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
```

### Level Design

Something I noticed in my first 3 iteration tests was that not all players were experimenting with the range of movement that the controller could give. I figured that part of it was just the way the game level was presented. It was very structured and did not allow for player exploration, which I believe subconciously limited the player to only try to perform the tasks like going up the determined path. 

I thus decided to create another level for my user testing. This level was design with an exploration element in mind through which the user can go through different paths and experiment with the movement controls in order to get into certain areas.

An extra addition I made to this game iteration was to improve the camera of the player. I used the CineMachine 2D package from the Unity asset store in order to give the player an ample view of their possible routes without compromising the exploration element. CineMachine allowed easy manipulation of camera lengths, camera follow of the player and establishing a death time in which the camera does not follow the player so as to not have a constant movement, which was confising and overwhelming for inexperienced ganers. CineMachine was a powerful tool that I could edit during the user tests which allowed the users to concentrate on experimenting with the controller rather than getting distracted by the screen and camera follow. 

### Accessiblity

Originally I wanted to reduce the size of the controller so people could use it with only one hand. But, after some experimentation with sizes, I noticed that regardless of the size, people were still using both hands to grab the controller, so the reduction of the size lost its purpose. 

I proceeded to focus more on making the controller accesible for people not too familiar with video games, since at the begining all of my users were people who had played games their whole lives. 

I started to notice that least experienced users would not bend the joystick as much and thus, would limit the speed that their player character could have, which in turn would make their gameplay longer and more frustrating for when they had to restart. 

So I decided to a a handle to the joystick in the hopes that the size would prompt people to move the joystick more in order for them to get the range of movement that the game allowed, without having to tell them that they could bend the joystick more. This worked and users actually though I had tweaked with the speed and movement when in fact I had only changed the handle.

A few users commented that the size of the handle was too big, so I chopped off part of it and was told that the current height is "optimal for play". 

For the final presentation of my controller, I decided to change from a cardboard handle to one made by moldable plastic in order to make it more ergonomic. 

### Fabrication

Since this was my final iteration for the class, I decided to spend a bit more time on fabrication, which also allowed me to improve certain aspects of the user testing.

One of the main issues with my early prototypes was the fact that they could not handle much abuse from the player. I thus changed the material for my 2nd iteration to cardboard. However, the problem of doin this was that the cables I had were too big and disorganized for the box, so every now and then one cable would go loose and I would have to disassemble the whole thing to repair it. 

For this version, I decided to spend the first week on the wiring. This included using smaller jumper wires for the breadboard and soldering the cables of the input elements (button and joystick). Afterwards, I changed the arrangement of the cables so they would fall in an area similar to where they would be psoitioned inside the box. Since the joystick was at the top, its cables would be placed on the side of the breadboard closest to the top while the button cables would go nearer the front of where the cube's front it. This made the wiring difficulties completely subside and I had no issues of things disconnecting at all during user testing and transportation. 

Then I decided to make the box sturdier by using electrical tape instead of normal tape. Jake lend me some electrical tape during one of our classes and it worked wonders for making the walls of the box stick together. I thus decided to incorporate this in my design. Once the issues with the wiring were fixed, I was able to completely cover the corners of the box with black tape which secured it so players could be more rough with the controller. 

Lastly, I changed the color of the button from blue to red because many users were not noticing that there was a button they could press since it would merge a bit with the black body of the controller. 

## Reflections 

I really enjoyed the whole process of fabricating controllers for UNITY or other software like P5.js. 

I believe that this project has taught me the importance of user testing and how sometimes features you think are essential to your design are not actually that important. Many of the things I changed for this version of the controller were based on user feedback and behaviour rather than on my own personal view of the design. In fact, I ended up discarting many of my ideas for the sake of user experience. 

I am now so much more comfortable AND capable of doing fast prototyping and iterating on ideas rather than spending a bunch of time in a product that has not been tested. I feel like this approach is what I will be aiming for in my artistic and technical practice moving on. 

Overall it was very fun! I made a lot of friends through user testing, met very interesting people and learned a lot about how to user test in different ways and change my designs to fascilitate that part of the process. 

Thank you professor Barret! 

### For the future:

Even though I am very happy with the results of this controller, there are still things I would like to change. The main things to focus on for future iterations would be:
- Expanding game interactions for my current level that would make the player focus on fullfilling in-game objects rather than playtesting movement (so that they can do the second one unconciously)
- Experimenting with the weight of the handle since some people have mentioned that it is a bit too light
- Debugging current issues with the Unity project: there is a slight problem with the colliders of the tiles. Someone suggested I try the [Edge Collider 2D](https://docs.unity3d.com/Manual/class-EdgeCollider2D.html) which looks like a very promising option. Another thing could be to create my own version of the Tarodev library that uses Unity colliders instead of costume made ones. 
- Creating levels with different mechanics that utilize the same controller or making it a multiplayer game with two separate controllers either of the same type or two different iterations.  
