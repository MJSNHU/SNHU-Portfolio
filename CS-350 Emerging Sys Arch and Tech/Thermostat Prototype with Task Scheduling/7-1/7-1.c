/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART2.h>


/* Driver configuration */
#include "ti_drivers_config.h"

#define DISPLAY(x) UART2_write(uart, &output, x, NULL);

// I2C Global Variables
static const struct {
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = {
    { 0x48, 0x0000, "11X" },
    { 0x49, 0x0000, "116" },
    { 0x41, 0x0001, "006" }
};
uint8_t             txBuffer[1];
uint8_t             rxBuffer[2];
I2C_Transaction     i2cTransaction;

// UART Global Variables
char                output[64];
int                 bytesToSend;

// Driver Handles - Global variables
I2C_Handle      i2c;
UART2_Handle     uart;
Timer_Handle    timer0;

volatile unsigned char TimerFlag = 0;
void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    TimerFlag = 1;
}

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
volatile unsigned char Button0Flag = 0;
void gpioButtonFxn0(uint_least8_t index)
{
    Button0Flag = 1;
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
volatile unsigned char Button1Flag = 0;
void gpioButtonFxn1(uint_least8_t index)
{
    Button1Flag = 1;
}

void initUART(void)
{
    UART2_Params uartParams;
    size_t bytesRead;
    size_t bytesWritten = 0;
    uint32_t status     = UART2_STATUS_SUCCESS;

    /* Create a UART where the default read and write mode is BLOCKING */
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;

    uart = UART2_open(CONFIG_UART2_0, &uartParams);

    if (uart == NULL)
    {
        /* UART2_open() failed */
        while (1) {}
    }
}

// Make sure you call initUART() before calling this function.
void initI2C(void)
{
    int8_t              i, found;
    I2C_Params          i2cParams;

    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "))

    // Init the driver
    I2C_init();

    // Configure the driver
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    // Open the driver
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1);
    }

    DISPLAY(snprintf(output, 32, "Passed\n\r"))

    // Boards were shipped with different sensors.
    // Welcome to the world of embedded systems.
    // Try to determine which sensor we have.
    // Scan through the possible sensor addresses

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;

    found = false;
    for (i=0; i<3; ++i)
    {
        i2cTransaction.targetAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;

        DISPLAY(snprintf(output, 64, "Is this %s? ", sensors[i].id))
        if (I2C_transfer(i2c, &i2cTransaction))
        {
            DISPLAY(snprintf(output, 64, "Found\n\r"))
            found = true;
            break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"))
    }

    if(found)
    {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r", sensors[i].id, i2cTransaction.targetAddress))
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Temperature sensor not found, contact professor\n\r"))
    }
}


void initTimer(void)
{
    Timer_Params    params;

    // Init the driver
    Timer_init();

    // Configure the driver
    Timer_Params_init(&params);
    params.period = 100000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    // Open the driver
    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    DISPLAY( snprintf(output, 64, "Timer Configured\n\r"))
}

int16_t readTemp(void)
{
    int     j;
    int16_t temperature = 0;

    i2cTransaction.readCount  = 2;
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        /*
         * Extract degrees C from the received data;
         * see TMP sensor datasheet
         */
        temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
        temperature *= 0.0078125;

        /*
         * If the MSB is set '1', then we have a 2's complement
         * negative value which needs to be sign extended
         */
        if (rxBuffer[0] & 0x80)
        {
            temperature |= 0xF000;
        }
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Error reading temperature sensor (%d)\n\r",i2cTransaction.status))
        DISPLAY(snprintf(output, 64, "Please power cycle your board by unplugging USB and plugging back in.\n\r"))
    }

    return temperature;
 }

// constants for task periods in milliseconds
const unsigned long tasksPeriodGCD = 100;
const unsigned long periodCheckButton = 200;
const unsigned long periodCheckTemp = 500;
const unsigned long periodPrintData = 1000;
const unsigned short tasksNum = 3;

typedef struct task {
   int state;                  // Task's current state
   unsigned long period;       // Task period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   int (*TckFct)(void);        // Task tick function
} task;
// array of tasks
task tasks[tasksNum];

enum TaskName {CheckButton, CheckTemp, PrintData};
enum ButtonSM {SM_Start, State1};


int TckFct(){

    return State1;

}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    int         j;
    int16_t     temperature = 0;
    int16_t     setpoint = 0;
    uint16_t    timer = 0;
    uint8_t     heat = 0;
    uint32_t    seconds = 0;

    /* Call driver init functions */
    GPIO_init();

#ifdef CONFIG_GPIO_TMP_EN
    GPIO_setConfig(CONFIG_GPIO_TMP_EN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    /* Allow the sensor to power on */
    sleep(1);
#endif

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1) {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
        GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
    }


    initUART(); // The UART must be initialized before calling initI2C()
    DISPLAY( snprintf(output, 64, "UART + GPIO + Timer +I2C + Interrupts by Eric Gregori\n\r"))
    DISPLAY( snprintf(output, 64, "GPIO + Interrupts configured\n\r"))
    initI2C();
    initTimer();


    DISPLAY( snprintf(output, 64, "Starting Task Scheduler\n\r"))

    //initialize  tasks
    int i = CheckButton;
    tasks[i].state = SM_Start;
    tasks[i].period = periodCheckButton;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TckFct = &TckFct;
    i = CheckTemp;
    tasks[i].state = SM_Start;
    tasks[i].period = periodCheckTemp;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TckFct = &TckFct;
    i = PrintData;
    tasks[i].state = SM_Start;
    tasks[i].period = periodPrintData;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TckFct = &TckFct;

    // Loop Forever
    // The student should add flags (similar to the timer flag) to the button handlers.
    // Timer interrupt set to 100ms
    while (1)
    {

        // Every 200ms check the button flags
        // TODO: Add your code
        if (tasks[CheckButton].state == State1){
            tasks[CheckButton].state = SM_Start; // * see note in PrintDisplay below

            if (Button0Flag == 1) {
                Button0Flag = 0;
                ++setpoint;
            }
            if (Button1Flag == 1) {
                Button1Flag = 0;
                --setpoint;
            }
        }

        // Every 500ms read the temperature and update the LED
        // TODO: Add your code
        if (tasks[CheckTemp].state == State1) {
            tasks[CheckTemp].state = SM_Start; // * see note in PrintDisplay below
            temperature = readTemp();
        }
        if (setpoint > temperature) {
            heat = 1;
            GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
        } else {
            heat = 0;
            GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
        }


        // Every second output the following to the UART
        // <%02d,%02d,%d,%04d>, temperature, setpoint, heat, seconds
        // TODO: Add your code here
        if ((tasks[PrintData].state == State1)){
            tasks[PrintData].state = SM_Start;                  ++seconds;
            DISPLAY( snprintf(output, 64, "<%02d,%02d,%d,%04d>\n\r", temperature, setpoint, heat, seconds));
        }

        // Refer to ZyBooks - "Converting different-period tasks to C"
        // Remember to configure the timer period
        while (!TimerFlag){}   // Wait for timer period
        TimerFlag = 0;         // Lower flag raised by timer
        ++timer;

        // For each task, call task tick function if task's period is up
        for (i=0; i < tasksNum; i++) {
           if (tasks[i].elapsedTime >= tasks[i].period){
              // Task is ready to tick, so call its tick function
              tasks[i].state = tasks[i].TckFct();
              tasks[i].elapsedTime = 0; // Reset the elapsed time
           }
           tasks[i].elapsedTime += tasksPeriodGCD;
        }
    }
    return (NULL);
}
