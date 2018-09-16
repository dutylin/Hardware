


#ifndef __DATATYPE_H
#define __DATATYPE_H

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  通用宏定义
*********************************************************************************************************/
  /*
#ifndef TRUE
#define TRUE     1ul
#endif                                                               

#ifndef FALSE
#define FALSE    0ul
#endif                                                                 

#ifndef NULL
#define NULL     0ul
#endif 
 */ 
  
#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

/*********************************************************************************************************
  定义与编译器无关的数据类型
*********************************************************************************************************/
#define BOOLEAN unsigned char                                            /*  布尔变量                    */
#define INT8U   unsigned char                                            /*  无符号8位整型变量           */
#define INT8S   signed   char                                            /*  有符号8位整型变量           */
#define INT16U  unsigned int                                             /*  无符号16位整型变量          */
#define INT16S  signed   int                                             /*  有符号16位整型变量          */
#define INT32U  unsigned long                                            /*  无符号32位整型变量          */
#define INT32S  signed   long                                            /*  有符号32位整型变量          */
#define FP32    float                                                    /*  单精度浮点数（32位长度）    */
#define FP64    double                                                   /*  双精度浮点数（64位长度）    */

#define vid     void                                                     /*  空                          */
#define uint32  unsigned long                                            /*  无符号32位整型变量          */
#define int32s  signed   long                                            /*  有符号32位整型变量          */
#define uint16  unsigned int                                             /*  无符号16位整型变量          */
#define uint16s signed   int                                             /*  无符号8位整型变量           */
#define uint8   unsigned char                                            /*  无符号8位整型变量           */
#define uint8s  signed   char                                            /*  无符号8位整型变量           */
#define fp32    float                                                    /*  单精度浮点数（32位长度）    */
#define fp64    double                                                   /*  双精度浮点数（64位长度）    */

/*********************************************************************************************************
  固件提供的功能引入的头文件
*********************************************************************************************************/

/*********************************************************************************************************
  其它需要引入的头文件
*********************************************************************************************************/

#ifdef __cplusplus
}
#endif 


#endif