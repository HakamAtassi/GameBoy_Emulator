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


### Tetris 
<p align="center">
 <img src="https://user-images.githubusercontent.com/83780720/188169021-ccd7ecc3-8c05-4f23-9535-998e6dc95af9.png"/>
</p>


### Printing vRam 

<p align="center">
 <img src="https://user-images.githubusercontent.com/83780720/187287426-22f4fe4a-7963-41bf-8f49-ada1544be4da.png"/>
</p>

### Printing test results from verified mem dump 

<p align="center">
 <img src="https://user-images.githubusercontent.com/83780720/187350775-10a86cf9-fa8c-4614-b9bc-c4f564c1a6bd.png"/>
</p>
