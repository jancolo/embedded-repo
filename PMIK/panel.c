#include "panel.h"
#include "ili932x.h"

#define KEY_SIZE_X 40
#define KEY_SIZE_Y 80
#define LCD_MAX_X 240
#define LCD_MAX_Y 320

#define CMD_IN_X 10
#define CMD_IN_Y 10


int viewPanel(int maxx,int maxy){
}
/**Gets key values and makes keytab */

void drawKeyPanel(int keyNum, char * tab)
{
	int i = 0;
	int xLines = keyNum/(LCD_MAX_X/KEY_SIZE_X)+1;
	
	/* Draw lines between keys */
	for (i = xLines; i > 0 ; --i)
	{
		LCD_DrawLine(0, LCD_MAX_Y-i*KEY_SIZE_Y, LCD_MAX_X, LCD_MAX_Y-i*KEY_SIZE_Y);
	}
	for (i = KEY_SIZE_X; i < LCD_MAX_X; i = i + KEY_SIZE_X)
	{
		LCD_DrawLine(i, LCD_MAX_Y-xLines*KEY_SIZE_Y, i, LCD_MAX_Y);
	}
}

void drawCommandIn(char * tab)
{
	WriteString(CMD_IN_X, CMD_IN_Y, tab, BLUE, 1);
}

void drawCommandResponsed()
{
	uint8_t tab[2]={'D','U','P','A'};
	WriteString(10,50, tab, BLACK, 0);
}
