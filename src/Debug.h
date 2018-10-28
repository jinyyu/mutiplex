#ifndef LIBREACTOR_DISTRIBUTION_DEBUG_H
#define LIBREACTOR_DISTRIBUTION_DEBUG_H

#include <string.h>
#include <stdio.h>
#include <string>


#define LOG_DEBUG(format, ...) do { fprintf(stdout, "[DEBUG] [%s:%d] " format "\n", strrchr(__FILE__, '/') + 1, __LINE__, ##__VA_ARGS__); } while(0)

#endif //LIBREACTOR_DISTRIBUTION_DEBUG_H
