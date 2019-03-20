#include <Arduino.h>

#include "cec-device.h"

CEC_Device *last;
void ICACHE_RAM_ATTR deviceISR() {
  last->isr();
}

/*
void ICACHE_RAM_ATTR deviceTimerEnd() {
  timer1_write(500000); 
  last->timerEnd();
}
*/

CEC_Device::CEC_Device(RemoteDebug &_debug)
: CEC_LogicalDevice(_debug)
, Debug(_debug)
// , _isrTriggered(false)
// , _lastLineState2(true)
{
}

void CEC_Device::isr() {
  const int now = micros();
  hdmiBitBuffer.push(now, digitalRead(_in_line));
}

void CEC_Device::begin(int in_line, int out_line)
{
  _in_line = in_line;
  _out_line = out_line;
  pinMode(_out_line, OUTPUT);
  pinMode( _in_line,  INPUT);

  last = this;
  /*
  Timer1.initialize(250);
  Timer1.attachInterrupt(timeoutEnd);
  timer1_attachInterrupt(deviceTimerEnd);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(500000); 
  */

  attachInterrupt(digitalPinToInterrupt(_in_line), deviceISR, CHANGE);

  digitalWrite(_out_line, LOW);
  delay(200);

  CEC_LogicalDevice::Initialize();
}

void CEC_Device::OnReady()
{
  Debug.printf("Device ready\n");
}

/*
void CEC_Device::OnCECPacket(int source, int dest, const unsigned char* buffer, int count)
{
  // This is called when a frame is received.  To transmit
  // a frame call TransmitFrame.  To receive all frames, even
  // those not addressed to this device, set Promiscuous to true.
  Debug.printf("Packet received at %ld: %02d %02d -> %02d: %02X", millis(), count, source, dest, ((source&0x0f)<<4)|(dest&0x0f));
  for (int i = 0; i < count; i++)
    Debug.printf(":%02X", buffer[i]);
  Debug.printf("\n");
}
*/

void CEC_Device::enqueueState(HdmiBitBuffer &ref)
{
  this->hdmiBitBuffer.enqueue(ref);
}

bool CEC_Device::LineState()
{
  int state = digitalRead(_in_line);
  return state == LOW;
}


void CEC_Device::SetLineState(bool state)
{
  /*
  Debug.printf("SetLineState:%d", state);
  digitalWrite(_out_line, state?LOW:HIGH);
  // give enough time for the line to settle before sampling
  // it
  delayMicroseconds(50);
  this->hdmiBitBuffer.reset();
  */
  // _lastLineState2 = LineState();
}

/*
void CEC_Device::SignalIRQ()
{
  // This is called when the line has changed state
  _isrTriggered = true;
}

bool CEC_Device::IsISRTriggered()
{
  if (_isrTriggered)
  {
    _isrTriggered = false;
    return true;
  }
  return false;
}
*/


void CEC_Device::Handle()
{
  /*
  bool state = LineState();
  // Debug.printf("CEC_Device::Handle:%d:%d\r", _in_line, state);
  if (_lastLineState2 != state)
  {
    DEBUG_D("LineState:port=%d:%d\n", _in_line, state);
    _lastLineState2 = state;
    SignalIRQ();
  }
  */
  /*
  if (idxISR) {
    char buf[64];
    strcpy(buf, "");
    for (int i = 0; i < idxISR; ++i) {
      sprintf(buf+strlen(buf), "[%d:%04d]", ISRS[i].state, ISRS[i].difftime);
    }
    DEBUG_I("I%s\n", buf);
  }
  idxISR = 0;
  */
  CEC_LogicalDevice::Handle();
}
