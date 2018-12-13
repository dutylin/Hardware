#include "filter.h"

MANETIC_TYPEDEF  vmanetic_usr;
void manetic_filter(float *data)
{
    unsigned char i,k;
	  float temp;

	//s(k),a(k) filter
	for(i=0;i<3;i++)
	{
		vmanetic_usr.S[i][vmanetic_usr.sample_index]=data[i];	
	}
	vmanetic_usr.sample_index = vmanetic_usr.sample_index + 1;
	
	for(i=0;i<3;i++)
	{
	    if(vmanetic_usr.sample_index<N)//k<N
	    {
				temp = 0;
				for(k=0;k<vmanetic_usr.sample_index;k++)
				{	
					temp=temp+vmanetic_usr.S[i][k];
				}
				temp = temp/vmanetic_usr.sample_index;
				vmanetic_usr.A[i][k-1] = temp;
				
		}
		else
		{
			temp = 0;
			k=vmanetic_usr.sample_index-N;
			for(;k<vmanetic_usr.sample_index;k++)//k>=N
			{	
				temp=temp+vmanetic_usr.S[i][k];
			}
			temp = temp /N;
			vmanetic_usr.A[i][k-1] = temp;		
		}
	}	
	//b(k) update 
	for(i=0;i<3;i++)
	{
    temp = 0;
		for(k=0;k<vmanetic_usr.sample_index;k++)
		{	
			temp=temp+vmanetic_usr.A[i][k];
		}
		temp = temp /vmanetic_usr.sample_index;
		vmanetic_usr.B[i][k] = temp;
		temp = 0;
		
	  if(vmanetic_usr.sample_index<50)//k<N
	  {
	    	;
		}
		else
		{
			if(vmanetic_usr.B[i][k]<=MAX_THRES)
			{
			    if(k>0)
					vmanetic_usr.B[i][k] = vmanetic_usr.B[i][k-1];
				else 
					vmanetic_usr.B[i][k] = vmanetic_usr.B[i][BUFFERSIZE-1];					
			}
			else
			{
				if(k>0)
				   vmanetic_usr.B[i][k] = vmanetic_usr.B[i][k-1]*(1-alpha)+
				                           vmanetic_usr.A[i][k]*alpha;
				else 
				  vmanetic_usr.B[i][k] = vmanetic_usr.B[i][BUFFERSIZE-1]*(1-alpha)+
				                           vmanetic_usr.A[i][k]*alpha;	   
			}		
		}
	}		
	if(vmanetic_usr.sample_index >= BUFFERSIZE)
		vmanetic_usr.sample_index = 0;
	printf("%f    %f    %f    %f    %f    %f    %f\n",data[0],data[1],data[2],vmanetic_usr.B[0][k],vmanetic_usr.B[1][k],vmanetic_usr.B[2][k],data[3]); 
	//printf("Manetic= %f 	%f	   %f ",data[0],data[1],data[2]);
  // printf("  %f\n",manetic_gauss_buff[3]);	


}
