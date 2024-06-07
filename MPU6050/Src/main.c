#include "i2c.h"
#include "time_base.h"
#include "uart.h"
#include "stdio.h"
#include "MPU6050.h"

#define rate 300
uint32_t previous;


int main(void)
{
	Ticks_Init(16000000);

	uart2_rxtx_init();

	i2c_init();

	MPU_ConfigTypeDef myConfig;

	myConfig.Accel_Full_Scale = AFS_SEL_4g;
	myConfig.ClockSource = Internal_8MHz;
	myConfig.CONFIG_DLPF = DLPF_184A_188G_Hz;
	myConfig.Sleep_Mode_Bit = 0;
	myConfig.Gyro_Full_Scale = FS_SEL_500;

	MPU6050_Config(&myConfig);

	ScaledData_Def meAccel;

	while(1)
	{
		MPU6050_Get_Accel_Scale(&meAccel);
		if(get_Ticks()-previous >rate)
		{
		    printf("Accel: X = %.2f Y = %.2f Z = %.2f\r\n", meAccel.x, meAccel.y, meAccel.z);
		    printf("==============================\r\n");
			previous=get_Ticks();
		}
	}

}
