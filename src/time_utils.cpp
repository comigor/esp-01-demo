#include <Arduino.h>
#include <time.h>

#include "time_utils.h"

String formatTime(String format)
{
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    char time_human[12] = {0};
    strftime(time_human, 12, const_cast<char *>(format.c_str()), ptm);
    return time_human;
}
