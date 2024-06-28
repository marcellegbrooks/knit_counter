#include <RTTStream.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_LSM6DS3.h>

RTTStream rtt;

int btnback = 12;
int btnrecord = 6;
int btnerase = 7;

int prsn = 2; // pressure sensor

int ledone = 10; // blue in schematic, yellow on board
int ledtwo = 13; // yellow in schematic, blue on board

int buzzer = 11; 

int scl = 21;
int sda = 20;

int recording = 0;

int record_exist = 0;

int stichcount = 0;

#define recomended 100 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # 
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x32 (depends on screen size)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  SerialUSB.begin(9600);

  pinMode(ledone, OUTPUT);
  pinMode(ledtwo, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(prsn, INPUT);

  pinMode(btnback, INPUT_PULLUP);
  pinMode(btnrecord, INPUT_PULLUP);
  pinMode(btnerase, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  }

  // display current stich count
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(stichcount);
  display.display();
}

void loop() { 
  
  int back = digitalRead(btnback);
  SerialUSB.println(back);
  if (back == HIGH){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println("Back");
    display.display(); 

    stichcount = stichcount - 1;
    delay(2000);
    display.clearDisplay();
    
    // display current stich count
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println(stichcount);
    display.display();
  }
  // if button is pushed, remove last stitch
  // if following pattern, go back on step in pattern as well
  // print on screen back

  int record = digitalRead(btnrecord);
  SerialUSB.println(record);

  // Recording 
  if (record == HIGH){
    if (recording == 0){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text
      display.println("Recording"); 
      display.display(); 
      // record stiches 
    } else{
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text 
      display.println("Recording stoped"); 
      display.display(); 
      record_exist = 1;

      delay(2000);
      display.clearDisplay();
      // display current stich count
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text
      display.println(stichcount);
      display.display();
    }
  }
  // if button is pushed, start recording gyroscope motions aka count stitches
  // if button is pressed again stop recording
  // print on screen recording while recording 

  // erase recording
  int erase = digitalRead(btnerase);
  SerialUSB.println(erase);
  if (erase == HIGH){
    display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text
      display.println("Recording erased"); 
      display.display(); 
      record_exist = 0; 

      delay(2000);
      display.clearDisplay();
      // display current stich count
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      // Display static text
      display.println(stichcount);
      display.display();
  }
  // if button is pressed, erase recorded pattern
  // print on screen recording erased

  int pressure = analogRead(prsn);
  SerialUSB.println(pressure);
  // if pressure is higher than recomended threshold, buzz buzzer 
  if (pressure > recomended){
    digitalWrite(buzzer, HIGH);

    delay(1000)
    digitalWrite(buzzer, LOW);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println("Release tension!");
    display.display(); 

    delay(2000);
    display.clearDisplay();
    
    // display current stich count
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println(stichcount);
    display.display();
  }
}
