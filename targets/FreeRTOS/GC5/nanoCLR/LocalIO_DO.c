
#include "LocalIO_DO.h"

extern local_io_t s_local_io_tx;

/**
 * @brief  Get number of digital output ports
 * @note   
 * @retval number of DOs ports
 */
uint32_t GetDONumber() {
	return DIGITAL_OUTPUT_PORTS;
}

/**
 * @brief Checks digital output bit and returns its state
 * @note   
 * @param  DONum: digital output port number
 * @retval true - high, false - low
 */
bool GetDO(uint32_t DONum)
{	
	if (DONum > GetDONumber()) return false;

	bool state = false;
	state = (bool) (s_local_io_tx.digital_output >> (DONum +3)) & 1U;
	return state;
}

/**
 * @brief  Sets digital output bit state
 * @note   
 * @param  state: true or false (high or low)
 * @param  DONum: digital output port number
 * @retval None
 */
void SetDO(bool state, uint32_t DONum)
{
	if (DONum > GetDONumber()) return;
		
	if (state)
	{
		s_local_io_tx.digital_output |= 1U << (DONum + 3);
	}
	else
	{
		s_local_io_tx.digital_output &= ~(1U << (DONum + 3));
	}
}

/**
 * @brief  Toggle state of the digital output port of the UAC18
 * @note   
 * @param  DONum: digital output port number
 * @retval returns state of port after toggle
 */
void ToggleDO(uint32_t DONum)
{
	if (DONum > GetDONumber()) return;

	s_local_io_tx.digital_output ^= 1U << (DONum + 3);
}
