#include "ConfigButton.h"

void log(std::string msg){
  if (Serial && DBG_CB==1){
    Serial.println(msg.c_str());
  }
}

Callback::Callback(){
}

Callback::~Callback(){
}

Callback::Callback(void (* callback)()){
  setCallback(callback);

}

void Callback::setCallback(void (*fn)()){
  _callback = fn;
}

void Callback::activate(){
  _active = true;
}

void Callback::deactivate(){
  _active = false;
}

bool Callback::isActive(){
  return _active;
}

void Callback::run(){
  _callback();
}


void ConfigButton::setPin(int pin){
  pinMode(pin, INPUT);
  _pin = pin;
}

void ConfigButton::setPressCallback(int duration, void (*fn)()){
  Serial.print("CB::setting up callback for press of ");
  Serial.println(duration * PRESS_PERIOD);
  Callback callback(fn);
  callback.activate();
  pressCallbackMap.insert(std::make_pair(duration, callback));
}

void ConfigButton::setClickCallback(int nClick, void (*fn)()){
  Serial.print("CB::setting up callback for ");
  Serial.print(nClick);
  Serial.print(" clicks");
  Callback callback(fn);
  callback.activate();
  clickCallbackMap.insert(std::make_pair(nClick, callback));
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

void ConfigButton::readButton(){
  if(digitalRead(_pin) == _active){
    _pressing = true;
    _pressedAt = millis();
    _pressDuration = _pressedAt - _pressStart;
  }
  else {
    _pressing = false;
    if (_pressDuration > DEBOUNCE){
      _lastPressedSince = millis() - _pressedAt;
      }
    else{
      resetPress();
    }
  }
}

void ConfigButton::resetPress(){
  _pressStart = millis();
  _pressDuration = 0;
  _nClick=0;
}

void ConfigButton::loop(){
  readButton();
  if(!_pressing){
    if(_pressDuration<= DEBOUNCE){
      resetPress();
    }
    else if (_pressDuration < PRESS_PERIOD){
      Serial.print("CB::click detected, duration ");
      Serial.println(_pressDuration);
      if ( _lastPressedSince < CLICK_TIMEOUT){
        _nClick++;
      }
      else{
        if(runClickCallback(_nClick)){
          log("callback ran.");
        }
        else{
          log("couldn't run callback.");
        }
        resetPress();
      }
    }
    else {
      int value = (int)(_pressDuration / PRESS_PERIOD);
      if(runPressCallback(value)){
        log("callback ran.");
      }
      else{
        log("couldn't run callback.");
      }
      resetPress();
    }
  }
}

bool ConfigButton:: runPressCallback(int duration){
  if(pressCallbackMap.find(duration) != pressCallbackMap.end()){
    Serial.println("Found callback to call");
    pressCallbackMap[duration].run();
    return true;
  }
  Serial.println("Didn't find callback");
  return false;
}

bool ConfigButton:: runClickCallback(int nClick){
  if(clickCallbackMap.find(nClick) != clickCallbackMap.end()){
    Serial.println("Found callback to call");
    clickCallbackMap[nClick].run();
    return true;
  }
  Serial.println("Didn't find callback");
  return false;
}
