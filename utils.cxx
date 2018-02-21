#include <utils.h>
#include <stdio.h>

time_t time_file_name(char *buffer, size_t max, time_t t)
{
    time_t acas = 0;
    if (t == 0)
        acas = time(0);
    else
        acas = t;
    //
    struct tm *cas = localtime(&acas);
    const char *months[12] = { "jan", "feb", "mar", "apr", "may", "jun", "jul",
                               "aug", "sep", "oct", "nov", "dec"
                             };
    snprintf(buffer, max, "%s%02d-%02d_%02d_%02d", months[cas->tm_mon], cas->tm_mday,
            cas->tm_hour, cas->tm_min, cas->tm_sec);
    return acas;
}
