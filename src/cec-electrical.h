#ifndef CEC_ELECTRICAL_H__
#define CEC_ELECTRICAL_H__

#include <Ticker.h>

#include "serial-line.h"
#include "cec-packet.h"
#include <RemoteDebug.h>

#include "hdmi-bit.h"

#define CEC_MAX_RETRANSMIT 5
const int CEC_ISR_RING_SIZE  = 18;

class CEC_Electrical : public RecvCecBuffer {
public:
	CEC_Electrical(RemoteDebug &debug);
	void Initialize();
	// void SetAddress(int address);

	unsigned long Process();
	bool TransmitPending() { return _primaryState == CEC_TRANSMIT && _secondaryState == CEC_IDLE_WAIT; }
	virtual bool TransmitFrame(SendCecBuffer &sbuf);

	void bitEndISR();


protected:
	virtual void enqueueState(HdmiBitBuffer &isrs) = 0;
	virtual void SetLineState(bool) = 0;
	virtual bool LineState() = 0;

  	HdmiBitBuffer hdmiBitBuffer;

private:
	typedef enum {
		CEC_IDLE,
		CEC_TRANSMIT,
		CEC_RECEIVE,
	} CEC_PRIMARY_STATE;

	typedef enum {
		CEC_RCV_STARTBIT1,
		CEC_RCV_STARTBIT2,
		CEC_RCV_DATABIT1,
		// CEC_RCV_DATABIT2,
		CEC_RCV_DATABIT2,
		// CEC_RCV_ACK_SENT,
		// CEC_RCV_ACK1,
		// CEC_RCV_ACK2,
		// CEC_RCV_LINEERROR,

		CEC_IDLE_WAIT,
		CEC_XMIT_STARTBIT1,
		CEC_XMIT_STARTBIT2,
		CEC_XMIT_DATABIT1,
		CEC_XMIT_DATABIT2,
		CEC_XMIT_ACK,
		CEC_XMIT_ACK2,
		CEC_XMIT_ACK3,
		CEC_XMIT_ACK_TEST,
	} CEC_SECONDARY_STATE;

	typedef enum {
		CEC_RCV_BIT0,
		CEC_RCV_BIT1,
		CEC_RCV_BIT2,
		CEC_RCV_BIT3,
		CEC_RCV_BIT4,
		CEC_RCV_BIT5,
		CEC_RCV_BIT6,
		CEC_RCV_BIT7,
		CEC_RCV_BIT_EOM,
		CEC_RCV_BIT_ACK,

		CEC_ACK,
		CEC_NAK,

		CEC_XMIT_BIT0,
		CEC_XMIT_BIT1,
		CEC_XMIT_BIT2,
		CEC_XMIT_BIT3,
		CEC_XMIT_BIT4,
		CEC_XMIT_BIT5,
		CEC_XMIT_BIT6,
		CEC_XMIT_BIT7,
		CEC_XMIT_BIT_EOM,
		CEC_XMIT_BIT_ACK,


		CEC_IDLE_RETRANSMIT_FRAME,
		CEC_IDLE_NEW_FRAME,
		CEC_IDLE_SUBSEQUENT_FRAME,
	} CEC_TERTIARY_STATE;

private:
	bool ResetState();
	/*
	void ResetTransmit(bool retransmit);
	virtual void OnTransmitBegin();
	virtual void OnTransmitComplete(bool);
	*/

	virtual void OnCECFrame(SerialByte* buffer, int count) = 0;

	void ProcessFrame();

	// Helper functions
	bool Raise();
	bool Lower();
	void HasRaised(unsigned long);
	void HasLowered(unsigned long);
	bool CheckAddress();
	// SerialByte *ReceivedBit(bool);
	unsigned long LineError(const char *, int diffTime);
	unsigned long processISR(HdmiBit &isr);

	// void dump();
	

	// int _address;
	RemoteDebug &Debug;

	bool _lastLineState;
	unsigned long _lastStateChangeTime;
	unsigned long _bitStartTime;
	// int timerTick;

	int _xmitretry;

	/*
	bool _eom;
	bool _follower;
	bool _broadcast;
	bool _amLastTransmittor;
	bool _transmitPending;

	HdmiBit isrRing[CEC_ISR_RING_SIZE];
	int isrPos;
	int isrRingLen;
	int isrStartBitPos;
	int isrTime;
  	Ticker ticker;
	*/


	CEC_PRIMARY_STATE _primaryState;
	CEC_SECONDARY_STATE _secondaryState;
	// CEC_TERTIARY_STATE _tertiaryState;
};



#endif // CECWIRE_H__
