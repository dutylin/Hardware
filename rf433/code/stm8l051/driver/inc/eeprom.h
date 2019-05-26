#ifndef EEPROM_H
#define EEPROM_H
#include "key.h"

#define KEYFUNC_DEFAULT         INCH
#define FREQ_DEFAULT            315
#define CHANNEL_DEFAULT         0
#define SN_DEFAULT              0X12 

params_typedef *system_params_get(void);
#endif
