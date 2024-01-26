#include <multitasking.h>

// struct CPUState
// {
//     uint32_t eax;
//     uint32_t ebx;
//     uint32_t ecx;
//     uint32_t edx;

//     uint32_t esi;
//     uint32_t edi;
//     uint32_t ebp;

//     /*
//     uint32_t gs;
//     uint32_t fs;
//     uint32_t es;
//     uint32_t ds;
//     */

//     uint32_t error;

//     uint32_t eip;
//     uint32_t cs;
//     uint32_t eflags;
//     uint32_t esp;
//     uint32_t ss;

// } __attribute__ ((packed));



Task::Task(GlobalDescriptorTable* gdt, void entrypoint())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    /*
    cpustate->gs = 0;
    cpustate->fs = 0;
    cpustate->es = 0;
    cpustate->ds = 0;
    */

    cpustate->error = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    cpustate->eflags = 0x202;
    //cpustate->esp;
    //cpustate->ss;

}

Task::~Task() {}

TaskManager::TaskManager() : numTasks(0), currentTask(-1)
{
    
}

TaskManager::~TaskManager()
{

}

bool TaskManager::AddTask(Task* task)
{
    if (numTasks >= 255)
        return false;
    else
    {
        tasks[numTasks++] = task;
        return true;
    }
}

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;

    if (currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;

    if (++currentTask >= numTasks)
        currentTask = 0;

    return tasks[currentTask]->cpustate;
}