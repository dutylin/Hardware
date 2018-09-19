/*!
*******************************************************************************

*******************************************************************************
*/

#ifndef __IAR_OPTIMIZE_H__
#define __IAR_OPTIMIZE_H__


#define OPTI_NO_CSE           _Pragma("optimize=no_cse")
#define OPTI_NO_CODE_MOTION   _Pragma("optimize=no_code_motion")
#define OPTI_NO_INLINE        _Pragma("optimize=no_inline")
#define OPTI_NO_TBAA          _Pragma("optimize=no_tbaa") 

#define OPTI_S                _Pragma("optimize=s")
#define OPTI_Z                _Pragma("optimize=z")

#define OPTI_NONE             _Pragma("optimize=none")
#define OPTI_LOW              _Pragma("optimize=low")
#define OPTI_MEDIUM           _Pragma("optimize=medium")
#define OPTI_HIGH             _Pragma("optimize=high")

#if DEBUG_OPTI_EN==1
#define DEBUG_OPTI OPTI_LOW
#else 
#define DEBUG_OPTI
#endif

#endif
