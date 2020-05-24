#include <Arduino.h>
#include "Display.h"
#include "config.h"
#include <ESP32Encoder.h>
#include <ArduinoNvs.h>

Display display;

bool selected;
bool forward;
bool backward;
int scrollIndex;
ESP32Encoder encoder;
ESP32Encoder button;
uint16_t speed, setspeed;
const int freq = 400;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;

uint16_t sw1_debounce, sw2_debounce;

void goDown() {
  if(selected) {
    //
  } else {
    //
  }
  if(speed < 0xFF)
  {
    speed++;
  }
}

void goUp() {
  if(selected) {
    //
  } else {
    //
  }
  if(speed > 0)
  {
    speed--;
  }
}

void onClick() {
  selected = !selected;
}


void handleEncoder() {
    int encoderValue = encoder.getCount();
    bool move = encoderValue > 0;
    encoder.setCount(move ? (encoderValue & 1) : - (encoderValue & 1) );
    encoderValue = abs(encoderValue)/2;
    for(int i=0; i<encoderValue; i++) {
        if(move) goUp(); 
        else goDown();
    }
    for(int i=0; i<button.getCount(); i++) {
        onClick();
    }   
    button.setCount(0);
}

void set_pwm_outputs() {
  
  if(speed > 0 && forward)
  {
    ledcWrite(pwmChannelA, speed);
    ledcWrite(pwmChannelB, 0);
  }
  else if(speed > 0 && backward)
  {
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelB, speed);
  }
  else
  {
    ledcWrite(pwmChannelA, 0);
    ledcWrite(pwmChannelB, 0);
  }
  
}

void setup() {
  Serial.begin(115200);
  display.begin();
  display.clear();  
  NVS.begin();
  encoder.attachSingleEdge(ENC_B, ENC_A);
  button.attachSingleEdge(ENC_SW, -1);
  button.setCount(0);
  encoder.setCount(0);

  speed = NVS.getInt("speed");
  setspeed = speed;
  sw1_debounce = 0;
  sw2_debounce = 0;

  display.update();

  ledcSetup(pwmChannelA, freq, resolution);
  ledcAttachPin(PWM_A, pwmChannelA);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(PWM_B, pwmChannelB);
  forward = 0;
  backward = 0;
  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
}

void loop() {
  set_pwm_outputs();

  sw1_debounce = (sw1_debounce << 1) | digitalRead(SW_1);
  sw2_debounce = (sw2_debounce << 1) | digitalRead(SW_2);

  if(sw1_debounce == 0x8000) {
    forward = 1;
  } else if(sw1_debounce == 0x7FFF) {
    forward = 0;
  }
  if(sw2_debounce == 0x8000) {
    backward = 1;
  } else if(sw2_debounce == 0x7FFF) {
    backward = 0;
  }

  if(forward) {
    display.draw(2,3,"Forward");
  } else if(backward) {
    display.draw(2,3,"Backward");
  }

  handleEncoder();
  char title[11] = "Solder Pen";
  char value[15];

  sprintf(value, "Speed: %3d/255", speed);
  display.draw(2,2,value);
  display.draw(2,1,title);

  if(setspeed != speed)
  {
    setspeed = speed;
    NVS.setInt("speed", speed);
  }
  

  display.update();
}