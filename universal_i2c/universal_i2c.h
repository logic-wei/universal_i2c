#ifndef _UNIVERSAL_I2C_H
#define _UNIVERSAL_I2C_H

#define UNIVERSAL_I2C_INT32 int

struct UniversalI2cAdapter {
	void (*setScl)(int level);
	void (*setSda)(int level);
	void (*setSclDir)(int dir);//0-input 1-output
	void (*setSdaDir)(int dir);
	int (*getSda)();
	void (*delayUs)(UNIVERSAL_I2C_INT32 us);
};

struct UniversalI2c {
	int id;
	UNIVERSAL_I2C_INT32 freq;//unit:HZ it's just a approximate value 
	struct UniversalI2cAdapter *adapter;
};

//public:
void UniversalI2c_init(struct UniversalI2c *ui2c);
int UniversalI2c_id(struct UniversalI2c *ui2c);
void UniversalI2c_writeByte(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char byte);
void UniversalI2c_writeWord(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr,unsigned int word);
void UniversalI2c_writeBytes(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char *bytes, int length);
unsigned char UniversalI2c_readByte(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr);
unsigned int UniversalI2c_readWord(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr);
void UniversalI2c_readBytes(struct UniversalI2c *ui2c, unsigned char devAddr, unsigned char regAddr, unsigned char *bytes, int length);

//private:
void UniversalI2c_start(struct UniversalI2c *ui2c);
void UniversalI2c_stop(struct UniversalI2c *ui2c);
void UniversalI2c_waitForResponse(struct UniversalI2c *ui2c);
void UniversalI2c_response(struct UniversalI2c *ui2c);
void UniversalI2c_noResponse(struct UniversalI2c *ui2c);
void UniversalI2c_putByteOnBus(struct UniversalI2c *ui2c,unsigned char byte);
unsigned char UniversalI2c_getByteOnBus(struct UniversalI2c *ui2c);




#endif
