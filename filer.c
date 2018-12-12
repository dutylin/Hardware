#include "filter.h"

MANETIC_TYPEDEF  vmanetic_usr;
void manetic_filter(short int *data)
{
        unsigned char i;
	for(i=0;i<3;i++)
	{
		vmanetic_usr.S[i][vmanetic_usr.sample_index]=data[i];	
	}
	vmanetic_usr.sample_index = vmanetic_usr.sample_index + 1;
	for(i=0;i<3;i++)
	{
		vmanetic_usr.A[i][vmanetic_usr.sample_index]=data[i];	
	}	
	
	if(vmanetic_usr.sample_index >= N)
		vmanetic_usr.sample_index = 0;

}
