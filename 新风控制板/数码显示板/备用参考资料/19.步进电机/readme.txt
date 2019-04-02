1设计要求
控制实验平台的发光二极管LED1、LED2、LED3、LED4，使它们有规律的点亮，具体顺序如下：
LED1亮->LED2亮->LED3亮->LED4亮，如此反复，当按下开发板上的B3按钮时，灯全部熄灭，
2秒钟后发光二极管全部点亮，再过一秒钟后返回被中断前的状态，有规律的依次点亮。

2 硬件电路设计
在评估板上，LED1、LED2、LED3、LED4分别与PC6、PC7、PC8、PC9相连，按键Key与PB9相连。

3软件程序设计
根据任务要求，程序内容主要包括：
(1)	配置PB口第9个引脚作为外部中断，下降延触发； 
(2)	读取端口数据输出寄存器GPIOC_ODR[15:0]的值，因为C口[9:6]位和四个LED灯连通。   
(3)	初始化时，LED依次点亮，当按下Key键时产生中断；
(4)	中断服务子程序内容：灯全部熄灭，2秒钟后发光二极管全部点亮，再过1秒钟后中断返回。
整个工程包含3个源文件：STM32F10x.s、stm32f10x_it.c和main.c，其中STM32F10x.s为启动代码，
所有中断服务子程序均在stm32f10x_it.c中，其它函数则在main.c中。下面分别介绍相关的函数，
具体程序清单见参考程序。
GPIO_Configuration函数负责配置GPIO端口，其中GPIO_InitStructure数据结构包含所有GPIO端口
配置所需各项，函数GPIO_Init则实现某个端口的配置。在STM32F10xR.LIB和STM3210xD.LIB中均已
定义，读者可通过查阅5.3节所述的ST公司发布的《ARM?-based 32-bit MCU STM32F101xx and 
STM32F103xx Firmware Library》手册来了解该数据结构以及相关驱动函数。
NVIC_Configuration函数用于配置嵌入式中断控制器，相关内容将在后面章节介绍，此处需要将PB9
引脚配置为外部中断。
EXTI9_5_IRQHandler函数是PB9的中断服务子程序，当按下Key按钮之后触发EXTI9_5中断，在中断服
务子程序中先将灯全部熄灭，延迟2秒后全部点亮，再延迟1秒之后退出中断服务子程序。
SysTick_Configuration函数用于配置和允许系统时钟中断，系统时钟中断服务子程序SysTickHandler
函数则用于产生1毫秒的延时，这样Delay函数就可以通过开关系统时钟计数器来实现精确延时了。

4 运行过程
(1)	使用Keil uVision3 通过ULINK 2仿真器连接实验板，打开实验例程目录GPIO_TEST子目录下的
GPIO.Uv2例程，编译链接工程；
(2)	选择软件调试模式，点击MDK 的Debug菜单，选择Start/Stop Debug Session项或Ctrl+F5键，
在逻辑分析仪中添加GPIOC_ODR.6、GPIOC_ODR.7、GPIOC_ODR.8、GPIOC_ODR.9，点击Run按钮即可在
逻辑分析仪中看到如图7-11；
(3)	选择硬件调试模式，选择Start/Stop Debug Session项或Ctrl+F5键，下载程序并运行，观察LED
灯的变化情况；当程序运行在while循环体内时，按Key键，程序进入中断服务子程序EXTI9_5_IRQHandler()，
单步运行，观察LED的变化情况。 
(4)	退出Debug模式，打开Flash菜单>Download，将程序下载到开发板的Flash中，按RESET键复位，
观察LED灯的情况，正常情况应如表7-6所列。

LED灯状态	说    明
LED1	LED2	LED3	LED4	
亮		灭		灭		灭	    程序正常运行，发光二极管依次点亮
灭		亮		灭		灭	
灭		灭		亮		灭	
灭		灭		灭		亮	
灭		灭		灭		灭	      外部信号输入，发生中断，执行中断处理程序
亮		亮		亮		亮	
