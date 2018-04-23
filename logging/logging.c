
#include "stm32f1xx_hal.h"
#include "logging.h"
#include <stdio.h>
#include <stdarg.h>



void Logging_Write(const char *format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
}
