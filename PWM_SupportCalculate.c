#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//储存性变量
extern float32 SinTable[grid_freq];
//计算性变量
extern float32 vg;
//控制性变量
extern int16 mode;
//修正性变量
extern float32 DriftRectify_Vg;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//模拟电网
void GridImitate(void)
{
	static Uint16 count=0,cycle=0; //15个点对应2个相位
	static int16 flag=-1;
	static float32 vg_meanmax,vg_meanmin,vg_max[5],vg_min[5],vg_1=0,vg_2=0,vg_3=0;

	if(cycle>54) //24 20
		flag=4;

	switch(flag)
	{
	case -1:
		for(count=0;count<5;count++)
		{
			vg_max[count]=0;
			vg_min[count]=0;
		}
		count=0;
		flag=0;
		break;
	case 0:
		if((0<vg)&&(0<vg_1)&&(vg_1<vg)&&(vg_2<0)&&(vg_3<0)&&(vg_3<vg_2))
		{
			flag=1;
		}
		else
		{
			vg_3=vg_2;
			vg_2=vg_1;
			vg_1=vg;
		}
		break;
	case 1:
		if(cycle<5)
		{
			if(vg>vg_max[cycle])
				vg_max[cycle]=vg;
			if(vg<vg_min[cycle])
				vg_min[cycle]=vg;

			if(count<(grid_freq-1))
			{
				count++;
			}
			else
			{
				count=0;
				cycle++;
			}
		}
		else
		{
			vg_meanmax=(vg_max[0]+vg_max[1]+vg_max[2]+vg_max[3]+vg_max[4])/5;
			vg_meanmin=(vg_min[0]+vg_min[1]+vg_min[2]+vg_min[3]+vg_min[4])/5;
			DriftRectify_Vg=(-vg_meanmin-vg_meanmax)/2;
			vg_1=0;
			vg_2=0;
			vg_3=0;
			flag=2;
		}
		break;
	case 2:
		if((0<vg)&&(0<vg_1)&&(vg_1<vg)&&(vg_2<0)&&(vg_3<0)&&(vg_3<vg_2))
		{
			flag=3;
		}
		else
		{
			vg_3=vg_2;
			vg_2=vg_1;
			vg_1=vg;
		}
		break;
	case 4:
		for(count=0;count<grid_freq;count++)
			SinTable[count]=SinTable[count]/50;
		mode=0;
		break;
	}

	if(flag==3)
	{
		SinTable[count]=SinTable[count]+(vg_1+vg_2)/2/(vg_meanmax+DriftRectify_Vg);
		if(count<(grid_freq-1))
		{
			count++;
			vg_3=vg_2;
			vg_2=vg_1;
			vg_1=vg;
		}
		else
		{
			flag=2;
			vg_1=0;
			vg_2=0;
			vg_3=0;
			count=0;
			cycle++;
		}
	}

}
