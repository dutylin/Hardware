#include "filter.h"
#include <math.h>
MANETIC_TYPEDEF  vmanetic_usr;
unsigned char sample_count,not_first_run;
unsigned char event_end_count;
unsigned char event_start_count;
unsigned char timeout;
unsigned char  manetic_filter(unsigned char state,float *data)
{
   unsigned char i,k;
   float temp;
	 float t1,t2;

	//s(k),a(k) filter
	for(i=0;i<3;i++)
	{
		vmanetic_usr.S[i][vmanetic_usr.sample_index]=data[i];	
	}
	vmanetic_usr.Tse[0]=vmanetic_usr.Tse[0]+0.1;//systime 
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
	if(state == INIT)//init 50 times
	{

		//b(k) update 
		for(i=0;i<3;i++)
		{
			temp = 0;
			for(k=0;k<vmanetic_usr.sample_index;k++)
			{	
				temp=temp+vmanetic_usr.A[i][k];
			}
			temp = temp /vmanetic_usr.sample_index;
			vmanetic_usr.B[i][1] = temp;
			vmanetic_usr.B[i][0] = vmanetic_usr.B[i][1] ;
			temp = 0;


		}	
	}
	else  //trace base line
	{
		for(i=0;i<3;i++)
		{
			 if(i==2)
			 {   
				  t1 = fabs(vmanetic_usr.B[i][1]);
				  t2 = fabs(vmanetic_usr.S[i][vmanetic_usr.sample_index-1]);				 
					if((t1-t2)>=MAX_THRES)
					{

						event_start_count = event_start_count +1;
						temp=vmanetic_usr.B[i][1];
						vmanetic_usr.B[i][1] = vmanetic_usr.B[i][0];
						vmanetic_usr.B[i][0]=temp;	
						if(event_start_count>=20)
						{   
							if(vmanetic_usr.event_update_flag == 0)
							{
								vmanetic_usr.Tse[1] = vmanetic_usr.Tse[0];	
								vmanetic_usr.event_update_flag = 1;
							}

							if(vmanetic_usr.B[i][1]<=vmanetic_usr.A[i][vmanetic_usr.sample_index-1])
							{
								event_start_count = 0;
								vmanetic_usr.Tp[i][vmanetic_usr.mp_index] = vmanetic_usr.Tse[0];
								vmanetic_usr.Mp[i][vmanetic_usr.mp_index] = vmanetic_usr.S[i][vmanetic_usr.sample_index-1];
								vmanetic_usr.mp_index = vmanetic_usr.mp_index + 1;
							}
							else
							{
								event_start_count = 0;
								vmanetic_usr.Tv[i][vmanetic_usr.mp_index] = 0;							
								vmanetic_usr.Mv[i][vmanetic_usr.mv_index] = vmanetic_usr.S[i][vmanetic_usr.sample_index-1];
								vmanetic_usr.mv_index = vmanetic_usr.mv_index + 1;
							}

						}
					}
			else
			{
			     event_start_count = 0;
				   temp=vmanetic_usr.B[i][1];
				   vmanetic_usr.B[i][1] = vmanetic_usr.B[i][0]*(1-alpha)+
									vmanetic_usr.A[i][vmanetic_usr.sample_index-1]*alpha;			
				   vmanetic_usr.B[i][0]=temp;	
				if(vmanetic_usr.event_update_flag == 1)
				{
				       event_end_count = event_end_count + 1;
					if(event_end_count>=20)
					{
				    vmanetic_usr.Tse[2] = vmanetic_usr.Tse[0];	
						vmanetic_usr.event_update_flag = 0;
						event_end_count = 0;
					}

				}				
			}		 
			 }
					
		}

	}
	
	if(vmanetic_usr.sample_index >= BUFFERSIZE)
	{
		vmanetic_usr.sample_index = 0;
	}
		printf("%f    %f    %f    %f    %f    %f    %f\n",data[0],data[1],data[2],vmanetic_usr.B[0][1],vmanetic_usr.B[1][1],vmanetic_usr.B[2][1],data[3]); 

	if(vmanetic_usr.sample_index<50)//k<N
	{
		return 0;
	}
	else
		return 1;		
}
