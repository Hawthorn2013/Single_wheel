#include "includes.h"

BYTE count=0;

void main(void)
{
	
	for(;;)
	{
		if(g_Control)
		{
			g_Control=0;
			count++;
			angle_read(AngleResult);
			angle_calculate();
			if(count==4)
			{
				
			}
			else if(count==5)
			{
				count=0;
			}
		}
	}
}

