#include <iostream>
#include <unordered_map>
#include <functional>
#include <cstdint>
int main()
{
    std::unordered_map<IORegisters, std::function<Byte(void)>, IORegistersHasher> readCallbacks;
    readCallbacks[IORegisters::PPUCTRL] = []() -> Byte 
    {
        return 0xFF; 
    };
    IORegisters reg = IORegisters::PPUCTRL;
    if (readCallbacks.find(reg) != readCallbacks.end()) 
    {
        Byte value = readCallbacks[reg]();
        std::cout << "Read value: " << std::hex << static_cast<int>(value) << std::endl;
    }
    for( auto& a : readCallbacks)
    {
        std::cout<<static_cast<std::size_t>(a.first)<<std::endl;
    }
    // IORegistersHasher Hash;
    // std::size_t a=Hash(reg);
    // std::cout<<a<<std::endl;
    return 0;
}
