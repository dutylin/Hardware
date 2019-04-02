#ifndef __STD_INT_H__
#define __STD_INT_H__

#define STD_INT64 1

typedef signed char sint8;
typedef unsigned char uint8;

typedef signed short sint16;
typedef unsigned short uint16;

typedef signed long sint32;
typedef unsigned long uint32;

//typedef signed int sint32;
//typedef unsigned int uint32;

#if STD_INT64==1
typedef signed long long int sint64;
typedef unsigned long long int uint64;
#endif

typedef unsigned int uint8_least;
typedef signed int sint8_least;

typedef unsigned int uint16_least;
typedef signed int sint16_least;

typedef unsigned int uint32_least;
typedef signed int sint32_least;

#define BOOL int

#endif
