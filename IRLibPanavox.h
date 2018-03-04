/*
 Name:		IRLibPanavox.h
 Created:	11/2/2017 11:00:23 PM
 Author:	Th3DarKZi0N
 Editor:	http://www.visualmicro.com
*/

#ifndef IRLIB_PROTOCOL_PANAVOX
#define IRLIB_PROTOCOL_PANAVOX

#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>

#define PANAVOX_AC				80

#define PANAVOX_AC_FREQ         38

#define PANAVOX_RAW_COUNT		100

#define PANAVOX_AC_HDR_MARK		4300
#define PANAVOX_AC_HDR_SPACE	4500
#define PANAVOX_AC_BIT_MARK		500
#define PANAVOX_AC_ONE_SPACE	1600
#define PANAVOX_AC_ZERO_SPACE	600
#define PANAVOX_AC_RPT_SPACE	5400
#define PANAVOX_AC_END_SPACE	0

#define READ_ERROR				-1

#ifdef IRLIBSENDBASE_H
class IRsendPanavox : public virtual IRsendBase {
private:
	void sendByte(uint8_t data);
	void sendPacket(uint32_t value);
public:
	void send(uint32_t cmd);
};
#endif // IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodePanavox : public virtual IRdecodeBase {
private:
	int readNextByte(uint8_t offset);
public:
	bool repeat;
	bool decode(void);
	virtual void dumpResults(bool verbose = true);
};
#endif // IRLIBDECODEBASE_H
#endif // IRLIB_PROTOCOL_PANAVOX
