#ifndef FILTER_H
#define FILTER_H
#define N		    20
#define BUFFERSIZE	60
#define MAX_THRES	0.3
#define alpha		0.05
#define INIT		1
typedef struct
{
  unsigned char mp_index;
  unsigned char mv_index;
	unsigned char event_update_flag;
  unsigned char sample_index;
	float S[3][BUFFERSIZE];
	float A[3][BUFFERSIZE];	
	float B[3][2];	
	float Mp[3][BUFFERSIZE];
	float Mv[3][BUFFERSIZE];	
	float Tp[3][BUFFERSIZE];
	float Tv[3][BUFFERSIZE];	
	float Tse[3];	
}MANETIC_TYPEDEF;
unsigned char  manetic_filter(unsigned char state,float *data);
#endif

