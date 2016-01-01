/*
 * serial_legency.c
 *
 * Created: 3/28/2015 12:09:46 PM
 *  Author: Ashish kushwaha
 */ 

#define F_CPU 11059200

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "USART.h"

#define op		PORTA
#define bulb	0
#define fan		1
#define tv		2
#define AC		3

typedef struct  
{
	char *dis;
}states;

char ret_res(char*str ){
	if(!strcmp(str,"fan")) return 2;
	else if(!strcmp(str,"aco")) return 4;
	else if(!strcmp(str,"mc")) return 0;
	else if(!strcmp(str,"mac")) return 0;
	else if(!strcmp(str,"tv")) return 3;
	else if(!strcmp(str,"bulb1")) return 1;
	else if(!strcmp(str,"bulb2")) return 0;
	else if(!strcmp(str,"bulb3")) return 0;
	else if(!strcmp(str,"moff")) return 0;
	else return 0;
}
void set_op_value(char cas){
	switch(cas){
		case 1 :op^=(1<<(cas-1));break;
		case 2 :op^=(1<<(cas-1));break;
		case 3 :op^=(1<<(cas-1));break;
		case 4 :op^=(1<<(cas-1));break;
		default:op=0; 
	}
}

void display(void){
	states two[]={
		{"OFF"},
		{"ON "}
	};
	lcd_clrscr();
	lcd_puts("BULB=");
	lcd_puts(two[(op&(1<<bulb))].dis);
	lcd_puts(" FAN=");
	lcd_puts(two[((op&(1<<fan))>>fan)].dis);
	lcd_puts("\nTV=");
	lcd_puts(two[((op&(1<<tv))>>tv)].dis);
	lcd_puts("   AC=");
	lcd_puts(two[((op&(1<<AC))>>AC)].dis);
}
int main(void){
	DDRA=0xff;
	lcd_init(LCD_DISP_ON);
	lcd_puts("     Welcome");
	_delay_ms(700);
	const static int strSize = 10;				//size for receiving string 
	char * string = 0;							//target string	
	char response=0;							//after compare the string the int would have a value
	serial_init();								//uart at baud 38400
	while(1){
	set_op_value(response);
	display();
	string = malloc( strSize * sizeof(char) );	//allocating space in mamory heap
	if(string == 0) {							//if memory is not available
		lcd_clrscr();
		lcd_puts("can't alloc mem!\n");			
		return 1;
	}
	
	else{													//if memory is free
	response=ret_res(make_str(strSize,string));				//compare the received string to ans 
	free(string);											//free that allocated space
	string = 0;												//not necessary but good idea
	 }
	}
	return 0 ;
}