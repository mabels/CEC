
#include "serial-line.h"

RecvCecBuffer::RecvCecBuffer(RemoteDebug &debug): Debug(debug)
{
	// _transmitBufferCount = 0;
	// _transmitBufferBit = 7;
	// _transmitBufferByte = 0;
	// _receiveBufferCount = 0;
	// _receiveBufferBit = 7;
	_receiveBufferPos = 0;
}

	/*
void RecvCecBuffer::OnReceiveComplete(SerialByte *buffer, int count)
{
	ResetReceiveBuffer();
}

void RecvCecBuffer::ClearTransmitBuffer()
{
	_transmitBufferCount = 0;
	_transmitBufferBit = 7;
	_transmitBufferByte = 0;
}

bool RecvCecBuffer::Transmit(const unsigned char* buffer, int count)
{
	if (!TransmitPartial(buffer, count))
		return false;

	OnTransmitBegin();
	return true;
}

bool RecvCecBuffer::TransmitPartial(const unsigned char* buffer, int count)
{
	if (count < 0 || count >= (SERIAL_BUFFER_SIZE - _transmitBufferCount))
		return false;

	for (int i = 0; i < count; i++)
		_transmitBuffer[_transmitBufferCount + i] = buffer[i];
	_transmitBufferCount += count;
	_transmitBufferBit = 7;
	_transmitBufferByte = 0;
	return true;
}

int RecvCecBuffer::PopTransmitBit()
{
	if (_transmitBufferByte == _transmitBufferCount)
		return 0;

	int bit = (_transmitBuffer[_transmitBufferByte] >> _transmitBufferBit) & 1;
	if( _transmitBufferBit-- == 0 )
	{
		_transmitBufferBit = 7;
		_transmitBufferByte++;
	}
	return bit;
}

int RecvCecBuffer::RemainingTransmitBytes()
{
	return _transmitBufferCount - _transmitBufferByte;
}

int RecvCecBuffer::TransmitSize()
{
	return _transmitBufferCount;
}
	*/

SerialByte *RecvCecBuffer::PushReceiveBit(int val) {
	SerialByte *complete = _receiveBuffer[_receiveBufferPos].push(val);
	if (complete->complete()) {
		/*
		DEBUG_D("PushReceiveBit:%d:%03x:%03x\n",
			 _receiveBufferByte, 
			 _receiveBuffer[_receiveBufferByte].data,
			 ~_receiveBuffer[_receiveBufferByte].data);
		*/
		_receiveBufferPos++;
		if (_receiveBufferPos >= SERIAL_BUFFER_SIZE) {
			DEBUG_E("RecvCecBuffer wrap recv buffer");
			ResetReceiveBuffer();
		}
	}
	return complete;
}

/*
int RecvCecBuffer::ReceivedBytesPos()
{
	return _receiveBufferPos;
}
*/

void RecvCecBuffer::ResetReceiveBuffer()
{
	for (int i = 0; i < _receiveBufferPos; ++i) {
		_receiveBuffer[i].reset();
	}
	_receiveBufferPos = 0;
}

/*
void RecvCecBuffer::ResetTransmitBuffer()
{
	_transmitBufferBit = 7;
	_transmitBufferByte = 0;
}
*/