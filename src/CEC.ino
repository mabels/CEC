#include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "RemoteDebug.h"

RemoteDebug Debug;

#include "cec-logical-device.h"
#include "pulse-audio-shim.h"
#include "update-ota.h"

#define IN_LINE 12
#define OUT_LINE 13
// #define HPD_LINE 10

#define phy1 ((_physicalAddress >> 8) & 0xFF)
#define phy2 ((_physicalAddress >> 0) & 0xFF)

class MyCEC: public CEC_Device {
  private:
    // PulseAudioShim *pulseAudioShim;
    RemoteDebug &debug;

  public:
    MyCEC(RemoteDebug &_debug): 
      CEC_Device(_debug),
      debug(_debug)
      // pulseAudioShim(0)
    { }




    /*
    void setPulseAudioShim(PulseAudioShim *pas) {
      pulseAudioShim = pas;
    }
    */
    
    /*
    void reportPhysAddr()    { unsigned char frame[4] = { 0x84, phy1, phy2, 0x04 }; TransmitFrame(0x0F,frame,sizeof(frame)); } // report physical address
    void reportStreamState() { unsigned char frame[3] = { 0x82, phy1, phy2 };       TransmitFrame(0x0F,frame,sizeof(frame)); } // report stream state (playing)
    
    void reportPowerState()  { unsigned char frame[2] = { 0x90, 0x00 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report power state (on)
    void reportCECVersion()  { unsigned char frame[2] = { 0x9E, 0x04 };             TransmitFrame(0x00,frame,sizeof(frame)); } // report CEC version (v1.3a)
    
    void reportOSDName()     { unsigned char frame[5] = { 0x47, 'H','T','P','C' };  TransmitFrame(0x00,frame,sizeof(frame)); } // FIXME: name hardcoded
    void reportVendorID()    { unsigned char frame[4] = { 0x87, 0x00, 0xF1, 0x0E }; TransmitFrame(0x00,frame,sizeof(frame)); } // report fake vendor ID
    // TODO: implement menu status query (0x8D) and report (0x8E,0x00)
    */ 
    void OnCECPacket(CECPacket &cecPacket) {
      cecPacket.dump("F");
      // pulseAudioShim->sendCecFrame(cecPacket);
      /*
      PulseBuffer my;
      //my.append(source);
      //my.append(dest);
      int msgCode = (count == 1 ? MSGCODE_FRAME_EOM : 0) | MSGCODE_FRAME_START;
      for (int i = 0; i < count; ++i) {
        my.append(msgCode);
        my.append(buffer[i]);
        msgCode = (i + 1 != count - 1 ? MSGCODE_FRAME_EOM : 0) | MSGCODE_FRAME_DATA;
        my.reset();
      }
      */
    }
};

// TODO: set physical address via serial (or even DDC?)

// Note: this does not need to correspond to the physical address (i.e. port number)
// where the Arduino is connected - in fact, it _should_ be a different port, namely
// the one where the PC to be controlled is connected. Basically, it is the address
// of the port where the CEC-less source device is plugged in.
/* 
class MyOutput : public Output {
  public:
    virtual ~MyOutput() {
    }
    void write(unsigned char c) const {
      Serial.write(c);
    }
};
MyOutput out;
*/
MyCEC Cec(Debug);
PulseAudioShim PulseAudio(Debug, Serial);

void setup()
{
  Serial.begin(38400);
  // Serial.begin(115200);
  // Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  updateOTA(Serial, ArduinoOTA);
  Debug.begin("pulsotor", Debug.INFO);

  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  Cec.begin(IN_LINE, OUT_LINE);
  // device.setPulseAudioShim(&pulseAudioShim);
  PulseAudio.begin(&Cec);
  
  // device.MonitorMode = true;
  // device.Promiscuous = true;
  // device.Initialize(CEC_LogicalDevice::CDT_PLAYBACK_DEVICE);
}

static int o = 0;
void loop()
{
  Debug.handle();
  ArduinoOTA.handle();

  // FIXME: does it still work without serial connected?
  // try to toggle LED e.g.
  Cec.Handle();
  PulseAudio.Handle();
  

}
