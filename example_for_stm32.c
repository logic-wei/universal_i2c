/*
This is just a very simple example to show you how to use universal_i2c library on stm32.
*/
#include "universal_i2c/universal_i2c.h"

void setScl(int level);
void setSda(int level);
void setSdaDir(int dir);
void setSclDir(int dir);
int getSda();
void delayUs(int us);

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
	.freq = 10000,
	.adapter = &i2c0Adapter
};

int main(void)
{
	UniversalI2c_init(&i2c0);
	while (1) {
		unsigned char data;
		data = UniversalI2c_readByte(&i2c0, 0x6b, 0x0b);
		HAL_UART_Transmit(&huart2, (uint8_t *)&data, 1, 1000);
		HAL_Delay(1000);
	}
}

#define SCL_GPIO GPIOC
#define SDA_GPIO GPIOC
#define SCL_PIN GPIO_PIN_2
#define SDA_PIN GPIO_PIN_3

void setScl(int level)
{
	if (level)
		HAL_GPIO_WritePin(SCL_GPIO, SCL_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SCL_GPIO, SCL_PIN, GPIO_PIN_RESET);
}

void setSda(int level)
{
	if (level)
		HAL_GPIO_WritePin(SDA_GPIO, SDA_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SDA_GPIO, SDA_PIN, GPIO_PIN_RESET);
}

void setSdaDir(int dir)
{
	static GPIO_InitTypeDef gpio_initStr = {
		.Pin = SDA_PIN,
		.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
		.Pull = GPIO_PULLUP,
		.Mode = GPIO_MODE_OUTPUT_OD,
	};
	if (dir)
		gpio_initStr.Mode = GPIO_MODE_OUTPUT_OD;
	else
		gpio_initStr.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(SDA_GPIO, &gpio_initStr);
}

void setSclDir(int dir)
{
		static GPIO_InitTypeDef gpio_initStr = {
		.Pin = SCL_PIN,
		.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
		.Pull = GPIO_PULLUP,
		.Mode = GPIO_MODE_OUTPUT_OD,
	};
	if (dir)
		gpio_initStr.Mode = GPIO_MODE_OUTPUT_OD;
	else
		gpio_initStr.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(SCL_GPIO, &gpio_initStr);
}

int getSda()
{
	return HAL_GPIO_ReadPin(GPIOC, SDA_PIN);
}

void delayUs(int us)
{
	int i;
	while (us--) {
	}
}