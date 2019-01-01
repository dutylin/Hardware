#ifndef FILTER_H
#define FILTER_H
#define N		    20
#define BUFFERSIZE	60
#define MAX_THRES	0.3
#define alpha		0.05
#define INIT		1
#define MAX_TD    0.8
typedef struct
{
  unsigned char mpv_index;
   unsigned char  car_present;
	unsigned char event_update_flag;
  unsigned char sample_index;
	float S[3][BUFFERSIZE];
	float A[3][BUFFERSIZE];	
	float B[3][2];	
	float Mpv[3][2*BUFFERSIZE];	
	float TI[3][2*BUFFERSIZE];	
	float Tse[3];	//   sample tick time: vmanetic_usr.Tse[0]       mpv start time :vmanetic_usr.Tse[1]	
	                     //    mpv end time :vmanetic_usr.Tse[2]	
}MANETIC_TYPEDEF;
unsigned char  manetic_filter(unsigned char state,float *data);
unsigned char Decision_Logic(void);
#endif

