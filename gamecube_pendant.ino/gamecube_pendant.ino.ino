/*
Code bases of  GamecubeController example by
  Copyright (c) 2014-2016 NicoHood
  See the readme for credit to other people.

  Additional code by Mik Wind
*/

/*
  Map gamecube controller to a CNCUSB MK1 jog input.
  Use D pad for X and Y axes
  Use D Stick for Z and A axes.
  R trigger to set the speed
  See Defines for Pins

*/

#include "Nintendo.h"
// Pin definitions
#define pinAminus 2
#define pinAplus 3
#define pinZminus 4
#define pinZplus 5
#define pinYminus 6
#define pinYplus 7
#define pinXminus 8
#define pinXplus 9
#define pinAnalogSpeedOut 10

#define pinController 13

//MEGA:
//#define pinController 7
//#define pinAnalogSpeedOut 2
//#define pinAminus 22
//#define pinAplus 23
//#define pinZminus 24
//#define pinZplus 25
//#define pinYminus 26
//#define pinYplus 27
//#define pinXminus 28
//#define pinXplus 29



// Define a Gamecube Controller
CGamecubeController GamecubeController(pinController);

struct JogOutput
{
  uint8_t speed = 0;
  bool x_plus = 0;
  bool x_minus = 0;
  bool y_plus = 0;
  bool y_minus = 0;
  bool z_plus = 0;
  bool z_minus = 0;
  bool a_plus = 0;
  bool a_minus = 0;
};
JogOutput NoMotion;

void writeToJogOutput(const JogOutput& out)
{
  //Analog speed is inverted!:
  analogWrite(pinAnalogSpeedOut, 255-out.speed);
  digitalWrite(pinAminus, out.a_minus);
  digitalWrite(pinAplus, out.a_plus);
  digitalWrite(pinZminus, out.z_minus);
  digitalWrite(pinZplus, out.z_plus);
  digitalWrite(pinYminus, out.y_minus);
  digitalWrite(pinYplus, out.y_plus);
  digitalWrite(pinXminus, out.x_minus);
  digitalWrite(pinXplus, out.x_plus);

}


void setup()
{
  // Set up PINS
  pinMode(pinAnalogSpeedOut, OUTPUT);
  pinMode(pinAminus,OUTPUT);
  pinMode(pinAplus,OUTPUT);
  pinMode(pinZminus,OUTPUT);
  pinMode(pinZplus,OUTPUT);
  pinMode(pinYminus,OUTPUT);
  pinMode(pinYplus,OUTPUT);
  pinMode(pinXminus,OUTPUT);
  pinMode(pinXplus,OUTPUT);


  // Start debug serial
  while (!Serial);
  Serial.begin(115200);

  // Wait for some Serial input
  // Serial.println(F("Enter any key to start reading the controller."));
  // Serial.println(F("Press A on the controller to enable rumble."));
  // while (Serial.read() == -1);

  Serial.println(F("Starting Pendant joystick controller emulation"));
  Serial.println();
}

void loop()
{
  // Try to read the controller data
  if (GamecubeController.read())
  {
    // Print Controller information
    auto status = GamecubeController.getStatus();
    auto report = GamecubeController.getReport();
    convertGCtoJOgOut(report, status);
    print_gc_report(report, status);

    // Rumble if button "A" was pressed
    // if (report.a) {
    //   GamecubeController.setRumble(true);
    // }
    // else {
    //   GamecubeController.setRumble(false);
    // }
  }
  else
  {
    // Add debounce if reading failed
    Serial.println(F("Error reading Gamecube controller."));
    writeToJogOutput(NoMotion);
    delay(1000);
  }
}

void convertGCtoJOgOut(Gamecube_Report_t &gc_report, Gamecube_Status_t &gc_status)
{
  static JogOutput theJogOutput;

  //Trigger that is more pressed gets picked
  if(gc_report.right > gc_report.left)
  {
  // Set max speed if R pressed
  if(gc_report.r)
    theJogOutput.speed = 255;
    else
     theJogOutput.speed = gc_report.right;
  }
  else
  {//The more you press L, the slower!

  if(gc_report.l)
    theJogOutput.speed = 1;
    else
    {
     uint8_t left = gc_report.left;
     if(left < 30)
      left = 30;
      else if (left > 200)
        left = 200;
     left -=30; // now left is between 0 and 170;
     left /=5; // now left is between 0 and 34
     theJogOutput.speed = 35-left;
    }
    
  }


  //One needs to press a button to move an axis.  x,y,z -> x,y,zAxis   b -> aAxis, Pressing A enables all axes
  //note that we don't use A for axis A because it is a big confortable button, and has a better layout.
  
  theJogOutput.x_minus = (gc_report.a || gc_report.x)  && (gc_report.dleft  || (gc_report.xAxis < 100));
  theJogOutput.x_plus = (gc_report.a || gc_report.x) && (gc_report.dright || (gc_report.xAxis > 200));
  
  theJogOutput.y_minus = (gc_report.a || gc_report.y)&& (gc_report.ddown || (gc_report.yAxis < 100));
  theJogOutput.y_plus = (gc_report.a || gc_report.y)&& (gc_report.dup || (gc_report.yAxis > 200));
  
  theJogOutput.z_plus = (gc_report.a || gc_report.z)&& (gc_report.cyAxis > 200);
  theJogOutput.z_minus = (gc_report.a || gc_report.z)&& (gc_report.cyAxis < 100);
  
  theJogOutput.a_plus = (gc_report.a || gc_report.b)&& (gc_report.cxAxis > 200);
  theJogOutput.a_minus = (gc_report.a || gc_report.b)&& (gc_report.cxAxis < 100);
  
  writeToJogOutput(theJogOutput);

}

void print_gc_report(Gamecube_Report_t &gc_report, Gamecube_Status_t &gc_status)
{
  // Print device information
  // Serial.print(F("Device: "));
  // switch (gc_status.device) {
  //   case NINTENDO_DEVICE_GC_NONE:
  //     Serial.println(F("No Gamecube Controller found!"));
  //     break;
  //   case NINTENDO_DEVICE_GC_WIRED:
  //     Serial.println(F("Original Nintendo Gamecube Controller"));
  //     break;

  //   default:
  //     Serial.print(F("Unknown "));
  //     Serial.println(gc_status.device, HEX);
  //     break;
  // }

  // Print rumble state
  // Serial.print(F("Rumble "));
  // if (gc_status.rumble)
  //   Serial.println(F("on"));
  // else
  //   Serial.println(F("off"));

  // Prints the raw data from the controller
  Serial.println();
  // Serial.println(F("Printing Gamecube controller report:"));


 Serial.print(F("Start:"));
 Serial.print(gc_report.start);
 Serial.println(",");

 Serial.print(F("Y:"));
 Serial.print(gc_report.y);
 Serial.println(",");

 Serial.print(F("X:"));
 Serial.print(gc_report.x);
 Serial.println(",");

 Serial.print(F("B:"));
 Serial.print(gc_report.b);
 Serial.println(",");

 Serial.print(F("A:"));
 Serial.print(gc_report.a);
 Serial.println(",");

 Serial.print(F("LL:"));
 Serial.print(gc_report.l);
 Serial.println(",");

 Serial.print(F("RR:"));
 Serial.print(gc_report.r);
 Serial.println(",");
  
 Serial.print(F("Z:"));
 Serial.print(gc_report.z);
 Serial.println(",");

  Serial.print(F("Dup:"));
  Serial.print(gc_report.dup);
  Serial.println(",");
  Serial.print(F("Ddown:"));
  Serial.print(gc_report.ddown);
  Serial.println(",");
  Serial.print(F("Dright:"));
  Serial.print(gc_report.dright);
  Serial.println(",");
  Serial.print(F("Dleft:"));
  Serial.print(gc_report.dleft);
  Serial.println(",");

  Serial.print(F("xAxis:"));
  Serial.print(gc_report.xAxis, DEC);
  Serial.println(",");
  Serial.print(F("yAxis:"));
  Serial.print(gc_report.yAxis, DEC);
  Serial.println(",");

  Serial.print(F("cxAxis:"));
  Serial.print(gc_report.cxAxis, DEC);
  Serial.println(",");
  Serial.print(F("cyAxis:"));
  Serial.print(gc_report.cyAxis, DEC);
  Serial.println(",");

  Serial.print(F("L:"));
  Serial.print(gc_report.left, DEC);
  Serial.println(",");
  Serial.print(F("R:"));
  Serial.print(gc_report.right, DEC);
  Serial.println();
}
