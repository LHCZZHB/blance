#include "../Drivers/SYSTEM/sys/sys.h"
#include "../Drivers/SYSTEM/usart/usart.h"
#include "../Drivers/SYSTEM/delay/delay.h"
#include "../Drivers/BSP/OLED/oled.h"
#include "../Drivers/BSP/MPU6050/mpu6050.h"

int16_t accel_data[3] = {0};
int16_t gyro_data[3] = {0};

int main(void)
{
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟为72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    
    oled_init();
    oled_clear();
    mpu6050_init();
    
    oled_showstring(0, 0, "Hello!", 6);
    while(1)
    {
        mpu6050_getdata(accel_data ,gyro_data);
        printf("%d, %d, %d, %d, %d, %d\r\n",accel_data[0], accel_data[1], accel_data[2], gyro_data[0], gyro_data[1], gyro_data[2]);
    }
}
