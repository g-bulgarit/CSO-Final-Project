# TODO
> Last updated: 17/12/2021

# General
* [ ] Write required `asm` test programs:
    * [ ] `mulmat.asm`
    * [ ] `binom.asm`
    * [ ] `circle.asm`
    * [ ] `disktest.asm`


* [ ] For each test program, copy the project and run it - we need to submit this input together with the project files.

___________________

# MIPS Simulator - In Progress
## General
* [x] Handle basic arithmetic and bitwise instructions
* [x] Handle `in`, `out`, `reti` and `.word` instructions.

## IO
* [x] Implement monitor
* [x] Implement 7-segment display
* [x] Implement LED array
* [ ] Implement disk read and write
* [x] Load disk content on MIPS initialization
* [ ] Save disk content to file on HALT

## Interrupts
* [x] Implement general interrupt logic
* [ ] Implement timer interrupt
* [ ] Implement disk IO interrupt
* [x] Implement IRQ2 interrupt

## Debug
* [x] Implement trace dump function (needed for project, but also helpful for debugging purposes)
* [x] Add command and memory dump prints if `DEBUG` is defined

## Fetch
* [x] Read file contents from `imemin.txt` and `dmemin.txt`.
## Decode
* [x] Decode opcodes via lookup table

## Tests
* [x] Compare our final assembler output against the fibbonaci example
* [x] Try edge cases defined in the PDF (long lines...)

## Execute
* [x] Implement all MIPS instructions

## Output
* [ ] Generate all required output files...

## Tests
* [ ] Write tests for all HW components (in `asm`) and actually test their functionality.

_________________________

# Assembler - Done
* Implement file I/O
  * [x] Read ASM file
  * [x] Write to output textfile
* Scan file twice, once to find all loops and once more to generate code
  * [x] Scan once to get all labels
  * [x] On second pass, replace lables with addresses
  * [x] Handle all 3 possibilities for $imm input data types
  * [x] Handle `.word` instruction
