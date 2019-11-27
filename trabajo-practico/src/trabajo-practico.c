#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "trabajo-practico.h"         /* <= own header */

static int32_t botones;
static int32_t leds;
static int32_t N = 1;

int main(void) {
    StartOS(AppMode1);

    return 0;
}

void ErrorHook(void) {
    ciaaPOSIX_printf("ErrorHook was called\n");
    ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n",
            OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(),
            OSErrorGetParam3(), OSErrorGetRet());
    ShutdownOS(0);
}

TASK(IniciarTareas) {
    ciaak_start();

    leds = ciaaPOSIX_open(OUTPUTS, ciaaPOSIX_O_RDWR);
    botones = ciaaPOSIX_open(INPUTS, ciaaPOSIX_O_RDWR);

    SetRelAlarm(ActivarTarea1, 0, 400);
    SetRelAlarm(ActivarTarea2, 0, 600);
    SetRelAlarm(ActivarTarea3, 0, 800);
    SetRelAlarm(ActivarVerPulsador1, 0, 100);
    SetRelAlarm(ActivarVerPulsador4, 0, 100);

    TerminateTask();

}

TASK(Tarea1) {
    uint8_t outputs = 0b00000001;

    ciaaPOSIX_write(leds, &outputs, 1);

    printf("T1: %d\n", N * 1);

    TerminateTask();
}

TASK(Tarea2) {
    uint8_t outputs = 0b00001000;
    ciaaPOSIX_write(leds, &outputs, 1);

    printf("T2: %d\n", N * 10);

    TerminateTask();
}

TASK(Tarea3) {
    uint8_t outputs = 0b00010000;
    ciaaPOSIX_write(leds, &outputs, 1);

    printf("T3: %d\n", N * 100);

    TerminateTask();
}

TASK(Tarea4) {
    N++;
    TerminateTask();
}

TASK(Tarea5) {
    N *= 1000;
    uint8_t outputs = 0b00100000;
    ciaaPOSIX_write(leds, &outputs, 1);
    TerminateTask();
}

uint8_t boton_1_estado_anterior = 0;
uint8_t boton_4_estado_anterior = 0;

TASK(VerPulsador1) {
    uint8_t estado_botones, boton_1_presionado;

    (void) ciaaPOSIX_read(botones, &estado_botones, 1);

    estado_botones = ~estado_botones;
    boton_1_presionado = estado_botones & BUTTON_1;

    if (boton_1_presionado && !boton_1_estado_anterior) {
        ActivateTask(Tarea4);
        boton_1_estado_anterior = 1;
    }

    if (!boton_1_presionado && boton_1_estado_anterior){
        boton_1_estado_anterior = 0;
    }

    TerminateTask();
}

TASK(VerPulsador4) {
    uint8_t estado_botones, boton_4_presionado;

    (void) ciaaPOSIX_read(botones, &estado_botones, 1);

    estado_botones = ~estado_botones;
    boton_4_presionado = estado_botones & BUTTON_4;

    if (boton_4_presionado && !boton_4_estado_anterior) {
        ActivateTask(Tarea5);
        boton_4_estado_anterior = 1;
    }

    if (!boton_4_presionado && boton_4_estado_anterior){
        boton_4_estado_anterior = 0;
    }

    TerminateTask();
}
