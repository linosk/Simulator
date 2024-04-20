#include <unordered_map>
#include <iostream>
#include <unistd.h>

class I2CDevice{

public:
    int devAddress;
    int commSpeed;
};

class SharedBus{

public:
    bool sdaState;
    bool sclState;

    void listDevices(void){
        if(devices.empty())
            std::cout<<"No devices.\n";
        else{ //diff between auto& and auto
            for(const auto device : devices){
                std::cout<<"--------------------\n";
                std::cout<<"Device address: "<<device.first<<"\n";
                std::cout<<"Communication speed: "<<device.second.commSpeed<<"\n";
            }
            std::cout<<"--------------------\n";
        }

    }

    void addDevice(I2CDevice device){
        devices.insert({device.devAddress,device});
    }

    void removeDevice(I2CDevice device){
        devices.erase(device.devAddress);
    }

    void clockSignal(int numStates, const int stateDuration = 0){
        while(numStates){ //concept is correct but it hast to be modifeid for the project
            if(numStates%2==0)
                std::cout<<"0";
            else
                std::cout<<"1";
            sleep(stateDuration); //it does not work as itented
            numStates--;
        }
    }

private:
    std::unordered_map<int, I2CDevice> devices;
};

int main(void){

    SharedBus bus;

    // bus.listDevices();
// 
    // I2CDevice d0;
    // d0.devAddress = 0x10;
    // d0.commSpeed = 340;
// 
    // I2CDevice d1;
    // d1.devAddress = 0x20;
    // d1.commSpeed = 340;
// 
    // bus.addDevice(d0);
    // bus.addDevice(d1);
// 
    // bus.listDevices();
// 
    // bus.removeDevice(d0);
// 
    // bus.listDevices();

    bus.clockSignal(10,1);

    return 0;
}