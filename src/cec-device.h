#ifndef CEC_DEVICE_H__
#define CEC_DEVICE_H__

#include <Ticker.h>

#include "cec-logical-device.h"

#include "hdmi-bit.h"


class CEC_Device : public CEC_LogicalDevice
{
public:
  CEC_Device(RemoteDebug &debug);
  
  // void Initialize();
  virtual void Handle();
  void begin(int in_line, int out_line);
  void isr();
  
protected:
  virtual void SetLineState(bool);
  virtual void enqueueState(HdmiBitBuffer &ref);
  virtual bool LineState();
/*
  virtual void SignalIRQ();
  virtual bool IsISRTriggered();
  virtual bool IsISRTriggered2() { return _isrTriggered; }
  */

  virtual void OnReady();
  // virtual void OnCECPacket(CECPacket &cp);
  
private:
  RemoteDebug &Debug;
  // bool _isrTriggered;
  // bool _lastLineState2;
  int _in_line;
  int _out_line;
  int timeoutEnd; 

};

#endif // CEC_DEVICE_H__
