#include <driver.h>

Driver::Driver()
{

}

Driver::~Driver()
{

}

void Driver::Activate() { }
int Driver::Reset() { }
void Driver::Deactivate() { }

DriverMAnager::DriverMAnager() 
    : numDrivers(0)
{ 
    for (uint16_t i = 0; i < 256; i++)
    {
        drivers[i] = nullptr;
    }
}

DriverMAnager::~DriverMAnager() { }

void DriverMAnager::Activate()
{
    for (uint8_t i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
    }
}

void DriverMAnager::AddDriver(Driver* driver)
{
    drivers[numDrivers] = driver;
    numDrivers++;
}