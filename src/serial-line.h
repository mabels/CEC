#ifndef SERIAL_LINE_H__
#define SERIAL_LINE_H__

#include "RemoteDebug.h"

#define SERIAL_BUFFER_SIZE 16

class SerialByte {
public:
	unsigned short _data;
	unsigned char cnt;

	SerialByte(): _data(0), cnt(0) {}

	void reset() {
		_data = 0;
		cnt = 0;
	}

	bool complete() {
		return cnt >= 10;
	}

	SerialByte* push(int val) {
		val = val ? 1 : 0;
		if (cnt < 10) {
			if (cnt <= 7) {
				_data |= (val << (7-cnt));
			} else {
				_data |= (val << cnt);
			}
			++cnt;
		} 
		return this;
	}

	unsigned char data() {
		return _data & 0xff;
	}

	bool eom() {
		return _data & (1 << 8);
	}

	bool ack() {
		return _data & (1 << 9);
	}
};

class SerialLine {
private:
	RemoteDebug &Debug;
public:
	SerialLine(RemoteDebug &debug);
/*
	void ClearTransmitBuffer();
	virtual bool Transmit(const unsigned char* buffer, int count);
	virtual bool TransmitPartial(const unsigned char* buffer, int count);
	void RegisterReceiveHandler();
	*/

protected:
	SerialByte _receiveBuffer[SERIAL_BUFFER_SIZE];
	int _receiveBufferPos;
	/*
	unsigned char _transmitBuffer[SERIAL_BUFFER_SIZE];
	int _transmitBufferCount;
	int _transmitBufferBit;
	int _transmitBufferByte;

	// int _receiveBufferCount;
	// int _receiveBufferBit;


	// virtual void OnTransmitBegin() {;}

	int PopTransmitBit();
	int RemainingTransmitBytes();
	int TransmitSize();
	void ResetTransmitBuffer();
	*/

	SerialByte *PushReceiveBit(int);
	void ResetReceiveBuffer();
};

#endif // SERIAL_LINE_H__
