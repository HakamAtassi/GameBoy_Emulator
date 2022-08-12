# GameBoy_Emulator

### Interrupts
The CPU can request an interrupt by setting flag(s) at 0xFF0F. The CPU can also toggle the Master interrupt switch with DI and EI opcodes. Therefore, the IME is a member of GameBoy that is passed to CPU as a pointer (and deleted in the GameBoy desctructor). 
Interrupts work like this: 

while running: 
  execute instruction
  check master enable
  check enable for individial interrupts
  check flag for interrupt
    if set, run ISR for instruction
