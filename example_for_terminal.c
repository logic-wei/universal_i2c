#include "universal_i2c/universal_i2c.h"
#include "stdio.h"
#include "string.h"

void setScl(int level)
{
	//print red -/_ to show the level
	if (level)
		printf("\033[33m-");
	else
		printf("\033[33m_");
}
void setSda(int level)
{
	//print yello -/_ to show the level
	if (level)
		printf("\033[32m-");
	else
		printf("\033[32m_");
}
void setSdaDir(int dir)
{
	
}
void setSclDir(int dir)
{
	
}
int getSda()
{
	return 0;
}
void delayUs(int us)
{
	
}

struct UniversalI2cAdapter i2c0Adapter = {
	.setScl = setScl,
	.setSda = setSda,
	.setSdaDir = setSdaDir,
	.setSclDir = setSclDir,
	.getSda = getSda,
	.delayUs = delayUs
};

struct UniversalI2c i2c0 = {
	.id = 0,
	.freq = 1000,
	.adapter = &i2c0Adapter
};

int main(int argc, char **argv)
{
	unsigned int devAddr, regAddr, data;
	if (argc == 1) {
		printf("help\n");
		return 0;
	}
	sscanf(argv[1], "0x%x", &devAddr);
	sscanf(argv[2], "0x%x", &regAddr);
	sscanf(argv[3], "0x%x", &data);
	printf("devAddr=0x%x regAddr=0x%x data=0x%x\n", devAddr, regAddr, data);
	printf("universal i2c example:\n"
		"\033[33myello -/_ refer to the scl line		\033[32mgreen -/_ refer to the sda line\n");
	UniversalI2c_init(&i2c0);
	UniversalI2c_writeByte(&i2c0, devAddr, regAddr, data);
	return 0;
}