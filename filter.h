#ifndef FILTER_H
#define FILTER_H
#define N		100
typedef struct
{
        unsigned char sample_index;
	short int S[3][N];
	float  A[3][N];	
	float B[3][N];		
}MANETIC_TYPEDEF;
#endif

