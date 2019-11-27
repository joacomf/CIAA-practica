#include "../inc/ejercicio5.h"         /* <= own header */

#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */

static int32_t leds;
static int32_t botones;
static int32_t boton_1_presionado = 0;
static int32_t boton_2_presionado = 0;
static int32_t boton_3_presionado = 0;
static int32_t boton_4_presionado = 0;

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
 * Ejercicio 5: Prender leds individualmente con los botones
 */
TASK(InitTask)
{
   ciaak_start();

   leds = ciaaPOSIX_open(OUTPUTS, ciaaPOSIX_O_RDWR);
   botones = ciaaPOSIX_open(INPUTS, ciaaPOSIX_O_RDWR);

   SetRelAlarm(ActivarPrenderLeds, 350, 50);
   SetRelAlarm(ActivarVerBotones, 350, 50);

   TerminateTask();
}


TASK(PrenderLeds) {
    uint8_t outputs = 0;

    if (boton_1_presionado) {
       outputs |= 0b00000111;
    }

    if (boton_2_presionado) {
       outputs |= 0b00001000;
    }

    if (boton_3_presionado) {
       outputs |= 0b00010000;
    }

    if (boton_4_presionado) {
       outputs |= 0b00100000;
    }

    ciaaPOSIX_write(leds, &outputs, 1);

    TerminateTask();
}

TASK(VerBotones){
    uint8_t estado_de_botones;

    ciaaPOSIX_read(botones, &estado_de_botones, 1);

    boton_1_presionado = ~estado_de_botones & BUTTON_1;
    boton_2_presionado = ~estado_de_botones & BUTTON_2;
    boton_3_presionado = ~estado_de_botones & BUTTON_3;
    boton_4_presionado = ~estado_de_botones & BUTTON_4;

    TerminateTask();

}
