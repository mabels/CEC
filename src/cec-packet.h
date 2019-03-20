#ifndef CECPacket_H__
#define CECPacket_H__

class CECPacket {
    public:
    RemoteDebug &Debug;
    SerialByte *buffer;
    unsigned char count;

    CECPacket(SerialByte *_buf, int _count, RemoteDebug &debug): 
        buffer(_buf), count(_count), Debug(debug) {
    }

    unsigned char sourceAddress() const {
        return (buffer[0].data() >> 4) & 0x0f;
    }

    unsigned char targetAddress() const {
        return buffer[0].data() & 0x0f;
    }
/*
	unsigned char buf[count - 1];
	for (int i = 1; i < count; ++i) {
		buf[i - 1] = buffer[i].data & 0xff;
	}
*/
    void dump() {
        char buf[6 + (count * sizeof(" %02d:A"))];
        strcpy(buf, "");
        char *sep = "";
        for (int i = 0; i < count; ++i) {
          sprintf(buf+strlen(buf), "%s%02x",
            sep, buffer[i].data());
          sep = " ";
        }
        strcat(buf, sep);
        for (int i = 0; i < count; ++i) {
          sprintf(buf+strlen(buf), "%c", 
            32 <= buffer[i].data() && buffer[i].data() < 'z' 
                ? buffer[i].data() : '.');
        }
        if (count > 0) {
            DEBUG_D("F%c[%s]\n", buffer[0].ack() ? '>' : '<', buf);
        }
    }
};

#endif