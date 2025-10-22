#include "iic.h"

void delay_5us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 11;
	while (--i);
}


void I2C_init()	
{
	sda = 1;
	_nop_();
	scl = 1;
	_nop_();
}

/*I2C��ʼ�ź�*/
void I2C_Start()  
{
	scl = 1;
	_nop_();
	sda = 1;
	delay_5us();
	sda = 0;
	delay_5us();
}

/*I2C��ֹ�ź�*/
void I2C_Stop()
{
	sda = 0;
	_nop_();
	scl = 1;
	delay_5us();
	sda = 1;
	delay_5us();
}

/*��������Ӧ��*/
void Master_ACK(bit i)		
{
	scl = 0; // ����ʱ����������sda���������ϵ����ݱ仯
	_nop_(); // �������ȶ�
	if (i)	 //���i = 1 ��ô������������ ��ʾ����Ӧ��
	{
		sda = 0;
	}
	else	 
	{
		sda = 1;	 //���ͷ�Ӧ��
	}
	_nop_();//�������ȶ�
	scl = 1;//����ʱ������ �ôӻ���sda���϶��� ������Ӧ���ź�
	delay_5us();
	scl = 0;//����ʱ�����ߣ� ռ�����߼���ͨ��
	_nop_();
	sda = 1;//�ͷ�sda�������ߡ�
	_nop_();
}

/*���ӻ�Ӧ��*/
bit Test_ACK()
{
	scl = 1;
	delay_5us();
	if (sda)
	{
		scl = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else
	{
		scl = 0;
		_nop_();
		return(1);
	}
}

/*����һ���ֽ�*/
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i = 0 ; i < 8 ; i++)
	{
		scl = 0;
		_nop_();
		if (byte & 0x80)
		{				
			sda = 1;	
			_nop_();				   
		}				
		else
		{
			sda = 0;
			_nop_();
		}
		scl = 1;
		_nop_();
		byte <<= 1;	// 0101 0100B 
	}
	scl = 0;
	_nop_();
	sda = 1;
	_nop_();
}


/*I2C ��һ�ֽ�*/
uchar I2C_read_byte()
{
	uchar dat,i;
	scl = 0;
	_nop_();
	sda = 1;
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		scl = 1;
		_nop_();
		if (sda)			    
		{
			 dat |= 0x01; //
		}
		else
		{
			dat &=  0xfe;	//1111 1110
		}
		_nop_();
		scl = 0 ;
		_nop_();
		if(i < 7)
		{
			dat = dat << 1;	
		}
	}
	return(dat);
}

/*I2C��������*/
bit I2C_TransmitData(uchar ADDR, DAT)
{
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR); 
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(DAT);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_Stop();
	return(1);	
}

/*I2C��������*/
uchar I2C_ReceiveData(uchar ADDR)
{
	uchar DAT;
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+1);
	if (!Test_ACK())
	{
		return(0);
	}
	DAT = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(DAT);	
}
