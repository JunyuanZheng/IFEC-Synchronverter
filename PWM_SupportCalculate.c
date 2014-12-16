#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//发送数据函数（自动适应小数点位置）,目前的波特率只能发送8个字符
void TransData(float32 input) //发送数据 //待测试
{
	Uint32 input_int=0,bit_dot=0,bit_max=0;
	int16 j;
	extern Uint16 data_bit[20];

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
		while(input<665350)  //如314.159变为3141.59变为31415.9变为314159变为3141590超出范围，跳出
		{
			input=input*10;
			bit_max++;
		}
		input_int=input/10; //变为314159
		bit_max--;
		j=bit_max+bit_dot+1; //进行转换 如314.159转为314[。]159[/n] bit_dot=3 bit_max=6
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
			SCIASend(data_bit[j]);
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
			while(input<665350)  //如314.159变为3141.59变为31415.9变为314159变为3141590超出范围，跳出
			{
				input=input*10;
				bit_max++;
			}
			input_int=input/10; //变为314159
			bit_max--;
			j=bit_max+bit_dot+1+1; //进行转换 如314.159转为314[。]159[/n] bit_dot=3 bit_max=6
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
				SCIASend(data_bit[j]);
				data_bit[j]=0;
			}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Te，Q滤波器
float32 TeFilter(float32 input,float32 b)
{
	static float32 output_1=0,input_1=0,output=0;
	output=((input+input_1)-(1-2*b/T)*output_1)/(2*b/T+1);
	input_1=input;
	output_1=output;
	return output;
}
float32 QFilter(float32 input,float32 b)
{
	static float32 output_1=0,input_1=0,output=0;
	output=((input+input_1)-(1-2*b/T)*output_1)/(2*b/T+1);
	input_1=input;
	output_1=output;
	return output;
}
/*














*/

//---------------------------------------------------测试用函数-----------------------------------------------------------------


float32 P_Mean(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=200; //可更改
	if(count<n)
	{
		count++;
		sum=sum+input;
	}
	else
	{
		count=0;
		output=sum/n;
		sum=0;
	}
	return output;
}
float32 Vg_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=200; //采样频率1kHz
	if(count<n)
	{
		count++;
		temp=temp+input*input;
	}
	else
	{
		count=0;
		output=sqrt(temp/n);
		temp=0;
	}
	return output;
}
float32 Mean_Q(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=10000; //可更改
	if(count<n)
	{
		count++;
		sum=sum+input;
	}
	else
	{
		count=0;
		output=sum/n;
		sum=0;
	}
	return output;
}
float32 Mean_Te(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=10000; //可更改
	if(count<n)
	{
		count++;
		sum=sum+input;
	}
	else
	{
		count=0;
		output=sum/n;
		sum=0;
	}
	return output;
}



