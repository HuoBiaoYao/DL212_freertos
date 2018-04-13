#include "hal_se_port.h"


struct _SE_FUNC sSE_FUNC={
  SE_ADC_Init,
	SE_Chan_Open,
	SE_Chan_Close,
	SE_AdcValue_Read,
	Bat_Read,
	Vref_Read,
	SE_OUT_Init,
	SE_OUT_Ctrl
};
struct _SE_FUNC *psSE_FUNC=&sSE_FUNC;

