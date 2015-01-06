#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"

extern int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
extern Uint16 number;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������ݺ������Զ���ӦС����λ�ã�,Ŀǰ�Ĳ�����ֻ�ܷ���8���ַ�
void TransData(float32 input) //�������� //������
{
	static Uint32 input_int=0,bit_dot=0,bit_max=0;
	static int16 j;
	static Uint16 data_bit[10];

	bit_dot=0;
	bit_max=0;
	j=0;
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
		while((input<665350)&&(bit_max+bit_dot)<6)  //��314.159��Ϊ3141.59��Ϊ31415.9��Ϊ314159��Ϊ3141590������Χ������
		{
			input=input*10;
			bit_max++;
		}
		input_int=input;
		j=bit_max+bit_dot+1; //����ת�� ��314.159תΪ314[��]159[/n] bit_dot=3 bit_max=3
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
			sci_send(data_bit[j]);
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
			while((input<665350)&&(bit_max+bit_dot)<5)  //��314.159��Ϊ3141.59��Ϊ31415.9��Ϊ314159��Ϊ3141590������Χ������
			{
				input=input*10;
				bit_max++;
			}
			input_int=input;
			j=bit_max+bit_dot+2; //����ת�� ��314.159תΪ314[��]159[/n] bit_dot=3 bit_max=6
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
//ϵͳ���ұ�������
void SelfProtect(void)
{
	EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
	EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
	while(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ú���
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


