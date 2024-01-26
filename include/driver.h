#ifndef __DERIVER_H__
#define __DERIVER_H__

#include "types.h"

class Driver
{
public:
    Driver();
    ~Driver();

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};

class DriverMAnager
{
public:
    DriverMAnager();
    ~DriverMAnager();

    void Activate();
    void AddDriver(Driver* driver);
private:
    Driver* drivers[256];
    int numDrivers;

};


#endif