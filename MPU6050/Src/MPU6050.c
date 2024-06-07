//Header files
#include "MPU6050.h"
#include "i2c.h"
#include "time_base.h"


//Library Variable
//2- Accel & Gyro Scaling Factor
static float accelScalingFactor, gyroScalingFactor;
//3- Bias variables
static float A_X_Bias = 0.0f;
static float A_Y_Bias = 0.0f;
static float A_Z_Bias = 0.0f;

static int16_t GyroRW[3];




//4- MPU6050 Initialaztion Configuration 
void MPU6050_Config(MPU_ConfigTypeDef *config)
{
	uint8_t Buffer = 0;
	//Clock Source 
	//Reset Device

	i2c_writeByte(MPU_ADDR, PWR_MAGT_1_REG, 0x80);

	delay(100);

	Buffer = config ->ClockSource & 0x07; //change the 7th bits of register

	Buffer |= (config ->Sleep_Mode_Bit << 6) &0x40; // change only the 7th bit in the register
	
	i2c_writeByte(MPU_ADDR,PWR_MAGT_1_REG, Buffer);

	delay(100); // should wait 10ms after changeing the clock setting.

	//Set the Digital Low Pass Filter
	Buffer = 0;

	Buffer = config->CONFIG_DLPF & 0x07;
	
	i2c_writeByte(MPU_ADDR,CONFIG_REG, Buffer);

	//Select the Gyroscope Full Scale Range
	Buffer = 0;

	Buffer = (config->Gyro_Full_Scale << 3) & 0x18;
	
	i2c_writeByte(MPU_ADDR,GYRO_CONFIG_REG, Buffer);

	//Select the Accelerometer Full Scale Range 
	Buffer = 0; 

	Buffer = (config->Accel_Full_Scale << 3) & 0x18;

	i2c_writeByte(MPU_ADDR,ACCEL_CONFIG_REG, Buffer);

	//Set SRD To Default
	MPU6050_Set_SMPRT_DIV(0x04);
	
	
	//Accelerometer Scaling Factor, Set the Accelerometer and Gyroscope Scaling Factor
	switch (config->Accel_Full_Scale)
	{
		case AFS_SEL_2g:
			accelScalingFactor = (2000.0f/32768.0f);
			break;
		
		case AFS_SEL_4g:
			accelScalingFactor = (4000.0f/32768.0f);
				break;
		
		case AFS_SEL_8g:
			accelScalingFactor = (8000.0f/32768.0f);
			break;
		
		case AFS_SEL_16g:
			accelScalingFactor = (16000.0f/32768.0f);
			break;
		
		default:
			break;
	}
	//Gyroscope Scaling Factor 
	switch (config->Gyro_Full_Scale)
	{
		case FS_SEL_250:
			gyroScalingFactor = 250.0f/32768.0f;
			break;
		
		case FS_SEL_500:
				gyroScalingFactor = 500.0f/32768.0f;
				break;
		
		case FS_SEL_1000:
			gyroScalingFactor = 1000.0f/32768.0f;
			break;
		
		case FS_SEL_2000:
			gyroScalingFactor = 2000.0f/32768.0f;
			break;
		
		default:
			break;
	}
	
}

//5- Get Sample Rate Divider
uint8_t MPU6050_Get_SMPRT_DIV(void)
{
	uint8_t Buffer = 0;
	
	i2c_readByte(MPU_ADDR,SMPLRT_DIV_REG,&Buffer);

	return Buffer;
}

//6- Set Sample Rate Divider
void MPU6050_Set_SMPRT_DIV(uint8_t SMPRTvalue)
{
	i2c_writeByte(MPU_ADDR,SMPLRT_DIV_REG, SMPRTvalue);
}

//7- Get External Frame Sync.
uint8_t MPU6050_Get_FSYNC(void)
{
	uint8_t Buffer = 0;
	
	i2c_readByte(MPU_ADDR,CONFIG_REG, &Buffer);
	Buffer &= 0x38; 
	return (Buffer>>3);
}

//8- Set External Frame Sync. 
void MPU6050_Set_FSYNC(enum EXT_SYNC_SET_ENUM ext_Sync)
{
	uint8_t Buffer = 0;
	i2c_readByte(MPU_ADDR,CONFIG_REG, &Buffer);
	Buffer &= ~0x38;
	
	Buffer |= (ext_Sync <<3); 
	i2c_writeByte(MPU_ADDR,CONFIG_REG, Buffer);
	
}

//9- Get Accel Raw Data
void MPU6050_Get_Accel_RawData(RawData_Def *rawDef)
{
	uint8_t state;
	uint8_t AcceArr[6], GyroArr[6];
	
	i2c_readByte(MPU_ADDR,INT_STATUS_REG, &state);


	if((state&&0x01))
	{
		i2c_ReadMulti(MPU_ADDR,ACCEL_XOUT_H_REG, 6,AcceArr);
		
		//Accel Raw Data
		rawDef->x = ((AcceArr[0]<<8) + AcceArr[1]); // x-Axis
		rawDef->y = ((AcceArr[2]<<8) + AcceArr[3]); // y-Axis
		rawDef->z = ((AcceArr[4]<<8) + AcceArr[5]); // z-Axis
		//Gyro Raw Data
		i2c_ReadMulti(MPU_ADDR,GYRO_XOUT_H_REG, 6,GyroArr);
		GyroRW[0] = ((GyroArr[0]<<8) + GyroArr[1]);
		GyroRW[1] = (GyroArr[2]<<8) + GyroArr[3];
		GyroRW[2] = ((GyroArr[4]<<8) + GyroArr[5]);
	}
}

//10- Get Accel scaled data (g unit of gravity, 1g = 9.81m/s2)
void MPU6050_Get_Accel_Scale(ScaledData_Def *scaledDef)
{

	RawData_Def AccelRData;
	MPU6050_Get_Accel_RawData(&AccelRData);
	
	//Accel Scale data 
	scaledDef->x = ((AccelRData.x+0.0f)*accelScalingFactor)/1000;
	scaledDef->y = ((AccelRData.y+0.0f)*accelScalingFactor)/1000;
	scaledDef->z = ((AccelRData.z+0.0f)*accelScalingFactor)/1000;
}

//11- Get Accel calibrated data
void MPU6050_Get_Accel_Cali(ScaledData_Def *CaliDef)
{
	ScaledData_Def AccelScaled;
	MPU6050_Get_Accel_Scale(&AccelScaled);
	
	//Accel Scale data 
	CaliDef->x = (AccelScaled.x) - A_X_Bias; // x-Axis
	CaliDef->y = (AccelScaled.y) - A_Y_Bias;// y-Axis
	CaliDef->z = (AccelScaled.z) - A_Z_Bias;// z-Axis
}

//12- Get Gyro Raw Data
void MPU6050_Get_Gyro_RawData(RawData_Def *rawDef)
{
	
	//Accel Raw Data
	rawDef->x = GyroRW[0];
	rawDef->y = GyroRW[1];
	rawDef->z = GyroRW[2];
	
}

//13- Get Gyro scaled data
void MPU6050_Get_Gyro_Scale(ScaledData_Def *scaledDef)
{
	RawData_Def myGyroRaw;
	MPU6050_Get_Gyro_RawData(&myGyroRaw);
	
	//Gyro Scale data 
	scaledDef->x = (myGyroRaw.x)*gyroScalingFactor; // x-Axis
	scaledDef->y = (myGyroRaw.y)*gyroScalingFactor; // y-Axis
	scaledDef->z = (myGyroRaw.z)*gyroScalingFactor; // z-Axis
}

//14- Accel Calibration
void _Accel_Cali(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
{
	//1* X-Axis calibrate
	A_X_Bias		= (x_max + x_min)/2.0f;
	
	//2* Y-Axis calibrate
	A_Y_Bias		= (y_max + y_min)/2.0f;
	
	//3* Z-Axis calibrate
	A_Z_Bias		= (z_max + z_min)/2.0f;
}
