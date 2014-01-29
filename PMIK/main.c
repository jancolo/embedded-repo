#include <stdio.h>
#include <math.h>

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "ili932x.h"
#include "touch.h"
#include "hardware.h"

#include "panel.h"

__IO uint32_t TimeFromRTC = 0;

#define LCD_MAX_X 240
#define LCD_MAX_Y 320
#define RECT_SIZE 30
#define MOVE_STEP 5

#define KEY_TAB_SIZE 4

uint8_t keyTable[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
uint8_t AT_command[2] = { 'A', 'T'};
int keyNum = 0;

void NVIC_Configuration_TouchScreenPen(void);

void NVIC_Configuration_RTC(void);
void RTC_Configuration(void);

int Check_Rect_Pos_X(int posx, int currFlag);
int Check_Rect_Pos_Y(int posy, int currFlag);
int Check_Touch_Pos_X(int posx);
int Check_Touch_Pos_Y(int posy);

void Write_Rect(int posx, int posy, int color);

void Draw_Key_Panel();

int main(void) {
	
	/* Current rect position */
	int position_X = 0;
	int position_Y = 0;
	
	/* Flags that determines move directions */
	int dirFlagX = 0;
	int dirFlagY = 0;
	
	int rectSize = RECT_SIZE;
	
	int i = 0;
	
	int offset = 10;

	SystemInit();	

	/* RTC configuration*/
	NVIC_Configuration_RTC();
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* RTC Configuration */
		RTC_Configuration();

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	} else {

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	
	/* SysTick freq f = 72MHz/8 = 9MHz */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	/* Count from 9MHz/1000 (1ms) */ 
	SysTick_Config(9000);

	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_None);

	/* Clear reset flags */
	RCC_ClearFlag();

	/* NVIC to enable interrupt on screen touch */
	NVIC_Configuration_TouchScreenPen();

	/* touch screen */
	Touch_Init();

	/* init LCD */
	LCD_Init();
	LCD_Clear(WHITE);
	
	//Draw_Key_Panel();
	drawKeyPanel(16, keyTable);
	drawCommandIn(AT_command);

	/* Infinite loop */
	while (1)
	{

	    if (TimeFromRTC == 1)
	    {

	    	//LCD_Fill(position_X-RECT_SIZE/2,position_Y-RECT_SIZE/2,position_X+RECT_SIZE/2,position_Y+RECT_SIZE/2,BLACK);
	    	//Write_Rect(position_X, position_Y, BLACK);   	
			
	    	if (dirFlagX == 0 && dirFlagY == 0) {
				position_X = position_X + MOVE_STEP;
				position_Y = position_Y + MOVE_STEP;
			} 
			else if (dirFlagX == 1 && dirFlagY == 0) {
				position_X = position_X - MOVE_STEP;
				position_Y = position_Y + MOVE_STEP;
			}
			else if (dirFlagX == 1 && dirFlagY == 1) {
				position_X = position_X - MOVE_STEP;
				position_Y = position_Y - MOVE_STEP;
			}
			else if (dirFlagX == 0 && dirFlagY == 1) {
				position_X = position_X + MOVE_STEP;
				position_Y = position_Y - MOVE_STEP;
			}
			
			//position_X = Check_Touch_Pos_X(position_X);
			//position_Y = Check_Touch_Pos_Y(position_Y);
			
			//Write_Rect(position_X, position_Y, YELLOW);   	
			//LCD_Fill(position_X-RECT_SIZE/2,position_Y-RECT_SIZE/2,position_X+RECT_SIZE/2,position_Y+RECT_SIZE/2,YELLOW);
			
			dirFlagX = Check_Rect_Pos_X(position_X, dirFlagX);
			dirFlagY = Check_Rect_Pos_Y(position_Y, dirFlagY);					

			TimeFromRTC = 0;
	    }
	    		/* If screen have been touched */
		if(Pen_Point.Key_Sta==Key_Down)
		{
			Pen_Int_Set(0); 
			do
			{
				/* Get new touch positon and change rectangle pos */
				Convert_Pos();
				Pen_Point.Key_Sta=Key_Up;
				//LCD_Fill(position_X-RECT_SIZE/2,position_Y-RECT_SIZE/2,position_X+RECT_SIZE/2,position_Y+RECT_SIZE/2,BLACK);
		        position_X = Pen_Point.X0;
		        //position_X = Check_Touch_Pos_X(Pen_Point.X0);
		        position_Y = Pen_Point.Y0;
		        //position_Y = Check_Touch_Pos_Y(Pen_Point.Y0);
		        
				/* Whet key panel was touched */
				if (position_X > 0 && position_X < LCD_MAX_X && position_Y > (LCD_MAX_Y - LCD_MAX_X) && position_Y < LCD_MAX_Y) {
					
					/* Clearing shown characters when display is overloaded*/
					if (offset > (LCD_MAX_X)) {
						offset = 10;
						LCD_Fill(0, 0, LCD_MAX_X, LCD_MAX_Y-LCD_MAX_X-1, WHITE);
					}
					
					Show_Numbers(position_X, position_Y, offset);
					offset = offset + 30;
					/* Delay for prevent few touches */
					Delay(3000);					
				}
			}while(PEN==0);
			
			Pen_Int_Set(1); // turn interrupt back on
		}

	}
}
/* Shows character on display that was touched */
void Show_Numbers(int x, int y, int offset)
{
	keyNum = ((y - (LCD_MAX_Y - LCD_MAX_X))/(LCD_MAX_X/KEY_TAB_SIZE))*KEY_TAB_SIZE + (x/(LCD_MAX_X/KEY_TAB_SIZE));
	//keyNum = ((y-80)/60)*4 + x/60;
	if (keyNum >= 0 && keyNum < KEY_TAB_SIZE*KEY_TAB_SIZE) LCD_ShowCharBig(offset,10,keyTable[keyNum],3,1);
	else LCD_ShowCharBig(offset,10,'X',3,1);
}

void Draw_Key_Panel()
{
	int i = 0;
	int j = 0;
	int offset = 10;
	int cell_size = LCD_MAX_X/4;
	
	/* Draw lines between keys */
	for (i = LCD_MAX_X; i > 0 ; i = i - LCD_MAX_X/4)
	{
		LCD_DrawLine(0, LCD_MAX_Y-i, LCD_MAX_X, LCD_MAX_Y-i);
	}
	for (i = LCD_MAX_X/4; i < LCD_MAX_X; i = i + LCD_MAX_X/4)
	{
		LCD_DrawLine(i, LCD_MAX_Y-LCD_MAX_X, i, LCD_MAX_Y);
	}
	
	for (i = KEY_TAB_SIZE-1; i >= 0; --i) {
		for (j = KEY_TAB_SIZE-1; j >= 0; --j) {
			LCD_ShowCharBig(offset + cell_size*i, 90 + cell_size*j, keyTable[i+j*4],3,1);
		}
	}
		
	/*
	LCD_ShowNumBig(offset,90,0,1,3);
	LCD_ShowNumBig(offset,90 + cell_size,4,1,3);
	LCD_ShowNumBig(offset,90 + (2*cell_size),8,1,3);
	LCD_ShowCharBig(offset,90 + (3*cell_size),'C',3,1);
	
	LCD_ShowNumBig(offset + cell_size,90,1,1,3);
	LCD_ShowNumBig(offset + cell_size,90 + cell_size,5,1,3);
	LCD_ShowNumBig(offset + cell_size,90 + (2*cell_size),9,1,3);
	LCD_ShowCharBig(offset + cell_size,90 + (3*cell_size),'D',3,1);
	
	LCD_ShowNumBig(offset + (2*cell_size),90,2,1,3);
	LCD_ShowNumBig(offset + (2*cell_size),90 + cell_size,6,1,3);
	LCD_ShowCharBig(offset + (2*cell_size),90 + (2*cell_size),'A',3,1);
	LCD_ShowCharBig(offset + (2*cell_size),90 + (3*cell_size),'E',3,1);
	
	LCD_ShowNumBig(offset + (3*cell_size),90,3,1,3);
	LCD_ShowNumBig(offset + (3*cell_size),90 + cell_size,7,1,3);
	LCD_ShowCharBig(offset + (3*cell_size),90 + (2*cell_size),'B',3,1);
	LCD_ShowCharBig(offset + (3*cell_size),90 + (3*cell_size),'F',3,1);
	*/ 
}
	
int Check_Rect_Pos_X(int posx, int currFlag)
{
	if (posx <= RECT_SIZE/2) return 0;
	else if (posx > LCD_MAX_X - RECT_SIZE/2) return 1;
	else return currFlag;
}

int Check_Rect_Pos_Y(int posy, int currFlag)
{
	if (posy <= RECT_SIZE/2) return 0;
	else if (posy > LCD_MAX_Y - RECT_SIZE/2) return 1;
	else return currFlag;
}		

int Check_Touch_Pos_X(int posx)
{
	if (posx <= RECT_SIZE/2) return RECT_SIZE/2;
	else if (posx >= LCD_MAX_X - RECT_SIZE/2) return (LCD_MAX_X - RECT_SIZE/2);
	else return posx;
}

int Check_Touch_Pos_Y(int posy)
{
	if (posy <= RECT_SIZE/2) return RECT_SIZE/2;
	else if (posy >= LCD_MAX_Y - RECT_SIZE/2) return (LCD_MAX_Y - RECT_SIZE/2);
	else return posy;
}

void Write_Rect(int posx, int posy, int color)
{
	if (posx-RECT_SIZE/2 < 0 && posy-RECT_SIZE/2 < 0) {
		LCD_Fill(0,0,posx+RECT_SIZE/2,posy+RECT_SIZE/2,color);
	}
	else if (posx-RECT_SIZE/2 < 0) {
		LCD_Fill(0,posy-RECT_SIZE/2,posx+RECT_SIZE/2,posy+RECT_SIZE/2,color);
	}
	else if (posy-RECT_SIZE/2 < 0) {
		LCD_Fill(posx-RECT_SIZE/2,0,posx+RECT_SIZE/2,posy+RECT_SIZE/2,color);
	}
	else LCD_Fill(posx-RECT_SIZE/2,posy-RECT_SIZE/2,posx+RECT_SIZE/2,posy+RECT_SIZE/2,color);	
}

void NVIC_Configuration_RTC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* page: 60 */
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767/2); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/* page: 92 */
void NVIC_Configuration_TouchScreenPen(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the EXTI15_10_IRQn Interrupt
	 * This will fire when the touch screen pen is down
	 * */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



