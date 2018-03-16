#ifndef PANAVOX_AC_H
#define PANAVOX_AC_H

#include "IRLibPanavox.h"

#define PANAVOX_AC_TEMP_MIN 17
#define PANAVOX_AC_TEMP_MAX	30

#include "ACmode.h"

class PanavoxAC
{
private:
  ACMode _mode;
	uint8_t _temperature;
	uint8_t _fanSpeed;
	IRsendPanavox _irSender;
public:
	PanavoxAC();
	~PanavoxAC();

	void sendState(void);

	// Getters/Setterss
	void setMode(ACMode mode);
	ACMode getMode(void);
	void setTemperature(uint8_t temp);
	uint8_t getTemperature(void);
	void setFanSpeed(uint8_t level);
	uint8_t getFanSpeed(void);

	// Extra commands
	void sendMoveFlap(void);
	void sendSwapLight(void);
	void sendSuperMode(void);
	void sendAutoClean(void);
	void sendFlapOscillation(void);
};

#endif // PANAVOX_AC_H