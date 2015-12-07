#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "driver/uart.h"
#include "user_config.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);
static int cycleCnt = 0;

static volatile os_timer_t some_timer;


void some_timerfunc(void *arg)
{
    os_printf("Blinky: %d\n", cycleCnt);
    cycleCnt += 1;

    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT13)
    {
        //Set GPIO2 to LOW
        gpio_output_set(0, BIT13, BIT13, 0);
    }
    else
    {
        //Set GPIO2 to HIGH
        gpio_output_set(BIT13, 0, BIT13, 0);
    }
}

//Do nothing function
static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
    os_delay_us(10);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    uart_init(BIT_RATE_57600, BIT_RATE_57600);
    // Initialize the GPIO subsystem.
    gpio_init();

    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

    //Set GPIO2 low
    gpio_output_set(0, BIT13, BIT13, 0);

    //Disarm timer
    os_timer_disarm(&some_timer);

    //Setup timer
    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //500 is the fire time in ms
    //0 for once and 1 for repeating
    os_timer_arm(&some_timer, 10, 1);
    
    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}
