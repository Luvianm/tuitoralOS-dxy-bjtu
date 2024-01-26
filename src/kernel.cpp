#include <types.h>
#include <gdt.h>
#include <interrupts.h>
#include <keyboard.h>
#include <driver.h>
#include <mouse.h>

void printf(const int8_t* str)
{
    //sreen address
    static int16_t* VideoMemory = (int16_t *)0xb8000;  //first byte: fore/back colour; second byte: constant
    static int8_t x = 0, y = 0;  //apperance position

    for (int32_t i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            ++y;
            x = 0;
            break;
        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            //two bytes: 1.colours(0xFF00 black screen white words ) 2.constant
            ++x;
            break;
        }
        if(x >= 80){
            ++y;
            x=0;
        }
        if(y >= 25){
            for (y = 0; y < 25; ++y) {
                for (x = 0; x < 80; x++) {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                }
            }
            x = 0;
            y = 0;
        }
    }
}

void printfhex(const uint8_t num)
{
    uint8_t c = num;
    static char msg[3] = {"0"};
    const char * hex = "0123456789ABCDEF";
    msg[0] = hex[(c >> 4) & 0x0F];
    msg[1] = hex[c & 0x0F];
    msg[2] = '\0';
    printf(msg);
    //printf(" ");
}

void taskA()
{
    while (true)
    {
        printf("A");
    }
}

void taskB()
{
    while (true)
    {
        printf("B");
    }
}

class PrintfKeyboardEventHandler: public KeyboardEventHandler
{
    public:
    void OnKeyDown(char c)
    {
        char msg[2] = {"\0"};
        msg[0] = c;
        printf(msg);
    }
};

class MouseToConsole: public MouseEventHandler
{
    int8_t x, y;
    static uint16_t* VideoMemory;
public:
    MouseToConsole() : x(40), y(12)
    {
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                                  ((VideoMemory[80 * y + x] & 0x0F00) << 4) |
                                  (VideoMemory[80 * y + x] & 0x00FF);
        
    }

    void OnMouseMove(int16_t xoffset, int16_t yoffset)
    {
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                                  ((VideoMemory[80 * y + x] & 0x0F00) << 4) |
                                  (VideoMemory[80 * y + x] & 0x00FF);
        x += xoffset;
        if (x < 0) x = 0;
        if (x >= 80) x = 79;
        y += yoffset;
        if (y < 0) y = 0;
        if (y >= 25) y = 24;

        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                                  ((VideoMemory[80 * y + x] & 0x0F00) << 4) |
                                  (VideoMemory[80 * y + x] & 0x00FF); 
    }

    void OnMouseDown(uint8_t button)
    {
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                                  ((VideoMemory[80 * y + x] & 0x0F00) << 4) |
                                  (VideoMemory[80 * y + x] & 0x00FF);
    }

    void OnMouseUp(uint8_t button)
    {
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | 
                                  ((VideoMemory[80 * y + x] & 0x0F00) << 4) |
                                  (VideoMemory[80 * y + x] & 0x00FF);
    }

};

uint16_t* MouseToConsole::VideoMemory = (uint16_t*)0xB8000;

typedef void (*constructor)();
extern constructor start_ctors;
extern constructor end_ctors;

extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; ++i)
        (*i)();
}

extern "C" void kernelMain(void* multiboot_structrue, int32_t magicnumber)
{
    GlobalDescriptorTable gdt;
    TaskManager taskManager;

    printf("Hello, My OSworld!\n");
    printf("Hello, MY OSworld!\n");

    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    //taskManager.AddTask(&task1);
    //taskManager.AddTask(&task2);

    InterruptManager interrupts(&gdt, &taskManager);

    DriverMAnager driverManager;

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);


    driverManager.AddDriver(&keyboard);
    driverManager.AddDriver(&mouse);
    //keyboard.Activate();
    driverManager.Activate();

    interrupts.Activate();

    while (1);
}