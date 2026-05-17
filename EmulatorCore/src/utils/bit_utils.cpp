#include "bit_utils.h"

namespace BitUtils {
	bool GetMSB(uint8_t value) {
		return (value & 0b10000000) >> 7;
	}

	bool GetLSB(uint8_t value) {
		return value & 0b00000001;
	}
};