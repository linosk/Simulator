#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <thread>

bool isThereMaster = false; //not sure if this is the best or even good approach, should be part of SharedeBus

class I2CDevice{

public:
    int devAddress;
    int commSpeed;
    bool isMaster = false;

    I2CDevice(int devAddress, int commSpeed){
        this->devAddress = devAddress;
        this->commSpeed = commSpeed;
    }

    void becomeMaster(void){ //friend function
        if(!isThereMaster){
            isMaster = true;
            isThereMaster = true;
        }
        else
            std::cout<<"Device 0x"<<std::hex<<this->devAddress<<" cannot become master. Master already exists.\n";
    }
};

class SharedBus{

public:
    uint8_t sdaState:1;
    uint8_t sclState:1;

    void listDevices(void){
        if(devices.empty())
            std::cout<<"No devices.\n";
        else{ //diff between auto& and auto
            for(const auto device : devices){
                std::cout<<"--------------------\n";
                std::cout<<"Device address: 0x"<<std::hex<<device.first<<"\n";
                std::cout<<"Communication speed: "<<std::dec<<device.second.commSpeed<<"\n"; //seconds? miliseconds?
            }
            std::cout<<"--------------------\n";
        }

    }

    void addDevice(I2CDevice device){ //it all should be logged
        auto it = devices.find(device.devAddress);
        if(it != devices.end()){
            std::cout<<"Device 0x"<<std::hex<<device.devAddress<<" already added.\n";
        }
        else{
            if(devices.insert({device.devAddress,device}).second)
                std::cout<<"Device 0x"<<std::hex<<device.devAddress<<" succesfully added.\n";
            else
                std::cout<<"Unable to add device 0x"<<std::hex<<device.devAddress<<".\n";
        }
    }

    void removeDevice(I2CDevice device){ //possibilty of removing by another device with the same address
        auto it = devices.find(device.devAddress);
        if(it != devices.end()){
            if(devices.erase(device.devAddress))
                std::cout<<"Device 0x"<<std::hex<<device.devAddress<<" succesfully removed.\n";
            else
                std::cout<<"Unable to remove device 0x"<<std::hex<<device.devAddress<<".\n";
        }
        else{
            std::cout<<"Device 0x"<<std::hex<<device.devAddress<<" not added. Nothing to remove.\n";
        }
    }

    void clockSignal(const int numStates = 0, const float stateDurationInSec = 0){
        int count = numStates; //needs to be modified
        int time = 1000*stateDurationInSec;

        while(count){
            std::cout<<'1'<<std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
            std::cout<<'0'<<std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
            count--;
        }
        std::cout<<'1'<<std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

private:
    std::unordered_map<int, I2CDevice> devices;
};

int main(void){

    SharedBus bus;

    I2CDevice d0(0x10,300);
    I2CDevice d1(0x10,340);

    bus.addDevice(d0);
    bus.addDevice(d0);
    //bus.addDevice(d1);
    //bus.removeDevice(d0);
    bus.removeDevice(d0);
    bus.removeDevice(d0);
    bus.listDevices();

    d0.becomeMaster();
    d1.becomeMaster();

    //bus.clockSignal();

    return 0;
}