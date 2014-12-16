#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������ݺ������Զ���ӦС����λ�ã�,Ŀǰ�Ĳ�����ֻ�ܷ���8���ַ�
void TransData(float32 input) //�������� //������
{
	Uint32 input_int=0,bit_dot=0,bit_max=0;
	int16 j;
	extern Uint16 data_bit[20];

	if(input>0)
	{
	    //��С����ǰ�м�λ
		input_int=input;
		while(input_int>0)
		{
				input_int=input_int/10;
				bit_dot++;

		}
		//���ݴ���ת��������Ӧ��
		while(input<665350)  //��314.159��Ϊ3141.59��Ϊ31415.9��Ϊ314159��Ϊ3141590������Χ������
		{
			input=input*10;
			bit_max++;
		}
		input_int=input/10; //��Ϊ314159
		bit_max--;
		j=bit_max+bit_dot+1; //����ת�� ��314.159תΪ314[��]159[/n] bit_dot=3 bit_max=6
		data_bit[j]=10; //���һλ���з�  �� ��7λΪ����
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
		//��������
		for(j=0;j<bit_dot+bit_max+2;j++)
		{
			SCIASend(data_bit[j]);
			data_bit[j]=0;
		}
	}
	else
		if(input<0)
		{
			//��С����ǰ�м�λ
			input_int=-input;
			while(input_int>0)
			{
				input_int=input_int/10;
				bit_dot++;
			}
			//���ݴ���ת��������Ӧ��
			input=-input;
			while(input<665350)  //��314.159��Ϊ3141.59��Ϊ31415.9��Ϊ314159��Ϊ3141590������Χ������
			{
				input=input*10;
				bit_max++;
			}
			input_int=input/10; //��Ϊ314159
			bit_max--;
			j=bit_max+bit_dot+1+1; //����ת�� ��314.159תΪ314[��]159[/n] bit_dot=3 bit_max=6
			data_bit[j]=10; //���һλ���з�  �� ��7λΪ����
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
			//��������
			for(j=0;j<bit_dot+bit_max+3;j++)
			{
				SCIASend(data_bit[j]);
				data_bit[j]=0;
			}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Te��Q�˲���
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

//---------------------------------------------------�����ú���-----------------------------------------------------------------


float32 P_Mean(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=200; //�ɸ���
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
	static Uint16 count=0,n=200; //����Ƶ��1kHz
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
	static Uint16 count=0,n=10000; //�ɸ���
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
	static Uint16 count=0,n=10000; //�ɸ���
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



