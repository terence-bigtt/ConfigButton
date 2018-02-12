#ifndef ConfigButton_h
#define ConfigButton_h
#include <Arduino.h>
#include <string>
#include <map>
#include <iterator>

#define MAX_ACTIONS 5
#define DBG_CB 1
#define DEBOUNCE 20
#define CLICK_TIMEOUT 500
#define PRESS_PERIOD 1000

void log(std::string msg);

class Callback{
public:
  Callback();
  Callback(void (*callback)());
  ~Callback();

  void setCallback(void (*callback)());
  void activate();
  void deactivate();
  bool isActive();
  void run();

private:
  void (*_callback)()=NULL;
  bool _active=false;
};

class ConfigButton{
public:
  ConfigButton();
  ConfigButton(int pin);
  ConfigButton(int pin, int active);
  ~ConfigButton();

  void setPin(int pin);
  void setPressCallback(int duration, void (*fn)());
  void setClickCallback(int duration, void (*fn)());
  void setActive(int state );
  void loop();

private:
  void readButton();
  int _nActions=0;
  int _nClick=0;
  int _pin=-1;
  int _active=-1;
  unsigned long _pressDuration=0;
  unsigned long _pressStart= 0;
  unsigned long _lastPressedSince = 0;
  unsigned long _pressedAt = 0;
  bool _pressing=false;
  bool runPressCallback(int duration);
  bool runClickCallback(int nClick);
  void resetPress();
  std::map<int, Callback> pressCallbackMap, clickCallbackMap;
};
#endif
