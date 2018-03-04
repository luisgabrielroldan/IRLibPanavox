/*
 Name:		IRLibPanavox.cpp
 Created:	11/2/2017 11:00:23 PM
 Author:	Th3DarKZi0N
 Editor:	http://www.visualmicro.com
*/

#include "IRLibPanavox.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void IRsendPanavox::sendPacket(uint32_t value) {
	uint8_t* data = (uint8_t*)&value;

	mark(PANAVOX_AC_HDR_MARK);
	space(PANAVOX_AC_HDR_SPACE);

	for (int i = 0; i < 3; i++) {
		sendByte(data[i]);
		sendByte(~data[i]);
	}

	mark(PANAVOX_AC_BIT_MARK);
}

void IRsendPanavox::sendByte(uint8_t data) {
	for (int i = 7; i >= 0; i--) {
		mark(PANAVOX_AC_BIT_MARK);
		if (CHECK_BIT(data, i)) {
			space(PANAVOX_AC_ONE_SPACE);
		}
		else {
			space(PANAVOX_AC_ZERO_SPACE);
		}
	}
}

void IRsendPanavox::send(uint32_t cmd)
{
	enableIROut(PANAVOX_AC_FREQ);

	sendPacket(cmd);
	space(PANAVOX_AC_RPT_SPACE);
	sendPacket(cmd);
	space(PANAVOX_AC_END_SPACE);
}

int IRdecodePanavox::readNextByte(uint8_t offset) {
	uint8_t data = 0;
	uint8_t offset_limit = offset + 16;

	while (offset < offset_limit) {

		if (!MATCH(recvGlobal.decodeBuffer[offset], PANAVOX_AC_BIT_MARK)) {
			return READ_ERROR;
		}

		offset++;

		if (MATCH(recvGlobal.decodeBuffer[offset], PANAVOX_AC_ONE_SPACE)) {
			data = (data << 1) | 1;
		}
		else if (MATCH(recvGlobal.decodeBuffer[offset], PANAVOX_AC_ZERO_SPACE)) {
			data <<= 1;
		}
		else {
			return READ_ERROR;
		}

		offset++;
	}

	return data;
}

bool IRdecodePanavox::decode(void)
{
	this->resetDecoder();

	uint32_t data[6];
	uint8_t offset;

	if (MATCH(recvGlobal.decodeBuffer[0], PANAVOX_AC_RPT_SPACE))
		this->repeat = true;
	else
		this->repeat = false;

	if (recvGlobal.decodeLength != PANAVOX_RAW_COUNT)
		return RAW_COUNT_ERROR;

	// Check header
	if (!MATCH(recvGlobal.decodeBuffer[1], PANAVOX_AC_HDR_MARK))
		return HEADER_MARK_ERROR(PANAVOX_AC_HDR_MARK);

	if (!MATCH(recvGlobal.decodeBuffer[2], PANAVOX_AC_HDR_SPACE))
		return HEADER_SPACE_ERROR(PANAVOX_AC_HDR_SPACE);

	offset = 3; // Skip header and initial gap

	// Read data
	for (int i = 0; i < 6; i++) {
		int result = this->readNextByte(offset + (i * 16));
		if (result == READ_ERROR)
			return false;

		data[i] = (unsigned char)result;
	}

	// Check data integrity
	for (int i = 0; i < 6; i += 2) {
		if ((~data[i] & 0xFF) != data[i + 1])
			return false;
	}

	this->value = (data[4] & 0xFF) << 16 | (data[2] & 0xFF) << 8
		| (data[0] & 0xFF);

	this->address = 0;
	this->protocolNum = PANAVOX_AC;
	this->bits = 24;

	return true;
}

void IRdecodePanavox::dumpResults(bool verbose)
{
	Serial.println(this->value, HEX);
}


