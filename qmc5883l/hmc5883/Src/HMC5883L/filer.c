#include "filter.h"
#include <math.h>

MANETIC_TYPEDEF  vmanetic_usr;
unsigned char sample_count,not_first_run;
unsigned char event_end_count;
unsigned char event_start_count;
unsigned char NoEvent_count;
extern unsigned int timeout;
float *data2;
float e1[3],e2[3];
unsigned char  manetic_filter(unsigned char state,float *data)
{
	unsigned char i,k,n;
	float temp;
	float t1,t2;
	data2=data;
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
		printf("%f    %f    %f    %f    %f     %f    %f     %f     %f",
		data2[0],data2[1],data2[2],
		vmanetic_usr.B[0][1],
		vmanetic_usr.B[1][1],
		vmanetic_usr.B[2][1],
		vmanetic_usr.Mpv[0][vmanetic_usr.mpv_index],
		vmanetic_usr.Mpv[1][vmanetic_usr.mpv_index],
		vmanetic_usr.Mpv[2][vmanetic_usr.mpv_index]
		);

		printf("    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f ",
		vmanetic_usr.TI[0][vmanetic_usr.mpv_index],
		vmanetic_usr.TI[1][vmanetic_usr.mpv_index],
		vmanetic_usr.TI[2][vmanetic_usr.mpv_index],
		vmanetic_usr.Tse[0],
		vmanetic_usr.Tse[1],
		vmanetic_usr.Tse[2],
		e1[0],e1[1],e1[2],
		e2[0],e2[1],e2[2]
		); 	
		printf("   %f \n",data2[3]);

	}
	else  //trace base line
	{
		for(i=0;i<3;i++)
		{  
			t1 = (vmanetic_usr.B[i][1]);
			t2 = (vmanetic_usr.S[i][vmanetic_usr.sample_index-1]);		
			e1[i] = fabs(t1-t2);
			if( e1[i]>=MAX_THRES)// over max thresh
			{
				event_start_count = event_start_count +1;
				temp=vmanetic_usr.B[i][1];
				vmanetic_usr.B[i][1] = vmanetic_usr.B[i][0];
				vmanetic_usr.B[i][0]=temp;	
				if(event_start_count>=10)//over setting count 
				{   
					if(vmanetic_usr.B[i][1]<=vmanetic_usr.S[i][vmanetic_usr.sample_index-1])//update mp
					{
						event_start_count = 0;
						vmanetic_usr.TI[i][vmanetic_usr.mpv_index] = vmanetic_usr.Tse[0];
						vmanetic_usr.Mpv[i][vmanetic_usr.mpv_index] = vmanetic_usr.S[i][vmanetic_usr.sample_index-1];
						vmanetic_usr.mpv_index = vmanetic_usr.mpv_index + 1;
					}
					else  if(vmanetic_usr.event_update_flag == 1)//update mv
					{
						event_start_count = 0;
						vmanetic_usr.TI[i][vmanetic_usr.mpv_index] =  vmanetic_usr.Tse[0];							
						vmanetic_usr.Mpv[i][vmanetic_usr.mpv_index] = vmanetic_usr.S[i][vmanetic_usr.sample_index-1];
						vmanetic_usr.mpv_index= vmanetic_usr.mpv_index + 1;
					}

					
					if(vmanetic_usr.event_update_flag == 0||
					vmanetic_usr.event_update_flag == 2)
					{
						vmanetic_usr.Tse[1] = vmanetic_usr.Tse[0];	
						vmanetic_usr.event_update_flag = 1;
					}
		/*	n=0;
	printf("%f    %f    %f    %f    %f     %f    %f     %f     %f",
				n,n,n,
				n,n,n,
				vmanetic_usr.Mpv[0][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Mpv[1][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Mpv[2][vmanetic_usr.mpv_index-1]
				);

	  printf("    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f ",
				vmanetic_usr.TI[0][vmanetic_usr.mpv_index-1],
				vmanetic_usr.TI[1][vmanetic_usr.mpv_index-1],
				vmanetic_usr.TI[2][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Tse[0],
				vmanetic_usr.Tse[1],
				vmanetic_usr.Tse[2],
				e1[0],e1[1],e1[2],
				e2[0],e2[1],e2[2]
				); 	
			printf("   %f \n",data2[3]);*/
				}
			}
			else //update b(k)
			{
				if(event_start_count==0&&vmanetic_usr.event_update_flag != 1)
				{
					event_start_count = 0;//remove shake
						 // NoEvent_count = 0;
					vmanetic_usr.event_update_flag =0;
					temp=vmanetic_usr.B[i][1];
					vmanetic_usr.B[i][1] = vmanetic_usr.B[i][0]*(1-alpha)+
					vmanetic_usr.A[i][vmanetic_usr.sample_index-1]*alpha;			
					vmanetic_usr.B[i][0]=temp;		
					
				}


				if(vmanetic_usr.event_update_flag == 1)
				{
					event_end_count = event_end_count + 1;
					if(event_end_count>=10)
					{
						vmanetic_usr.Tse[2] = vmanetic_usr.Tse[0];	
						vmanetic_usr.event_update_flag = 2;
						event_end_count = 0;
						//vmanetic_usr.Tse[0]=0;//systime 
					}					
				}	
				
				
			}		 
		}

	}

	if(vmanetic_usr.sample_index >= BUFFERSIZE)
	{
		vmanetic_usr.sample_index = 0;
	}
	if(vmanetic_usr.mpv_index >= BUFFERSIZE)
	{
		vmanetic_usr.mpv_index = 0;
	}



	if(vmanetic_usr.sample_index<50)//k<N
	{
		return 0;
	}
	else
		return 1;		
}
float max_mp;
float min_mv;
void max_mpv(float max_mp_tmp,float max_mv_tmp)
{
	unsigned char i,j;
	for(j=0;j<3;j++)
	{
		for(i=0;i<(vmanetic_usr.mpv_index);i++)
		{
			if(max_mp_tmp < vmanetic_usr.Mpv[j][i] )
				max_mp_tmp = vmanetic_usr.Mpv[j][i];
			else
				max_mp_tmp = max_mp_tmp;	
			
			if(max_mv_tmp>vmanetic_usr.Mpv[j][i] )
				max_mv_tmp = vmanetic_usr.Mpv[j][i];
			else
				max_mv_tmp = max_mp_tmp;	
		}
	}

	
}
void Mpv_Delete(unsigned char delete_value_index,unsigned char current_index)
{
	 unsigned char j,n;
	for(j=0;j<3;j++)
	{
		memcpy(&vmanetic_usr.Mpv[j][delete_value_index],&vmanetic_usr.Mpv[j][delete_value_index+2],
							current_index-delete_value_index-1);
		
		memcpy(&vmanetic_usr.TI[j][delete_value_index],&vmanetic_usr.TI[j][delete_value_index+2],
							current_index-delete_value_index-1);	
	}  

	vmanetic_usr.mpv_index = vmanetic_usr.mpv_index - 2;
}
unsigned char Decision_Logic()
{
  unsigned char i,j,n;
  unsigned char currentMax_index;
	float ti_final,ti_first,t11,t22;

	 max_mpv(max_mp,min_mv);
   currentMax_index = vmanetic_usr.mpv_index;
		
	if( currentMax_index < 2*BUFFERSIZE)// mpv aquisition
	{ 
		for(j=0;j<3;j++)
		{
			for(i=0;i<(currentMax_index-1);i++)
			{
				t11 = (vmanetic_usr.Mpv[j][i]);
				t22 = (vmanetic_usr.Mpv[j][i+1]);	
				e2[i] = fabs(t11-t22);
				if(e2[i]<((max_mp-min_mv)*0.1))
					Mpv_Delete(i,currentMax_index);//delete illegal value
			}		
		}
	}
	else  //mpv aquisition equal to max size
	{
		for(j=0;j<3;j++)
		{
			if( (max_mp-min_mv)>MAX_TD)
			{    
				 ti_final = vmanetic_usr.TI[j][vmanetic_usr.mpv_index-1];
				 ti_first = vmanetic_usr.TI[j][0];
				 if((ti_final - ti_first)>(vmanetic_usr.Tse[2]-vmanetic_usr.Tse[1])*0.5)
					vmanetic_usr.car_present = 1;//car is coming
				 else
					vmanetic_usr.car_present = 0;// no car
			}
			else 
					vmanetic_usr.car_present = 0;			
		}
	}   

	
                               n=0;
                              printf("%f    %f    %f    %f    %f     %f    %f    %f     %f ",
				data2[0],data2[1],data2[2],
				vmanetic_usr.B[0][1],
				vmanetic_usr.B[1][1],
				vmanetic_usr.B[2][1],
				//n,n,n
				vmanetic_usr.Mpv[0][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Mpv[1][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Mpv[2][vmanetic_usr.mpv_index-1]
				);

	                      printf("    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f    %f ",
				vmanetic_usr.TI[0][vmanetic_usr.mpv_index-1],
				vmanetic_usr.TI[1][vmanetic_usr.mpv_index-1],
				vmanetic_usr.TI[2][vmanetic_usr.mpv_index-1],
				vmanetic_usr.Tse[0],
				vmanetic_usr.Tse[1],
				vmanetic_usr.Tse[2],
				e1[0],e1[1],e1[2],
				e2[0],e2[1],e2[2]
				); 	
			printf("   %f \n",data2[3]);
	if(vmanetic_usr.mpv_index >= 2*BUFFERSIZE)
	{
		vmanetic_usr.mpv_index = 0;
	}			
       return 1;
}
