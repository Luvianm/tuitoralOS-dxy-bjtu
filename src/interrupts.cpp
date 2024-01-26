#include <interrupts.h>


void printf(const int8_t*);
void printfhex(const uint8_t);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager)
    : interruptNumber(interruptNumber), interruptManager(interruptManager) 
{
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler()
{
    if (interruptManager->handlers[interruptNumber] == this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt, TaskManager* taskManager)
    : picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1), taskManager(taskManager)
{
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;  // interrupt gate's flag

    for (uint16_t i = 0; i < 256; i++)
    {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

    // 初始化可编程控制器
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);
    // 中断向量开始地址
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);
    // 输出数据
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picMasterData.Write(0x01);
    // 中断屏蔽码0闭1开
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptsDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    __asm__ volatile("lidt %0": :"m" (idt));
}

InterruptManager::~InterruptManager()
{

}

void InterruptManager::Activate()
{
    if (ActiveInterruptManager != nullptr)
    {
        ActiveInterruptManager->Deactivate();
    }
    ActiveInterruptManager = this;

    __asm__ volatile("sti");
}

void InterruptManager::Deactivate()
{
    if (ActiveInterruptManager != this)
    {
        ActiveInterruptManager = 0;
        __asm__ volatile("cli");
    }
    
}

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,            // 中断号
    uint16_t codeSegmentSelectorOffset, // 段选择符偏移
    void (*handler)(),                  // interrupt处理函数
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
    )
{
    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | (DescriptorPrivilegeLevel & 3 << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    // printf(" interrupt");

    if (ActiveInterruptManager != 0)
    {
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    

    // Port8BitSlow command(0x20);
    // command.Write(0x20);

    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if (handlers[interruptNumber] != nullptr)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if (interruptNumber != 0x20)
    {
        char msg[] = " unhandled interrupt 0x00";
        // const char* hex = "0123456789ABCDEF";
        // msg[23] = hex[(interruptNumber >> 4) & 0xF];
        // msg[24] = hex[interruptNumber & 0xF];

        // printf(msg);
        printfhex(interruptNumber);

        // Port8Bit dataPort(0x60);
        // dataPort.Read();

    }

    if (interruptNumber == 0x20)
        esp = (uint32_t)taskManager->Schedule((CPUState*)esp);

    if (0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        picMasterCommand.Write(0x20);
        if (0x28 <= interruptNumber)
        {
            picSlaveCommand.Write(0x20);
        }
    }

    return esp;
}