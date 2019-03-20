#ifndef HDMI_BIT_H__
#define HDMI_BIT_H__

typedef struct s_HdmiBit {
  bool state;
  int time;
  // int difftime;
} HdmiBit;
const int HdmiBitBufferSize = 8;

class HdmiBitBuffer {
public:
  int idx = 0;
  int lastTime = 0;
  HdmiBit buffer[HdmiBitBufferSize];

  void push(int now, bool state) {
    if (idx >= HdmiBitBufferSize) {
      idx= 0;
    }
    buffer[idx].state = state;
    buffer[idx].time = now;
    // buffer[idx].difftime = now - lastTime;
    ++idx;
    lastTime = now;
  }

  void reset() {
      idx = 0;
  }

  void enqueue(HdmiBitBuffer& oth) {
      oth.idx = idx;
      idx= 0;
      oth.lastTime = lastTime;
      memcpy(oth.buffer, buffer, sizeof(HdmiBit) * HdmiBitBufferSize);
  }

};

#endif