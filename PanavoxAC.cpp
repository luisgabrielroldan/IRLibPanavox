#include "PanavoxAC.h"

#define PANAVOX_AC_SIGN         0x0B
#define PANAVOX_AC_CMD_MODE		2
#define PANAVOX_AC_CMD_EXTRA	5

#define FAN_SPEED_0  5
#define FAN_SPEED_1  4
#define FAN_SPEED_2  2
#define FAN_SPEED_3  1

#define SETMODE(cmd,value)			cmd->param2.mode = (uint8_t)value
#define SETFANSPEED(cmd, value)		cmd->param1.fan_speed = (uint8_t)value
#define SETTEMP(cmd, value)			cmd->param2.temp = (uint8_t)value

enum class PanavoxACMode : uint8_t {
	Cool = 0,
	DryFan = 1,
	Auto = 2,
	Heat = 3
};

typedef uint8_t ac_mode_t;

typedef struct panavox_ac_cmd_t {
	uint8_t type : 4;
	uint8_t sign : 4;
	uint8_t param1;
	uint8_t param2;
} panavox_ac_cmd_t;

typedef struct panavox_ac_cmd_mode_param1_t {
	uint8_t ext : 5; // Fill with ones
	uint8_t fan_speed : 3;
} panavox_ac_cmd_mode_param1_t;

typedef struct panavox_ac_cmd_mode_param2_t {
	uint8_t ext : 2;
	uint8_t mode : 2;
	uint8_t temp : 4;
} panavox_ac_cmd_mode_param2_t;

typedef struct panavox_ac_cmd_mode_t {
	uint8_t type : 4;
	uint8_t sign : 4;
	panavox_ac_cmd_mode_param1_t param1;
	panavox_ac_cmd_mode_param2_t param2;
} panavox_ac_cmd_mode_t;

static uint8_t FAN_SPEEDS[] = {	FAN_SPEED_0, FAN_SPEED_1, FAN_SPEED_2, FAN_SPEED_3 };

uint8_t encodeTemp(uint8_t temp) {
	uint8_t value = temp - 17;
	if (value == 10 || value == 11)
		value += 4;
	return value ^ (value >> 1);
}

PanavoxAC::PanavoxAC()
{
}


PanavoxAC::~PanavoxAC()
{
}

void PanavoxAC::sendState(void)
{
	panavox_ac_cmd_t cmd;
	panavox_ac_cmd_mode_t* cmd_mode = (panavox_ac_cmd_mode_t*)&cmd;

	cmd.sign = PANAVOX_AC_SIGN;
	cmd.type = PANAVOX_AC_CMD_MODE;

	cmd_mode->param1.ext = 0x1F;
	cmd_mode->param2.ext = 0;

	SETFANSPEED(cmd_mode, FAN_SPEEDS[this->_fanSpeed]);
	SETTEMP(cmd_mode, encodeTemp(this->_temperature));

	if (this->_mode == ACMode::Off) {
		cmd.param1 = 0x7B;
		cmd.param2 = 0xE0;
	}
	else if (this->_mode == ACMode::Auto) {
		SETMODE(cmd_mode, PanavoxACMode::Auto);
		SETFANSPEED(cmd_mode, 0);
	}
	else if (this->_mode == ACMode::Dry) {
		SETMODE(cmd_mode, PanavoxACMode::DryFan);
		SETFANSPEED(cmd_mode, 0);
	}
	else if (this->_mode == ACMode::Fan) {
		SETMODE(cmd_mode, PanavoxACMode::DryFan);
		SETTEMP(cmd_mode, 0x0E);
	}
	else if (this->_mode == ACMode::Cool) {
		SETMODE(cmd_mode, PanavoxACMode::Cool);
	}
	else if (this->_mode == ACMode::Heat) {
		SETMODE(cmd_mode, PanavoxACMode::Heat);
	}

	uint32_t value = *((uint32_t*)&cmd) & 0x00FFFFFF;
	Serial.println(value, HEX);
	this->_irSender.send(value);
}

void PanavoxAC::setMode(ACMode mode)
{
	this->_mode = mode;
}

ACMode PanavoxAC::getMode(void)
{
	return this->_mode;
}

void PanavoxAC::setTemperature(uint8_t temp)
{
	this->_temperature = max(min(temp, PANAVOX_AC_TEMP_MAX), PANAVOX_AC_TEMP_MIN);
}

uint8_t PanavoxAC::getTemperature(void)
{
	return this->_temperature;
}

void PanavoxAC::setFanSpeed(uint8_t level)
{
	this->_fanSpeed = min(level, 3);
}

uint8_t PanavoxAC::getFanSpeed(void)
{
	return this->_fanSpeed;
}
