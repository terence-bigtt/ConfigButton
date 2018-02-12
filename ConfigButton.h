#ifndef ConfigButton_h
#define ConfigButton_h
#include <Arduino.h>
#include <string>
#include <map>
#include <iterator>

#define MAX_ACTIONS 5
#define DBG_CB 1
void log(std::string msg);

class CallbackDuration{
public:
  CallbackDuration();
  CallbackDuration(int duration);
  CallbackDuration(int duration, void (*callback)());
  ~CallbackDuration();

  void setDuration(int duration);
  void setCallback(void (*callback)());
  void activate();
  void deactivate();
  bool isActive();
  void run();

private:
  int _duration=-1;
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
  void setCallback(int duration, void (*fn)());
  void setActive(int state );
  void loop();

private:
  int readButton();
  int _nActions=0;
  int _pin=-1;
  int _active=-1;
  unsigned long _pressDuration=0;
  unsigned long _pressStart= 0;
  bool _pressing=false;
  bool runCallback(int duration);
  CallbackDuration callbacks[MAX_ACTIONS];
  std::map<int, CallbackDuration> callbackMap;
};
#endif
