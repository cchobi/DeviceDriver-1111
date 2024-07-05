#include <stdexcept>

#include "DeviceDriver.h"

using namespace std;

class ReadFailException : public std::exception {
public:
    ReadFailException(const char* message) : msg(message) {}

    virtual const char* what() const noexcept {
        return msg;
    }

private:
    const char* msg;
};


class WriteFailException : public std::exception {
public:
    WriteFailException(const char* message) : msg(message) {}

    virtual const char* what() const noexcept {
        return msg;
    }

private:
    const char* msg;
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly

    bool isSame = true;
    int preResult = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        int nowResult = (int)(m_hardware->read(address));
        if (preResult != nowResult) isSame = false;
        preResult = nowResult;
    }

    if (isSame == false) throw ReadFailException("Different read value !!");

    return preResult;
}

void DeviceDriver::write(long address, int data)
{
    int readValue = (int)(m_hardware->read(address));
    if(readValue != 0xFF) throw WriteFailException("Already written!!");

    m_hardware->write(address, (unsigned char)data);
}