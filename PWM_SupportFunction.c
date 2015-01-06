#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"

extern int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
extern Uint16 number;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//发送数据函数（自动适应小数点位置）,目前的波特率只能发送8个字符
void TransData(float32 input) //发送数据 //待测试
{
	static Uint32 input_int=0,bit_dot=0,bit_max=0;
	static int16 j;
	static Uint16 data_bit[10];

	bit_dot=0;
	bit_max=0;
	j=0;
	if(input>0)
	{
	    //求小数点前有几位
		input_int=input;
		while(input_int>0)
		{
			input_int=input_int/10;
			bit_dot++;
		}
		//数据处理转换（自适应）
		while((input<665350)&&(bit_max+bit_dot)<6)  //如314.159变为3141.59变为31415.9变为314159变为3141590超出范围，跳出
		{
			input=input*10;
			bit_max++;
		}
		input_int=input;
		j=bit_max+bit_dot+1; //进行转换 如314.159转为314[。]159[/n] bit_dot=3 bit_max=3
		data_bit[j]=10; //最后一位换行符  如 第7位为换行
		j--;
		while(j>bit_dot)
		{
			data_bit[j]=input_int%10+48;
			input_int=input_int/10;
			j--;
		}
		data_bit[bit_dot]='.';
		j--;
		while(j>-1)
		{
			data_bit[j]=input_int%10+48;
			input_int=input_int/10;
			j--;
		}
		//发送数据
		for(j=0;j<bit_dot+bit_max+2;j++)
		{
			sci_send(data_bit[j]);
			data_bit[j]=0;
		}
	}
	else
		if(input<0)
		{
			//求小数点前有几位
			input_int=-input;
			while(input_int>0)
			{
				input_int=input_int/10;
				bit_dot++;
			}
			//数据处理转换（自适应）
			input=-input;
			while((input<665350)&&(bit_max+bit_dot)<5)  //如314.159变为3141.59变为31415.9变为314159变为3141590超出范围，跳出
			{
				input=input*10;
				bit_max++;
			}
			input_int=input;
			j=bit_max+bit_dot+2; //进行转换 如314.159转为314[。]159[/n] bit_dot=3 bit_max=6
			data_bit[j]=10; //最后一位换行符  如 第7位为换行
			j--;
			while(j>(bit_dot+1))
			{
				data_bit[j]=input_int%10+48;
				input_int=input_int/10;
				j--;
			}
			data_bit[bit_dot+1]='.';
			j--;
			while(j>0)
			{
				data_bit[j]=input_int%10+48;
				input_int=input_int/10;
				j--;
			}
			data_bit[j]='-';
			//发送数据
			for(j=0;j<bit_dot+bit_max+3;j++)
			{
				sci_send(data_bit[j]);
				data_bit[j]=0;
			}
	}
}

Uint16 TransControl(void)
{
	static Uint16 count=0,cycle=0,output;
	if(count==200)
	{
		count=0;
		cycle++;
	}
	if(cycle==50)
		cycle=0;
	if(cycle%10==0)
		output=1;
	else
		output=0;
	count++;
	return output;
}

/*void TransData(float32 input)
{

	input_forward=input;
	input_back=(input-input_forward)*10000;
	if(input_forward<0)
	{    input_forward=-input_forward;
	     input_forwardH=input_forward/256+128;
	     input_forwardL=input_forward%256;
	     input_backH=input_back/256;
	     input_backL=input_back%256;
	}
	else
	{    input_forwardH=input_forward/256;
         input_forwardL=input_forward%256;
         input_backH=input_back/256;
         input_backL=input_back%256;
	}

}
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//系统自我保护函数
void SelfProtect(void)
{
	EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
	EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
	while(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试用函数
float32 e_RMS(float32 input)
{
	static float sum=0.0,output=0.0;
	static Uint16 count=0;

	if(count<200)
	{
		count++;
		sum=sum+input*input;
	}
	else
	{
		count=0;
		output=sqrt(sum/200);
		sum=0;
	}
	return output;
}

float32 vg_RMS(float32 input)
{
	static float sum=0.0,output=0.0;
	static Uint16 count=0;

	if(count<200)
	{
		count++;
		sum=sum+input*input;
	}
	else
	{
		count=0;
		output=sqrt(sum/200);
		sum=0;
	}
	return output;
}


