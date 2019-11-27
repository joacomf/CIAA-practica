#include "ejercicio4.h"         /* <= own header */
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */

static int32_t fd_out;
static int32_t botones;

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
 * Ejercicio 4: Activar la lógica del ejercicio 3 con los botones
 */
TASK(InitTask)
{
   ciaak_start();

   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);
   botones = ciaaPOSIX_open("/dev/dio/in/0", ciaaPOSIX_O_RDONLY);

   SetRelAlarm(ActivatePeriodicTask, 350, 10);

   TerminateTask();
}

uint8_t leds = 0b00000000;
TASK(PeriodicTask)
{
   static uint8_t estado_anterior_del_boton;
   uint8_t estado_actual_del_boton, boton_presionado;

   ciaaPOSIX_read(botones, &estado_actual_del_boton, 1);

   estado_actual_del_boton = ~estado_actual_del_boton;
   boton_presionado = (~estado_anterior_del_boton) & (estado_actual_del_boton);

   //implementación con IF
   /*
    * if(boton_presionado) {
    *   outputs += 1;
    * }
    */

   //implementación sin IF
   leds += 1 && boton_presionado;
   ciaaPOSIX_write(fd_out, &leds, 1);

   estado_anterior_del_boton = estado_actual_del_boton;
   /* terminate task */
   TerminateTask();
}
