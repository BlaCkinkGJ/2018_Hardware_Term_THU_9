#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "universal.h"

typedef struct _BLUE_CONFIG{
    bool isValid;
    bool isATscan;
}BlueConfig;

void bluetooth_init(BlueConfig *config);
void bluetooth_send_data(const char *data, BlueConfig *config);

#endif