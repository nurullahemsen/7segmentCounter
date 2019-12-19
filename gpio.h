#ifndef gpio_h
#define gpio_h

/* It is always best practice to define some macros for the signals*/
#define  SW1  (GPIOF->DATA&(1<<4))
#define	 SW2  (GPIOF->DATA&(1<<0))
#define	 red  (GPIOF->DATA=0x02)
#define	 blue  (GPIOF->DATA=0x04)
#define	 green (GPIOF->DATA=0x08)
#define	 white (GPIOF->DATA=0x0E)
#define	 Dark  (GPIOF->DATA=0x00)
#define	 Yellow (GPIOF->DATA=0x0A)
#define	 Sky_blue (GPIOF->DATA=0x0C)
#define out7_0 0x7E
#define out7_1 0x48
#define out7_2 0x3D
#define out7_3 0x6D
#define out7_4 0x4B
#define out7_5 0x67
#define out7_6 0x77
#define out7_7 0x4C
#define out7_8 0x7F
#define out7_9 0x6F
#endif
