
#include <stdlib.h>
#include <string.h>

#ifdef ARDUINO_ARCH_AVR
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <Arduino.h>
#endif


#include "cec-logical-device.h"
#include "cec-device.h"

#include "serial-line.h"

#define MSG_ESC 0xfd
#define MSG_EOM 0xfe
#define MSG_START 0xff
#define ESC_OFFSET 3

typedef unsigned char Byte;

class PulseBuffer {
    public: 
        Byte buffer[32];
        Byte buflen; 
        bool escaped;

    public:
        static PulseBuffer empty() {
            const Byte *data = {};
            return PulseBuffer(data, 0);
        }

        int packetLen(const Byte *buf, int len) {
            int out = 0;
            if (len >= 1 && buf[0] != MSG_START) {
                ++out;
            }
            for (int i = 0; i < len; ++i) {
                if (buf[i] == MSG_ESC) {
                    ++out;
                }
                ++out;
            }
            if (len > 1 && buf[len - 1] != MSG_START) {
                ++out;
            }
            return out;
        }   

        PulseBuffer(const int *ibuf, int len): buflen(0), escaped(false) { 
            Byte buf[len];
            for (int i = 0; i < len; ++i) {
                buf[i] = ibuf[i];
            }
            // buffer = new byte[packetLen(buf, len)];
            buildPacket(buf, len);
        }
        PulseBuffer(const Byte *buf, int len): buflen(0), escaped(false) { 
            // buffer = new byte[packetLen(buf, len)];
            buildPacket(buf, len);
        }
        PulseBuffer(): buflen(0), escaped(false) {
            // buffer = new byte[64];
         }
        ~PulseBuffer() {
            // delete buffer;
        }

        void buildPacket(const Byte *buf, int len) {
            if (len >= 1 && buf[0] != MSG_START) {
                append(MSG_START);
            }
            for (int i = 0; i < len; ++i) {
                append(buf[i]);
            }
            if (len > 0 && buf[len - 1] != MSG_EOM) {
                append(MSG_EOM);
            }
        }

        void reset() {
            buflen = 0;
        } 

        void append(unsigned int c) {
            if (c == MSG_ESC) {
                appendByte(c);
                appendByte(c + ESC_OFFSET);
            } else {
                appendByte(c);
            }
        }

        void appendByte(Byte c) {
            if (buflen < sizeof(buffer) - 1) {
                buffer[buflen++] = c & 0xff;
                buffer[buflen] = 0;
            } else {
                // DbgPrint("PulseBuffer overflow: %d", buflen);
            }
        }
        
        void append(const PulseBuffer &buf) {
            for (int i = 0; i < buf.buflen; ++i) {
                append(buf.buffer[i]);
            }
        }

        PulseBuffer slice(unsigned int start, int end) {
           if (end >= 0 && (unsigned int)end >= buflen) {
               end = buflen;
           }
           if (end < 0) {
               end = buflen + end;
           }
           if (start <= end) {
               return PulseBuffer(buffer + start, end - start);
           }
           return PulseBuffer::empty();
        }

        void dump(const char *prefix) const {
            char buf[6 + (buflen * sizeof(" %02d:A"))];
            strcpy(buf, "");
            char *sep = "";
            for (int i = 0; i < buflen; ++i) {
                sprintf(buf+strlen(buf), "%s%02x",
                    sep, buffer[i]);
                sep = " ";
            }
            strcat(buf, sep);
            for (int i = 0; i < buflen; ++i) {
            sprintf(buf+strlen(buf), "%c", 
                32 <= buffer[i] && buffer[i] < 'z' 
                    ? buffer[i] : '.');
            }
            if (buflen > 0) {
                DEBUG_D("A%s[%s]\n", prefix, buf);
            }
        }
};

/*
class Output {
    public:
        virtual ~Output() { }
        virtual void write(Byte c) const = 0;
};
*/

static PulseBuffer buildGetDeviceType() {
    const int data[] = { (int)MSGCODE_GET_DEVICE_TYPE, 0x33, 0x0 };
    PulseBuffer buffer(data, sizeof(data)/sizeof(int));
    return buffer;

}
static const PulseBuffer getDeviceType = buildGetDeviceType();
static PulseBuffer buildGetAdapterType() {
    const int data[] = { (int)MSGCODE_GET_ADAPTER_TYPE, 0x33, 0x0 };
    PulseBuffer buffer(data, sizeof(data)/sizeof(int));
    return buffer;

}
static const PulseBuffer getAdapterType = buildGetAdapterType();
static PulseBuffer buildGetBuildDate() {
    const int data[] = { (int)MSGCODE_GET_BUILDDATE, 0x50, 0xa4, 0xce, 0x79 };
    PulseBuffer buffer(data, sizeof(data)/sizeof(int));
    return buffer;

}
static const PulseBuffer getBuildDate = buildGetBuildDate();
static PulseBuffer buildResponseFirmwareVersion() {
    const int data[] = { (int)MSGCODE_COMMAND_ACCEPTED, 0x0, 0x4 };
    PulseBuffer buffer(data, sizeof(data)/sizeof(int));
    return buffer;

}
static const PulseBuffer getResponseFirmwareVersion = buildResponseFirmwareVersion();

class PulseAudioShim {
    private:
        RemoteDebug &debug;
        Print &output;
        SendCecBuffer sendCecBuffer;
        PulseBuffer packet;
        CEC_Device *cecDevice;

    public:

    PulseAudioShim(RemoteDebug &debug, Print& _output/* , CEC_Device& _cecDevice */): 
        debug(debug),
        output(_output),
        sendCecBuffer(debug)
        /* cecDevice(_cecDevice) */ {
    }

    void begin(CEC_Device *_cecDevice) {
        cecDevice = _cecDevice;
    }

    void Handle() {
        while (Serial.available()) {
            unsigned int c = Serial.read();
            this->addPacket(c);
        }
    }

    void response(/* const char *msg, */const PulseBuffer &packet) {
        // DbgPrint(msg);
        packet.dump("<");
        for (int i = 0; i < packet.buflen; ++i) {
            output.write(packet.buffer[i]);
        }
    }

    void sendFrame(cec_adapter_messagecode msg, Byte data0) const {
        output.write(MSG_START);
        output.write(msg);
        output.write(data0);
        output.write(MSG_EOM);
    }

    void sendFrame(cec_adapter_messagecode msg, Byte data0, Byte data1) const {
        output.write(MSG_START);
        output.write(msg);
        output.write(data0);
        output.write(data1);
        output.write(MSG_EOM);
    }

    void sendCecFrame(CECPacket &pkt) {
/*
        PulseBuffer encoded(buffer, pkt.count);
        const int address = (pkt.targetAddress() & 0x0f) | 
                            ((pkt.sourceAddress() << 4) & 0x0f);
        sendFrame(MSGCODE_FRAME_START, address);
        Byte *encbuf = encoded.buffer;
        // int packetSize = 2;
        for (int i = 0; i < encoded.buflen; ) {
            const int diff = count - i;
            if (diff > 2) {
                sendFrame(MSGCODE_FRAME_DATA, encbuf[i++], encbuf[i++]);
                continue;
            } else if (diff > 1) {
                sendFrame((cec_adapter_messagecode)(MSGCODE_FRAME_DATA | MSGCODE_FRAME_EOM),
			  encbuf[i++], encbuf[i++]);
            } else {
                sendFrame((cec_adapter_messagecode)(MSGCODE_FRAME_DATA | MSGCODE_FRAME_EOM),
			   encbuf[i++]);
            }
        }
        */
        
    }

    void processHostPacket(PulseBuffer packet) {
        switch(packet.buffer[1]) {
            case MSGCODE_PING: 
                response(/* "PING",*/packet);
                break;

            case MSGCODE_TRANSMIT: 
                sendCecBuffer.append(packet.buffer, 2, packet.buflen - 1, false);
                response(/* "TRANSMIT",*/packet);
                break;
            case MSGCODE_TRANSMIT_EOM: 
                sendCecBuffer.append(packet.buffer, 2, packet.buflen - 1, true);
                // DEBUG_D("SEND:TransmitFrame\n");
                cecDevice->TransmitFrame(sendCecBuffer);
                sendCecBuffer.reset();
                response(/* "TRANSMIT_EOM",*/packet);
                break;
            case MSGCODE_TRANSMIT_IDLETIME:
                response(/* "TRANSMIT_IDLE",*/packet);
                break;
            case MSGCODE_TRANSMIT_ACK_POLARITY:
                sendCecBuffer.ack = packet.buffer[2] ? true : false;
                // onMessage(packet.slice(2, -1));
                response(/* "TRANSMIT_ACK_POLARITY",*/packet);
                break;

            case MSGCODE_SET_CONTROLLED: 
                response(/* "SET_CONTROLLED",*/packet);
                break;
            case MSGCODE_FIRMWARE_VERSION: 
                response(/* "FIRMWARE_VERSION",*/getResponseFirmwareVersion);
                break;
            case MSGCODE_GET_BUILDDATE:
                response(/* "GET_BUILDDATE",*/getBuildDate);
                break;
;
            case MSGCODE_GET_ADAPTER_TYPE:
                response(/* "GET_ADAPTER_TYPE",*/getAdapterType);
                break;
            case MSGCODE_GET_DEVICE_TYPE:
                response(/* "GET_DEVICE_TYPE",*/getDeviceType);
                break;
            default:
                DEBUG_E("unnkown AdapterPacket:%02x\n", packet.buffer[1]);
                break;
        }
    }



    void addPacket(unsigned int c) {
        if (0 <= c && c <= 0xff) {
            /*
            if ('a' <= c && c <= 'z') {
                char buf[4];
                sprintf(buf, "%02x", c);
                output.write(buf[0]);
                output.write(buf[1]);
            }
            */
            if (c == MSG_START) {
                /*
                if (packet.buflen > 0) {
                    DbgPrint("IGNORE DATA:");
                    char *colon = "";
                    for (int i = 0; i < packet.buflen; ++i) {
                        DbgPrint("%s%02x", colon, i);
                        colon = ":";
                    }
                    DbgPrint("\n");
                }
                */
                packet.reset();
                packet.append(c);

            } else if (c == MSG_EOM) {
                packet.append(c);
                packet.dump(">");
                processHostPacket(packet);
            } else {
                packet.append(c);
            }
        }
    }
};
