
/*
 * USART.c
 *
 * Created: 3/29/2015 2:28:20 AM
 *  Author: Ashish kushwaha
 * Email: Ashish_kushwaha@outlook.com
 */ 


#include <avr/io.h>
#include "USART.h"



void serial_init(){
		UCSRB=(1<<TXEN)|(1<<RXEN);		
		UCSRC=(1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
		UBRRL=17;                                  ///38400   baudrate at xtal 11.0592Mhz	
}
//This function sends the string and terminates with LF \n
void serial_send(char *p)
{
	serial_Erase_all();
	for (;*p!=0;p++)
	{
		while (!(UCSRA&(1<<UDRE)));
		UDR=*p;
	}
	while (!(UCSRA&(1<<UDRE)));
	UDR=10;
}

void serial_send_byte(char byte){
	serial_Erase_all();
	while (!(UCSRA&(1<<UDRE)));
	UDR=byte;
}

void serial_Erase_all(void){
	unsigned char trash=0;
	while (UCSRA & (1<<RXC)){ 
	trash=UDR;
	}
}

unsigned char serial_read_byte(void)
{
	while (!(UCSRA&(1<<RXC)));
	return UDR;
}



int serial_read_int(void){
	int getval=0,ret=0,flag=0;
	while(getval!='\n'){
	getval=serial_read_byte();
	if (getval=='-')
	{
		flag=1;
	}
	if ((getval>='0')&&(getval<='9'))
	{
		ret=((ret*10)+(getval-48));
	}
	}
	if (flag==1)ret=(ret*(-1));
	return ret;	
}

char *make_str(uint8_t size,char *target_loc){
	unsigned char i=1,dummy=0;
	do 
	{
		dummy=serial_read_byte();
	} while ((dummy<33)||(dummy>126));
	target_loc[0]=dummy;
	do{
		target_loc[i]=serial_read_byte();
		++i;
	} while ((i!=size)&&(target_loc[i-1]!='\n'));
	if (target_loc[i-1]=='\n'){
		target_loc[i-1]='\0';
		serial_Erase_all();
		return target_loc;
	}
	else {
		free(target_loc);
		serial_Erase_all();
		return "less memory error";
	}
}