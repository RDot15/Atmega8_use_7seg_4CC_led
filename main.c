/*
 * GccApplication1.c
 *
 * Created: 24.07.2024 22:39:48
 * Author : Dmitry
 */ 
// LIGHT LED


# define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int a_1000 = 0;
int a_100 = 0;
int a_10 = 0;
int a1_1 = 0;

unsigned int count = 0;
unsigned int seconds = 0;
// 
//  int arr[10] =
//  {
// 	 0b00111111, // 0
// 	 0b00000110, // 1
// 	 0b01011011, // 2
// 	 0b01001111, // 3
// 	 0b01100110, // 4
// 	 0b01101101, // 5
// 	 0b01111101, // 6
// 	 0b00000111, // 7
// 	 0b01111111, // 8
// 	 0b01101111  // 9
// 	 
//  };
// 
void digit_func (unsigned int digit)
{
	a_1000 = digit/1000;   //тысячи
	a_100 = digit%1000/100; // сотни
	a_10 = digit%100/10; //десятки
	a1_1 = digit%10; //единицы
	
}

void digit_switch (unsigned int value)
{
	switch (value)
	{
		case 0:
			PORTD &= ~((1<<3) | (1<<2));
			PORTB |=(1<<0) | (1<<1) | (1<<2) | (1<<3);
			PORTD |= (1<<0) | (1<<1);
			break;
			
		case 1: 
			PORTB &= ~((1<<3) | (1<<0));
			PORTD &= ~((1<<3) | (1<<2) | (1<<1) | (1<<0));  
			PORTB |= (1<<2) | (1<<1);
			break;
			
		case 2:
			PORTD |= (1<<0) | (1<<2);
			PORTB |= (1<<0) | (1<<1) | (1<<3);
			PORTD &= ~(1<<1);
			PORTB &= ~(1<<2);
			break;
		
		case 3:
			PORTB |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
			PORTD |= (1<<2);
			PORTD &= ~((1<<0) | (1<<1));
			break;
	
		case 4:
			PORTD |= (1<<1) | (1<<2);
			PORTB |= (1<<1) | (1<<2);
			PORTB &= ~((1<<0) | (1<<3));
			PORTD &= ~(1<<0);
			break;
			
		case 5:
			PORTD |= (1<<1) | (1<<2);
			PORTB |= (1<<0) | (1<<2) | (1<<3);	
			PORTD &= ~(1<<0);
			PORTB &= ~(1<<1);
			break;
			
		case 6:
			PORTD |= (1<<0) | (1<<1) | (1<<2);
			PORTB |= (1<<0) | (1<<2) | (1<<3);
			PORTB &= ~(1<<1);	
			break;
			
		case 7:
			PORTB |= (1<<0) | (1<<1) | (1<<2);
			PORTB &= ~(1<<3);
			PORTD &= ~((1<<0) | (1<<1) | (1<<2));
			break;
		
		case 8:
			PORTD |= (1<<0) | (1<<1) | (1<<2);
			PORTB |= (1<<0) | (1<<1) | (1<<2) | (1<<3);	
			break;
			
		case 9:
			PORTB |= (1<<0) | (1<<1) | (1<<3);	
			PORTD |= (1<<1) | (1<<2);
			PORTD &= ~(1<<0);
			break;
							
	}
		
}


ISR(TIMER0_OVF_vect) // макрос прерывания . Формирование разрядов
{
count ++;
if (count > 4)
	count = 1;	 
	
	if (count == 1)
	{
		PORTD |= (1<<4) | (1<<5);
		PORTB |= (1<<5);
		PORTB &= ~(1<<4);
				
		digit_switch(a_1000);
	} 
	
	else if (count == 2)
	{
		PORTD |= (1<<4) | (1<<5);
		PORTB |= (1<<4); 
		PORTB &= ~(1<<5); 
		
		digit_switch(a_100);
	}
	
	else if (count == 3)
	{ 
		PORTB |= (1<<4) | (1<<5);
		PORTD |= (1<<5); 
		PORTD &= ~(1<<4);
		
		digit_switch(a_10);
		
	}
	
	else if (count == 4) 
	{
		PORTB |= (1<<4) | (1<<5);
		PORTD |= (1<<4); 
		PORTD &= ~(1<<5); 
						
		digit_switch(a1_1);
		
	}
	
}


int main(void)
{
	
	// Сегменты a, b, c, d	
	DDRB |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
	PORTB &= ~((1<<3) | (1<<2) | (1<<1) | (1<<0));
	
	// Сегменты e, f, g, h
	DDRD |= (1<<3) | (1<<2) | (1<<1) | (1<<0);
	PORTD &= ~((1<<3) | (1<<2) | (1<<1) | (1<<0));
	
	// Разряды 1, 2
	DDRB |= (1<<4) | (1<<5);
	PORTB |= (1<<4) | (1<<5);
	
	// Разряды 3, 4
	DDRD |= (1<<4) | (1<<5);
	PORTD |= (1<<4) | (1<<5);
	
		
	 TCCR0 = 0b00000010; // делитель на 8
     TCNT0 = 0;
     TIMSK = 0b00000001; // разрешаем прерывание при переполнении 0-го таймера счетчика  
	       // с таким делителем он будет срабатывать раз в 0,002сек. 2 милесекунды.
			// Можно еще записать как TIMSK |= (1<<TOIE0);
	
      sei(); // прерывание. главная функция по прерыванию
   
    while (1) 
    {
				
		digit_func(1987);
		   
     }
}


