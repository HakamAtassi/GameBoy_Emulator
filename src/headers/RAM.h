#ifndef RAM_H_
#define RAM_H_
#include <vector>
#include <cstdint>
#include <string>


    class RAM{
        private:
            std::vector<uint8_t> memory;    

        public:
            RAM();
            uint8_t read(uint16_t PC);  //reads the value at that exact address 
            void write(uint16_t PC, uint8_t val);   //writes data to address PC. returns nothing
    };


#endif  //namespace chip8VM
