#ifndef __MULTITAKING_H__
#define __MULTITAKING_H__

#include "types.h"
#include "gdt.h"

struct CPUState
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    /*
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    */

    uint32_t error;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;

} __attribute__ ((packed));

class Task
{
    friend class TaskManager;
public:
    Task(GlobalDescriptorTable* gdt, void entrypoint());
    ~Task();
private:
    uint8_t stack[4096];
    CPUState* cpustate;
};


class TaskManager
{
public:
    TaskManager();
    ~TaskManager();

    bool AddTask(Task* task);
    CPUState* Schedule(CPUState* cpustate);
private:
    Task* tasks[256];
    uint8_t numTasks;
    uint8_t currentTask;
};

#endif