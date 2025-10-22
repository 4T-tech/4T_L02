#include "at24c02.h"

void write_AT24C02(unsigned char addr, unsigned char dat)
{
    // 发送起始信号
    I2C_Start();
    
    // 发送设备地址加上写操作位
    I2C_send_byte(AT24C02_ADDR | 0x00); // AT24C02的写地址是基础地址加上0
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送内存地址
    I2C_send_byte(addr);
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送要写入的数据
    I2C_send_byte(dat);
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送停止信号
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