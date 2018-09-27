#include "dl212_ads1248.h"
#include "ads1248.h"

//AIN2---GND,AIN3---BAT

float Battery(void){
	ADS1248SetChannel(3,2);
  adc = ADS1248RDATARead()*3/0x7FFFFF;
	adc = adc*68/268;
	
	return adc;
}

float VoltDiff(unsigned char chan){
	float adc=0;
	
  switch (chan):
		case 1:
			;
		break;
		case 2:
			;
		break;
		case 3:
			;
		break;
	}
	adc = ADS1248RDATARead()*3/0x7FFFFF;
}

float VoltSe(unsigned char chan){
  switch (chan):
		case 1:
			;
		break;
		case 2:
			;
		break;
		case 3:
			;
		break;
		switch (chan):
		case 4:
			;
		break;
		case 5:
			;
		break;
		case 6:
			;
		break;
	}
  adc = ADS1248RDATARead()*3/0x7FFFFF;
} 