#ifndef TASKSKETULER_H
#define TASKSKETULER_H

#include <stdint.h>

typedef struct{
    uint32_t period;
    uint32_t onTime;
    int32_t offset;
    uint8_t active;
    void (*activate) ();
    void (*deactivate) ();
} task_t;
void runTasks(uint32_t nowint);
#endif // TASKSKETULER_H
