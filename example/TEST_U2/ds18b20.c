#include "ds18b20.h"

static unsigned char rom[8];

//
void OneWireDelay(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		OneWireDelay(5);
		DQ = 1;
		dat >>= 1;
	}
	OneWireDelay(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		OneWireDelay(5);
	}
	return dat;
}

//
bit InitDS18B20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	OneWireDelay(12);
  	DQ = 0;
  	OneWireDelay(80); 
  	DQ = 1;
  	OneWireDelay(10);  
  	initflag = DQ; 
  	OneWireDelay(5);
  
  	return initflag;
}


//
float GetTemperature(void)
{
    unsigned int temp;
	float temperature;
    unsigned char low,high;
  	InitDS18B20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0x44); 
	
	/* update:2022??11??3??
	   ????DS18B20????????????????? 
	*/
  	OneWireDelay(700);
  	InitDS18B20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0xBE); 

  	low = Read_DS18B20(); 
  	high = Read_DS18B20();

	temp = (high&0x0f);
	temp <<= 8;
	temp |= low;
	temperature = temp*0.0625;
  	return temperature;
}


