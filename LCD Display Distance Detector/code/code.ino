#include <LiquidCrystal.h> 
#include <SmoothProgress.h>
#include <BarStyle1.h>
#include "NewPing.h"
#define MAX_DISTANCE 400  // in CM
#define ITER 5            // Max iterations for improved accuracy
#define TRIG 4
#define ECHO 5
#define BUTTON_METRIC 3
#define BUTTON_ZERO 2
#define CONTRAST 60
double zero_dist = 0;
enum measure_modes {CM, MM, IN, FT};
enum measure_modes mode = CM;
bool zero_mode = false;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); 
LCD dispA(lcd, barStyle1);
SmoothProgressBar spb(dispA, 16, 0, 1); // Progress bar 10 characters wide, at 5-th column, 0-th row 
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);

void setup() {
  pinMode(BUTTON_METRIC, INPUT_PULLUP);
  pinMode(BUTTON_ZERO, INPUT_PULLUP);   
  analogWrite(6, CONTRAST);             // Adjust LCD contrast
  dispA.begin();
	lcd.begin(16, 2);
}

void loop() {
  double distance = (sonar.ping_median(ITER) / 2) * 0.0343;

  if (digitalRead(BUTTON_METRIC) == LOW) { // Change measurement display mode  
    switch (mode) {
      case MM: mode = IN; break;
      case IN: mode = FT; break;
      case FT: mode = CM; break;
      default: mode = MM;
    }
    delay(200);
  }

  if (digitalRead(BUTTON_ZERO) == LOW) {   // Zero mode on/off
    if (zero_mode) {zero_mode = false; zero_dist = 0;}
    else {zero_mode = true; zero_dist = distance;}
    delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  switch (mode) {                           // Update display according to measurement mode  
    case MM: display((distance - zero_dist) * 10, " mm"); break;
    case IN: display((distance - zero_dist) / 2.54, " in"); break;
    case FT: displayFT(distance - zero_dist); break;
    default: display(distance - zero_dist, " cm");
  }

	// Draw progress bar on the second line of the LCD
	spb.showProgressPct(distance/MAX_DISTANCE*100);
	delay(200);
}

void displayFT(double distance) {
  lcd.print("Dis = ");
  lcd.print((int) distance / 30);
  lcd.print(" ft ");
  lcd.print((int) (distance / 2.54) % 12);
  lcd.print(" in");
}

void display(double distance, const char *str) {
  lcd.print("Dis = ");
	lcd.print(distance);
	lcd.print(str);
}