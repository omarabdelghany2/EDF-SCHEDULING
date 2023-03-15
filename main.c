


#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "serial.h"
#include "Starter_Files_V1/header/GPIO.h"

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainTX_ENABLE		( ( unsigned long ) 0x00010000 )	/* UART1. */
#define mainRX_ENABLE		( ( unsigned long ) 0x00040000 ) 	/* UART1. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )
#define mainLED_TO_OUTPUT	( ( unsigned long ) 0xff0000 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )





char RunTimeStatsBuff[190];
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}


static void prvSetupHardware( void );

void Button_1_Monitor( void *pvParameters );
void Button_2_Monitor( void *pvParameters );
void Periodic_Transmitter( void *pvParameters );
void Uart_Receiver( void *pvParameters );

  void Load_1_Simulation( void *pvParameters );
void Load_2_Simulation( void *pvParameters );
/*-----------------------------------------------------------*/
/////////////////MT TRIALSSSSS




xComPortHandle xPort;
int main( void )
	{
	/* Setup the hardware for use with the Keil demo board. */

	prvSetupHardware();
			 xTaskPeriodicCreate(
                    Button_1_Monitor,         /* Function that implements the task. */
                    "Button_1_Monitor",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										50); 
			 						
			 xTaskPeriodicCreate(
                    Button_2_Monitor,         /* Function that implements the task. */
                    "Button_2_Monitor",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										50); 								
	
			 xTaskPeriodicCreate(
                    Periodic_Transmitter,         /* Function that implements the task. */
                    "Periodic_Transmitter",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										100); 	
			 xTaskPeriodicCreate(
                    Uart_Receiver,         /* Function that implements the task. */
                    "Uart_Receiver",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										20); 	
			 xTaskPeriodicCreate(
                    Load_1_Simulation,         /* Function that implements the task. */
                    "Load_1_Simulation",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										10);
			 xTaskPeriodicCreate(
                    Load_2_Simulation,         /* Function that implements the task. */
                    "Load_2_Simulation",        /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,           /* Stack size in words, not bytes. */
                    ( void * ) NULL,  /* Parameter passed into the task. */
                    1 | portPRIVILEGE_BIT,						 /* Priority at which the task is created. */
                    NULL, /* Used to pass out the created task's handle. */
										100 );	 										


	vTaskStartScheduler();


	for( ;; );
}
/*-----------------------------------------------------------*/








void vApplicationTickHook()
{
	

		GPIO_write(PORT_1,PIN1,PIN_IS_HIGH);
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
			
	
}

void vApplicationIdleHook()
{
GPIO_write(PORT_0,PIN9,PIN_IS_HIGH);	
	
}
/*this variables for calculating the Cpu load*/
float  Task_IN_TIME=0;
float  Task_OUT_TIME=0;
float  Tasks_Total_Time=0;
float   Cpu_load=0;
		
int i=0;
int Button1_OldState=0;
int Button1_State=0;
xComPortHandle pxPort;
 void Button_1_Monitor( void *pvParameters )
 {
	 int i=0;
	 int j=0;
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	 
			


													for( ;; )
													{
														//BUTTON1
														if(	Button1_State==GPIO_read(PORT_0,PIN1)==PIN_IS_HIGH)
														{
															if(Button1_State!=Button1_OldState)
																	{
																					vSerialPutString(xPort , (signed char*)"Rising_Edge_onButton1",21);
																					xSerialPutChar(pxPort,'\n',1);
																		
																	}
														}			
														else
														{
																	if(Button1_State!=Button1_OldState)
																	{
																					vSerialPutString(xPort , (signed char*)"Falling_Edge_onButton1",23);
																					xSerialPutChar(pxPort,'\n',1);
																		
																	}
														}	
															Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 50 );
															Task_IN_TIME=T1TC;
														

													}


 }
 
 

int Button2_OldState=0;
int Button2_State=0;
xComPortHandle pxPort;
 void Button_2_Monitor( void *pvParameters )
 {
	 int i=0;
	 int j=0;
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	 
			


													for( ;; )
													{
														//BUTTON1
														if(	Button2_State==GPIO_read(PORT_0,PIN3)==PIN_IS_HIGH)
														{
															if(Button2_State!=Button2_OldState)
																	{
																					vSerialPutString(xPort , (signed char*)"Rising_Edge_onButton1",21);
																					xSerialPutChar(pxPort,'\n',1);
																		
																	}
														}			
														else
														{
																	if(Button2_State!=Button2_OldState)
																	{
																					vSerialPutString(xPort , (signed char*)"Falling_Edge_onButton1",23);
																					xSerialPutChar(pxPort,'\n',1);
																		
																	}
														}
																	Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 50 );
															Task_IN_TIME=T1TC;
														
													
														
														

													}


 }
 
 
 
 
  void Periodic_Transmitter( void *pvParameters )
 {
	 int i=0;
	 int j=0;
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	 
			


													for( ;; )
													{
						

							
																				vSerialPutString(xPort , (signed char*)"Hey Its aperiodic Message Transmit",100);
																				
																					
															Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 100 );
															Task_IN_TIME=T1TC;
														
	
															
													}

													


 }
 signed char input=0;
   void Uart_Receiver( void *pvParameters )
 {
	 int i=0;
	 int j=0;
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );
	 
			


													for( ;; )
													{
						



																					while((U0LSR&0X02)==0X02) /* Checking for available data*/
																						{
																								xSerialGetChar(NULL,&input,1);	
																								xSerialPutChar(NULL,input,1);
																						

																						}
																					xSerialPutChar(NULL,'\n',1);		
														
															Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 20 );
															Task_IN_TIME=T1TC;
														

													}

													


 }
 
 
    void Load_1_Simulation( void *pvParameters )
 {
	 int i=0;
	 
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
	 
			


													for( ;; )
													{
						



																	for(i=0;i<33500;i++)
																			{
																	
																					i=i;
																			}
																			
																Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 10 );
															Task_IN_TIME=T1TC;
				
															
													}

													


 }
     void Load_2_Simulation( void *pvParameters )
 {
	 int i=0;
	 
	 TickType_t xLastWakeTime;
	 xLastWakeTime = xTaskGetTickCount();
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 6 );
	 
			


													for( ;; )
													{
						



																	for(i=0;i<80400;i++)
																			{
																	
																					i=i;
																			}
																			

															 Task_OUT_TIME=T1TC;
															Tasks_Total_Time+=(Task_OUT_TIME-Task_IN_TIME);
															Cpu_load=((float)Tasks_Total_Time/(float)T1TC)*(float)100;
															vTaskDelayUntil( &xLastWakeTime, 100 );
															Task_IN_TIME=T1TC;
														

													}

													


 }
 
  



static void prvSetupHardware( void )
{

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
	
		xPort = xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE, 30);
		GPIO_init();
	configTimer1();
}
/*-----------------------------------------------------------*/

