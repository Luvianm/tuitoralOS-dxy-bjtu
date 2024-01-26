#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "types.h"
#include "port.h"
#include "gdt.h"
#include "multitasking.h"

class InterruptManager;

class InterruptHandler
{
protected:
    uint8_t interruptNumber;
    InterruptManager* interruptManager;
public:
    InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
    ~InterruptHandler();

    virtual uint32_t HandleInterrupt(uint32_t esp);
};


class InterruptManager
{
    friend class InterruptHandler;
protected:
    InterruptHandler* handlers[256];
    TaskManager* taskManager;
    static InterruptManager* ActiveInterruptManager;

    struct GateDescriptor  // gate description character
    {
        uint16_t handlerAddressLowBits;  // 2 bytes interrupt address
        uint16_t gdt_codeSegmentSelector;//段选择子
        uint8_t reserved;  // reserved 1 byte
        uint8_t access;
        uint16_t handlerAddressHighBits;  // 2 bytes high address

    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];  //IDT table

    struct InterruptsDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;

    } __attribute__((packed));

    static void SetInterruptDescriptorTableEntry(  // initialize IDT table func
        uint8_t InterruptManager,//中断号
        uint16_t codeSegmentSelectorOffset,//段选择符偏移
        void (*handler)(),//interrupt处理函数
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );

    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

public:

    InterruptManager(GlobalDescriptorTable* gdt, TaskManager* taskManager);
    ~InterruptManager();

    void Activate();
    void Deactivate();

    static uint32_t handleInterrupt(uint8_t Interrupt,uint32_t esp);

    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);
  
    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x0C();
};

#endif