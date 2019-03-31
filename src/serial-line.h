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

	void setData(unsigned char data) {
		_data &= ~0xff;
		_data |= data;
	}

	void setAck(bool val) {
		_data &= ~(1 << 9);
		_data |= ((val ? 1: 0) << 9);
	}

	void setEom(bool val) {
		_data &= ~(1 << 8);
		_data |= ((val ? 1: 0) << 8);
	}
};

class SendCecBuffer {
private:
	RemoteDebug &Debug;

public:
	bool ack;
	SerialByte _sendBuffer[SERIAL_BUFFER_SIZE];
	int _sendBufferPos;

	SendCecBuffer(RemoteDebug &debug): 
		Debug(debug)
		, ack(false)
		, _sendBufferPos(0) {

	};

	void append(unsigned char *c, int start, int end, bool eom = false) {
		for (int i = start; i < end; ++i) {
			/*
			if (c[i] == MSG_ESC) {
				DEBUG_E("ESC is not implemented");
			}
			*/
			_sendBuffer[_sendBufferPos].setData(c[i]);
			_sendBuffer[_sendBufferPos].setAck(ack);
			if (i + 1 == start) {
				_sendBuffer[_sendBufferPos].setEom(eom);
			}
			++_sendBufferPos;
		}
	}

	void reset() {
		ack = false;
		_sendBufferPos = 0;
		DEBUG_D("\n");
	}

};

class RecvCecBuffer {
private:
	RemoteDebug &Debug;
public:
	RecvCecBuffer(RemoteDebug &debug);
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
