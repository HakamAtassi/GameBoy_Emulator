# GameBoy_Emulator

### Compile and run 
```
g++ <file>.cpp ../../src/RAM.cpp ../../src/Debugger.cpp ../../src/CPU.cpp ../../src/PPU.cpp ../../src/Cartridge.cpp ../../src/GameBoy.cpp ../../src/Timers.cpp ../../src/InterruptHandler.cpp ../../src/Instructions.cpp `pkg-config --cflags --libs sdl2`
```

### Useful resources: 

* PPU: https://hacktix.github.io/GBEDG/ppu/
* CPU: https://gbdev.io/pandocs/CPU_Registers_and_Flags.html and https://meganesulli.com/generate-gb-opcodes/
  *  The megane sulli website has a few innacuracies
* Timers: http://www.codeslinger.co.uk/pages/projects/gameboy/timers.html

