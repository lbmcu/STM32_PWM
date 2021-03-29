/*************************************************************************
    > File Name: main.c
    > Author: lbmcu
    > Mail: 1373766226@qq.com 
    > Created Time: Tue 30 Mar 2021 01:43:53 AM CST
    > Function: output any Freqency and Duty(0--100%)
 ************************************************************************/
#include "main.h"

/***********************
* Timing Time(Freqency): arr*psc*1000/72000000
* eg: arr = 4000 psc = 360 4000*360/72000 = 20(ms) = 1/(20/1000)s = 50HZ

* Duty Setting��
* 			TIM_OCPolarity_High�� 
								Duty+ = Compare4/arr*100%
*				TIM_OCPolarity_Low��
								Duty+ = (arr-Compare4)/arr*100%
************************/
void TIMx_PWM_Init(u32 arr, u32 psc)  {  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE); // TIM3ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx | RCC_APB2Periph_AFIO, ENABLE); //ʹ��GPIOʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = PWML1 | PWML2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr;  		// �Զ�װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc ; // ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // ʵ֤�ָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);  

	TIM_OCInitTypeDef TIM_OCInitStructure;  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM����Ƚϼ���
	//TIM_OC3Init(TIMx, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);

	TIM_Cmd(TIMx, ENABLE);  
}

// Setting Duty(0--100%)
void PWM_DutySet(unsigned char duty)
{
	TIM_SetCompare4(TIMx,duty*ACC/100);
}

int main(void){  
	
	unsigned int pwmVal=1000;
	char dir = 1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	DelayInit(); 
	TIMx_PWM_Init(ACC,PSC);
	
	//ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	while(1){
		DelayMs(10);
		// dir==1 led0pwmval����
		if(dir)
			pwmVal += 5;
		// dir==0 led0pwmval�ݼ� 
		else
			pwmVal -= 5;
		
		if(pwmVal >= 4000) dir = 0;	//led0pwmval����300�󣬷���Ϊ�ݼ�
		if(pwmVal == 0) dir = 1;		//led0pwmval�ݼ���0�󣬷����Ϊ����
		TIM_SetCompare4(TIMx,pwmVal);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}  
}
