#ifndef FILTER_H
#define FILTER_H
#define N		    20
#define BUFFERSIZE	60
#define MAX_THRES	0.5
#define alpha		0.05
typedef struct
{
    unsigned char sample_index;
	float S[3][BUFFERSIZE];
	float A[3][BUFFERSIZE];	
	float B[3][BUFFERSIZE];		
}MANETIC_TYPEDEF;
void manetic_filter(float *data);
#endif

