#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "port.h"
#include "types.h"
#include "interrupts.h"
#include "driver.h"

class KeyboardEventHandler
{
    public:
    KeyboardEventHandler();

    virtual void OnKeyDown(char) { }
    virtual void OnKeyUp(char) { }
};

class KeyboardDriver : public InterruptHandler, public Driver 
{
public:
    KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler);
    ~KeyboardDriver();

    uint32_t HandleInterrupt(uint32_t esp);
    void Activate();

private:
    Port8Bit dataPort;
    Port8Bit commandPort;

    KeyboardEventHandler* handler;
};

#endif