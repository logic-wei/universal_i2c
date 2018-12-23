# universal_i2c
i2c protocol implemented by software.It's aimed at running everywhere.

# build example
```
make
```
![how to build](https://github.com/logic-wei/universal_i2c/raw/master/screenshots/build.png)

# run example
type the command below to show the waveform when write data to i2c device.
```
./app [devAddr] [regAddr] [data]
```
![how to run example](https://github.com/logic-wei/universal_i2c/raw/master/screenshots/example.png)

# how to use

## implement adapter

As same as linux driver,you have to implement some callbacks to show the library how to set gpio level and so on.
Note:
UNIVERSAL_I2C_INT32 is a cross platorm type.It's defined in universal_i2c.h.
```c
void setScl(int level)
{
	//implemented by user
}
void setSda(int level)
{
	//implemented by user
}
void setSdaDir(int dir)
{
	//implemented by user
}
void setSclDir(int dir)
{
	//implemented by user
}
int getSda()
{
	//implemented by user
}
void delayUs(UNIVERSAL_I2C_INT32 us)
{
	//implemented by user	
}

struct UniversalI2cAdapter i2c0Adapter = {
	.setScl = setScl,
	.setSda = setSda,
	.setSdaDir = setSdaDir,
	.setSclDir = setSclDir,
	.getSda = getSda,
	.delayUs = delayUs
};
```

## initialize UniversalI2c structure
argument:
- id

The identity.
- freq

The frequency of i2c clock.This is based on delayUs callback.So it's just a approximate value.
- adapter

some callbacks implemented by user
```c
struct UniversalI2c i2c0 = {
	.id = 0,
	.freq = 1000,
	.adapter = &i2c0Adapter
};
```

## use it to transifer data
When you finished all the work above,it's simple to use i2c.
```c
int main(int argc, char **argv)
{
	UniversalI2c_init(&i2c0);
	UniversalI2c_writeByte(&i2c0, devAddr, regAddr, data);
	return 0;
}
```
