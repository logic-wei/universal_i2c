#include "universal_i2c.h"

//public begin
void UniversalI2c_init(struct UniversalI2c *ui2c)
{
	if (ui2c->freq > 1000000)
		ui2c->freq = 1000000;
	ui2c->adapter->setSclDir(1);
	ui2c->adapter->setSdaDir(1);
	ui2c->adapter->setScl(1);
	ui2c->adapter->setSda(1);
}

int UniversalI2c_id(struct UniversalI2c *ui2c)
{
	return ui2c->id;
}

void UniversalI2c_writeByte(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char byte)
{
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, byte);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_stop(ui2c);
}

void UniversalI2c_writeWord(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned int word)
{
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (word >> 8) & 0xff);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, word & 0xff);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_stop(ui2c);
}

void UniversalI2c_writeBytes(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char *bytes, int length)
{
	int i;

	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	for(i = 0; i < length; i += 1)
	{    
		UniversalI2c_putByteOnBus(ui2c, bytes[i]);
		UniversalI2c_waitForResponse(ui2c);
	}
	UniversalI2c_stop(ui2c);
}

unsigned char UniversalI2c_readByte(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr)
{
	unsigned char byte;

	//write device address
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	//write register address
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	//write device address
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x01);
	UniversalI2c_waitForResponse(ui2c);
	//read register
	byte = UniversalI2c_getByteOnBus(ui2c);
	UniversalI2c_noResponse(ui2c);
	UniversalI2c_stop(ui2c);

	return byte;
}

unsigned int UniversalI2c_readWord(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr)
{
	unsigned int word = 0;

	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x01);
	UniversalI2c_waitForResponse(ui2c);
	word = UniversalI2c_getByteOnBus(ui2c) << 8;
	UniversalI2c_response(ui2c);
	word |= UniversalI2c_getByteOnBus(ui2c);
	UniversalI2c_noResponse(ui2c);
	UniversalI2c_stop(ui2c);

	return word;
}

void UniversalI2c_readBytes(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char *bytes, int length)
{
	int i;

	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x00);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_putByteOnBus(ui2c, regAddr);
	UniversalI2c_waitForResponse(ui2c);
	UniversalI2c_start(ui2c);
	UniversalI2c_putByteOnBus(ui2c, (devAddr << 1) | 0x01);
	UniversalI2c_waitForResponse(ui2c);
	for(i = 0; i < length; i += 1)
	{
		bytes[i] = UniversalI2c_getByteOnBus(ui2c);
		if(i == length - 1)
			UniversalI2c_noResponse(ui2c);
		else
			UniversalI2c_response(ui2c);
	}
	UniversalI2c_stop(ui2c);
}
//public end

//private begin
void UniversalI2c_waitForAWhile(struct UniversalI2c *ui2c)
{
	ui2c->adapter->delayUs(1000000 / ui2c->freq);
}

void UniversalI2c_start(struct UniversalI2c *ui2c)
{
	ui2c->adapter->setScl(1);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setSda(0);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(0);
	UniversalI2c_waitForAWhile(ui2c);
}

void UniversalI2c_stop(struct UniversalI2c *ui2c)
{
	ui2c->adapter->setSda(0);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(1);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setSda(1);
	UniversalI2c_waitForAWhile(ui2c);
}

void UniversalI2c_waitForResponse(struct UniversalI2c *ui2c)
{
	int i = 0;

	ui2c->adapter->setSda(1);
	ui2c->adapter->setSdaDir(0);
	ui2c->adapter->setScl(1);
	UniversalI2c_waitForAWhile(ui2c);
	while (ui2c->adapter->getSda() && i < 500) {i += 1;}
	ui2c->adapter->setSdaDir(1);
	ui2c->adapter->setScl(0);
	UniversalI2c_waitForAWhile(ui2c);
}

void UniversalI2c_response(struct UniversalI2c *ui2c)
{
	ui2c->adapter->setSda(0);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(1);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(0);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setSda(1);
	UniversalI2c_waitForAWhile(ui2c);
}

void UniversalI2c_noResponse(struct UniversalI2c *ui2c)
{
	ui2c->adapter->setSda(1);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(1);
	UniversalI2c_waitForAWhile(ui2c);
	ui2c->adapter->setScl(0);
	UniversalI2c_waitForAWhile(ui2c);
}

void UniversalI2c_putByteOnBus(struct UniversalI2c *ui2c, unsigned char byte)
{
	int i;

	for(i = 0; i < 8; i += 1)
	{
		if(byte & (0x80 >> i))
			ui2c->adapter->setSda(1);
		else
			ui2c->adapter->setSda(0);
		UniversalI2c_waitForAWhile(ui2c);
		ui2c->adapter->setScl(1);//send data
		UniversalI2c_waitForAWhile(ui2c);
		ui2c->adapter->setScl(0);
		UniversalI2c_waitForAWhile(ui2c);
	}
	ui2c->adapter->setSda(0);//release
	UniversalI2c_waitForAWhile(ui2c);
}

unsigned char UniversalI2c_getByteOnBus(struct UniversalI2c *ui2c)
{
	unsigned char data = 0;
	int i;
	
	ui2c->adapter->setSda(1);//release SDA to get data
	ui2c->adapter->setSdaDir(0);
	for(i = 0; i < 8; i += 1)
	{
		ui2c->adapter->setScl(1);//to get data
		UniversalI2c_waitForAWhile(ui2c);
		if(ui2c->adapter->getSda())
			data = data | (0x80 >> i);
		ui2c->adapter->setScl(0);//to refresh data
		UniversalI2c_waitForAWhile(ui2c);
	}
	ui2c->adapter->setSdaDir(1);

	return data;
}
//private end
