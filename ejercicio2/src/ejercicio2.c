/*==================[inclusions]=============================================*/
#include "../inc/ejercicio2.h"         /* <= own header */
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
 * Ejercicio 2: Identificar las posiciones de los leds
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();
   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");
   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   SetRelAlarm(ActivatePeriodicTask, 350, 500);

   /* terminate task */
   TerminateTask();
}


uint8_t outputs = 0b00000000;
TASK(PeriodicTask)
{

   ciaaPOSIX_printf("Blinking\n");

   /* blink output */
   ciaaPOSIX_read(fd_out, &outputs, 1);
   //outputs ^= 0b00100000; // Verde        (Led 3)
   //outputs ^= 0b00010000; // Rojo         (Led 2)
   //outputs ^= 0b00001000; // Amarillo     (Led 1)
   //outputs ^= 0b00000100; // Azul  [B]    (Led RGB)
   //outputs ^= 0b00000010; // Verde [G]    (Led RGB)
   //outputs ^= 0b00000001; // Rojo  [R]    (Led RGB)
   outputs ^= 0b00000111; // Blanco [RGB  (Led RGB)
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* terminate task */
   TerminateTask();
}
