#ifndef ACMODE_H
#define ACMODE_H

enum class ACMode : uint8_t {
	Off = 0,
	Auto = 1,
	Cool = 2,
	Heat = 3,
	Dry = 4,
	Fan = 5
};

#endif // ACMODE_H