#include "at24c02.h"

void write_AT24C02(unsigned char addr, unsigned char dat)
{
    // ������ʼ�ź�
    I2C_Start();
    
    // �����豸��ַ����д����λ
    I2C_send_byte(AT24C02_ADDR | 0x00); // AT24C02��д��ַ�ǻ�����ַ����0
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // �����ڴ��ַ
    I2C_send_byte(addr);
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // ����Ҫд�������
    I2C_send_byte(dat);
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // ����ֹͣ�ź�
    I2C_Stop();
    
}
unsigned char read_AT24C02(unsigned char addr)
{
	unsigned char dat;
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(addr);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+1);
	if (!Test_ACK())
	{
		return(0);
	}
	dat = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(dat);
}