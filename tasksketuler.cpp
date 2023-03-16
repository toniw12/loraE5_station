#include "tasksketuler.h"

extern task_t tasks[];
extern int taskList_LEN;

void runTasks(uint32_t nowint){
    for(int i =0;i< taskList_LEN;i++){
        task_t * task=&tasks[i];
        uint32_t nowPhase=(nowint-task->offset)%task->period;
        if (nowPhase<task->onTime){
            if(task->active==0){
                task->active=1;
                if(task->activate){
                    task->activate();
                }
            }
        }
        else{
            if(task->active==1){
                task->active=0;
                if(task->deactivate){
                    task->deactivate();
                }
            }
        }
    }
}
