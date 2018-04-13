#include "hw_pwr.h"
#include "hal_pwr.h"

struct _Power_Func sPower_Func={
  Power_SW_Port_Init,
	Power_SW
};
struct _Power_Func *psPower_Func=&sPower_Func;
