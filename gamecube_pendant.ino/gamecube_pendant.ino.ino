/*
  Copyright (c) 2014-2016 NicoHood
  See the readme for credit to other people.

  GamecubeController example
  Enter any key into the serial monitor to start reading.
  Press A on the controller to enable rumble.
*/

#include "Nintendo.h"

// Define a Gamecube Controller
CGamecubeController GamecubeController(7);

// Pin definitions
#define pinLed LED_BUILTIN

void setup()
{
  // Set up debug led
  pinMode(pinLed, OUTPUT);

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
    print_gc_report(report, status);
    delay(100);

    // Rumble if button "A" was pressed
    if (report.a) {
      GamecubeController.setRumble(true);
    }
    else {
      GamecubeController.setRumble(false);
    }
  }
  else
  {
    // Add debounce if reading failed
    Serial.println(F("Error reading Gamecube controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);
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


  // Serial.print(F("Start:"));
  // Serial.print(gc_report.start);
  // Serial.println(",");

  // Serial.print(F("Y:"));
  // Serial.print(gc_report.y);
  // Serial.println(",");

  // Serial.print(F("X:"));
  // Serial.print(gc_report.x);
  // Serial.println(",");

  // Serial.print(F("B:"));
  // Serial.print(gc_report.b);
  // Serial.println(",");

  // Serial.print(F("A:"));
  // Serial.print(gc_report.a);
  // Serial.println(",");

  // Serial.print(F("LL:"));
  // Serial.print(gc_report.l);
  // Serial.println(",");

  // Serial.print(F("RR:"));
  // Serial.print(gc_report.r);
  // Serial.println(",");
    
  // Serial.print(F("Z:"));
  // Serial.print(gc_report.z);
  // Serial.println(",");

  // Serial.print(F("Dup:"));
  // Serial.print(gc_report.dup);
  // Serial.println(",");
  // Serial.print(F("Ddown:"));
  // Serial.print(gc_report.ddown);
  // Serial.println(",");
  // Serial.print(F("Dright:"));
  // Serial.print(gc_report.dright);
  // Serial.println(",");
  // Serial.print(F("Dleft:"));
  // Serial.print(gc_report.dleft);
  // Serial.println(",");

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
