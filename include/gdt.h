#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"

class GlobalDescriptorTable
{
public:
    class SegmentDescriptor
    {
    private:
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_high;
        uint8_t type;
        uint8_t flags_limit_hi;
        uint8_t base_vhi;
    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        uint32_t Base();
        uint32_t Limit();
    } __attribute__((packed));  // don't to be automaticly aligned

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    // for getting the position of code segment in the GDT; the position is the relative lean to the primary
    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();

};

#endif