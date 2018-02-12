#include "ConfigButton.h"

void log(std::string msg){
  if (Serial && DBG_CB==1){
    Serial.println(msg.c_str());
  }
}

CallbackDuration::CallbackDuration(){
}

CallbackDuration::~CallbackDuration(){
}

CallbackDuration::CallbackDuration(int duration){
  setDuration(duration);
}

CallbackDuration::CallbackDuration(int duration, void (* callback)()){
  setCallback(callback);
  setDuration(duration);
}

void CallbackDuration::setCallback(void (*fn)()){
  _callback = fn;
}

void CallbackDuration::setDuration(int duration){
  _duration = duration;
}

void CallbackDuration::activate(){
  _active = true;
}

void CallbackDuration::deactivate(){
  _active = false;
}

bool CallbackDuration::isActive(){
  return _active;
}


void CallbackDuration::run(){
  _callback();
}

void ConfigButton::setPin(int pin){
  pinMode(pin, INPUT);
  _pin = pin;
}

void ConfigButton::setCallback(int duration, void (*fn)()){
  Serial.print("CB::setting up callback for duration ");
  Serial.println(duration);
  CallbackDuration callback(duration, fn);
  callback.activate();
  callbackMap.insert(std::make_pair(duration, callback));
}

void ConfigButton::setActive(int state ){
  this->_active = state;
}

 ConfigButton::ConfigButton(){
}

 ConfigButton::ConfigButton(int pin){
  setPin(pin);
}

 ConfigButton::ConfigButton(int pin, int active){
  setPin(pin);
  setActive(active);
}

ConfigButton::~ConfigButton(){
}

void ConfigButton::loop(){
  int pinValue = digitalRead(_pin);
  if(pinValue == _active){
    this->_pressDuration = millis()- _pressStart;
  }
  else {
    if (_pressDuration >0){
      Serial.print("CB::pressed for ");
      Serial.println(_pressDuration);
      int value= (int)(_pressDuration/1000);
      Serial.println("Callback value: ");
      Serial.print(value);

      if(runCallback(value)){
        log("callback ran.");
      }
      else{
        log("couldn't run callback.");
      }
    }
    this->_pressStart = millis();
    this->_pressDuration = 0;
  }
}

bool ConfigButton:: runCallback(int duration){

  if(callbackMap.find(duration) != callbackMap.end()){
    Serial.println("Found callback to call");
    callbackMap[duration].run();
    return true;
  }
  Serial.println("Didn't find callback");
  return false;
}
