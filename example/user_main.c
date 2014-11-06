#include "ets_sys.h"
#include "i2c/i2c.h"
#include "driver/uart.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "config.h"

os_event_t    user_procTaskQueue[user_procTaskQueueLen];

static void user_procTask(os_event_t *events);

static volatile os_timer_t sensor_timer;


void sensor_timerfunc(void *arg)
{
    uint8 data;
    data = eeprom_readByte(0x50,0x01);
    os_printf("Byte: %x \n\r",data);
    data = data + 1;
    if(!eeprom_writeByte(0x50,0x01,data))
        os_printf("Write failed\n\r");
}

static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
    os_delay_us(5000);
}

void user_init(void)
{
    char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;

    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 32);

    //Init uart
    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    //Setup wifi
    wifi_set_opmode( 0x1 );
    wifi_station_set_config(&stationConf);

    i2c_init();

    uart0_sendStr("Booting\r\n");

    if(!eeprom_writeByte(0x50,0x01,0x00))
        os_printf("Write failed\n\r");

    //Disarm timer
    os_timer_disarm(&sensor_timer);

    //Setup timer
    os_timer_setfn(&sensor_timer, (os_timer_func_t *)sensor_timerfunc, NULL);

    //Arm timer for every 10 sec.
    os_timer_arm(&sensor_timer, 5000, 1);

    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}

