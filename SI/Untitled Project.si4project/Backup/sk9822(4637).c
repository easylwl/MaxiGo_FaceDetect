#include <spi.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysctl.h>
#include <string.h>
#include <stdbool.h>
#include <gpio.h>
#include <fpioa.h>
#include "sk9822.h"
#include <assert.h>




uint32_t sk9822_ledbuf[SK9822_BUF_LEN];

bool sk9822_clear(void)
{
    sk9822_ledbuf[0]=0;
    for (int i=1;i<13;i++)
    {
       sk9822_ledbuf[i]=  0xe0000000;   
    }
    sk9822_ledbuf[13]=0xffffffff;
    sk9822_send_data_gpiohs(sk9822_ledbuf,SK9822_BUF_LEN);
    return true;
}

bool sk9822_seton(void)
{
    sk9822_ledbuf[0]=0;
    for (int i=1;i<13;i++)
    {
       sk9822_ledbuf[i]=  0xefffffff;   
    }
    sk9822_ledbuf[13]=0xffffffff;
    sk9822_send_data_gpiohs(sk9822_ledbuf,SK9822_BUF_LEN);
    return true;
}

void sk9822_ledon(uint16_t lednum,uint8_t R, uint8_t G, uint8_t B)
{
    uint32_t sendv;
    if (lednum<12)
    {
        sk9822_ledbuf[0]=0;
        for (int i=1;i<13;i++)
        {
        sk9822_ledbuf[i]=  0xe0000000;   
        }
        sendv=0xef;
        sendv=(sendv<<8)+B;
        sendv=(sendv<<8)+G;
        sendv=(sendv<<8)+R;
        sk9822_ledbuf[lednum+1]=  sendv; 
        sk9822_ledbuf[13]=0xffffffff;
        sk9822_send_data_gpiohs(sk9822_ledbuf,SK9822_BUF_LEN);
    }
        
}


#define SK9822_DATA_GPION    4
#define SK9822_SCLK_GPION    5

void sk9822_init_gpiohs(void)
{
    fpioa_init();
    fpioa_set_function(24, FUNC_GPIO4);
    fpioa_set_function(25, FUNC_GPIO5);
    gpio_init();
    gpio_set_drive_mode(SK9822_DATA_GPION, GPIO_DM_OUTPUT);
    gpio_set_drive_mode(SK9822_SCLK_GPION, GPIO_DM_OUTPUT);

    gpio_set_pin(SK9822_DATA_GPION, GPIO_PV_LOW);
    gpio_set_pin(SK9822_SCLK_GPION, GPIO_PV_LOW);

    memset(sk9822_ledbuf,0,sizeof(sk9822_ledbuf)-1);
    sk9822_ledbuf[13]=0xffffffff;
}

bool sk9822_send_data_gpiohs(uint32_t *leddata,uint8_t led_cnt)
{
    if (leddata == NULL)
        return false;

#define DUMMY_CYCLE 100

    for(uint32_t i = 0; i < led_cnt; i++)
    {
        for (uint32_t mask = 0x80000000; mask > 0; mask >>= 1)
        {
            if (*(leddata + i) & mask)
                gpio_set_pin(SK9822_DATA_GPION, GPIO_PV_HIGH);
            else
                gpio_set_pin(SK9822_DATA_GPION, GPIO_PV_LOW);

            gpio_set_pin(SK9822_SCLK_GPION, GPIO_PV_LOW);
            for (uint64_t j = 0; j < DUMMY_CYCLE; j++)
                asm volatile("nop");
            gpio_set_pin(SK9822_SCLK_GPION, GPIO_PV_HIGH);    
            for (uint64_t j = 0; j < DUMMY_CYCLE; j++)
                asm volatile("nop");
        }
    }
    gpio_set_pin(SK9822_SCLK_GPION, GPIO_PV_LOW);
    return true;
}

