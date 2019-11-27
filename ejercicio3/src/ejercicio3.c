/*==================[inclusions]=============================================*/
#include "../inc/ejercicio3.h"         /* <= own header */
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */

/**
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;


int main(void)
{
   StartOS(AppMode1);
   return 0;
}

void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/*
 * Ejercicio 3: Encender de a uno los LEDs hasta que esten todos encendidos
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /* activate periodic task:
    *  - for the first time after 350 ticks (350 ms)
    *  - and then every 250 ticks (250 ms)
    */
   SetRelAlarm(ActivatePeriodicTask, 350, 150);

   /* terminate task */
   TerminateTask();
}


uint8_t outputs = 0b00000000;
TASK(PeriodicTask)
{

   /* write blinking message */
   ciaaPOSIX_printf("Blinking\n");

   /* blink output */
   ciaaPOSIX_read(fd_out, &outputs, 1);
   outputs += 1;
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* terminate task */
   TerminateTask();
}
