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

* Duty Setting：
* 			TIM_OCPolarity_High： 
								Duty+ = Compare4/arr*100%
*				TIM_OCPolarity_Low：
								Duty+ = (arr-Compare4)/arr*100%
************************/
void TIMx_PWM_Init(u32 arr, u32 psc)  {  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE); // TIM3时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx | RCC_APB2Periph_AFIO, ENABLE); //使能GPIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = PWML1 | PWML2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr;  		// 自动装载值
	TIM_TimeBaseStructure.TIM_Prescaler = psc ; // 定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  // 实证分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);  

	TIM_OCInitTypeDef TIM_OCInitStructure;  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出比较极性
	//TIM_OC3Init(TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	DelayInit(); 
	TIMx_PWM_Init(ACC,PSC);
	
	//实现比较值从0-300递增，到300后从300-0递减，循环
	while(1){
		DelayMs(10);
		// dir==1 led0pwmval递增
		if(dir)
			pwmVal += 5;
		// dir==0 led0pwmval递减 
		else
			pwmVal -= 5;
		
		if(pwmVal >= 4000) dir = 0;	//led0pwmval到达300后，方向为递减
		if(pwmVal == 0) dir = 1;		//led0pwmval递减到0后，方向改为递增
		TIM_SetCompare4(TIMx,pwmVal);	//修改比较值，修改占空比
	}  
}
