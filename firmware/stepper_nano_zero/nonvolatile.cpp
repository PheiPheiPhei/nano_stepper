/**********************************************************************
 *      Author: tstern
 *
 *	Misfit Tech invests time and resources providing this open source code,
 *	please support Misfit Tech and open-source hardware by purchasing
 *	products from Misfit Tech, www.misifittech.net!
 *
 *	Written by Trampas Stern  for Misfit Tech.
 *	BSD license, check license.txt for more information
 *	All text above, must be included in any redistribution
 *********************************************************************/
#include "nonvolatile.h"
#include "Flash.h"  //thanks to Kent Larsen for pointing out the lower case error
#include <Arduino.h>




//we use this so we can hard code calibration table
// be sure to set the last word as status flag
// this save time calibrating each time we do a code build
#ifdef NZS_FAST_CAL
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[16640]={  //allocates 33280 bytes
#else
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[256]={  //allocates 512 bytes
#endif
    		//insert the getcal command calibration data here
6361,6688,7019,7343,7667,7992,8326,8647,8970,9296,9630,9950,10275,10602,10937,11259,11581,11909,12245,12568,12891,13218,13557,13877,14202,14532,14868,15193,15513,15843,16180,16504,16827,17155,17492,17815,18138,18464,18801,19122,19444,19770,20107,20428,20749,21073,21409,21731,22053,22375,22711,23033,23356,23677,24017,24338,24658,24984,25320,25643,25967,26292,26630,26957,27281,27604,27945,28274,28597,28927,29267,29594,29919,30248,30588,30916,31246,31575,31913,32241,32570,32897,33237,33564,33892,34221,34562,34886,35213,35541,35878,36203,36531,36856,37191,37515,37841,38166,38502,38822,39145,39470,39806,40128,40449,40776,41110,41431,41755,42079,42413,42734,43058,43384,43719,44041,44364,44688,45025,45347,45672,45998,46336,46657,46980,47308,47645,47968,48291,48618,48955,49279,49601,49927,50264,50586,50907,51231,51570,51890,52212,52536,52871,53192,53510,53837,54171,54490,54812,55135,55470,55789,56111,56434,56770,57091,57412,57737,58072,58393,58718,59043,59380,59706,60029,60357,60695,61020,61346,61675,62014,62339,62666,62998,63334,63663,63992,64322,64662,64989,65315,112,453,777,1105,1434,1774,2101,2428,2755,3093,3418,3742,4070,4406,4730,5054,5381,5714,6036,0xFFFF

};



static_assert (sizeof(nvm_t)<sizeof(NVM_flash), "nvm_t structure larger than allocated memory");




//FLASH_ALLOCATE(NVM_flash, sizeof(nvm_t));


bool nvmWriteCalTable(void *ptrData, uint32_t size)
{
	bool x=true;
	flashWrite(&NVM->CalibrationTable,ptrData,size);
	return true;
}

bool nvmWrite_sPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->sPID,&pid,sizeof(pid));
	return true;
}

bool nvmWrite_vPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->vPID,&pid,sizeof(pid));
	return true;
}

bool nvmWrite_pPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->pPID,&pid,sizeof(pid));
	return true;
}

bool nvmWriteSystemParms(SystemParams_t &systemParams)
{
	systemParams.parametersVaild=true;

	flashWrite((void *)&NVM->SystemParams,&systemParams,sizeof(systemParams));
	return true;
}

bool nvmWriteMotorParms(MotorParams_t &motorParams)
{
	motorParams.parametersVaild=true;

	flashWrite((void *)&NVM->motorParams,&motorParams,sizeof(motorParams));
	return true;
}

bool nvmErase(void)
{
	bool data=false;
	uint16_t cs=0;

	flashWrite((void *)&NVM->CalibrationTable.status,&data,sizeof(data));
	flashWrite((void *)&NVM->sPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->vPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->pPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->motorParams.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->SystemParams.parametersVaild ,&data,sizeof(data));
#ifdef NZS_FAST_CAL
	flashWrite((void *)&NVM->FastCal.checkSum,&cs,sizeof(cs));
#endif
}

