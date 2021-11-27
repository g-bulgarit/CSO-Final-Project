# TODO
> Last updated: 20/11/2021

## Assembler
* Implement file I/O
  * [x] Read ASM file
  * [x] Write to output textfile
* Scan file twice, once to find all loops and once more to generate code
  * [x] Scan once to get all labels
  * [x] On second pass, replace lables with addresses
  * [x] Handle all 3 possibilities for $imm input data types
  * [x] Handle `.word` instruction


## Tests
* [x] Compare our final assembler output against the fibbonaci example
* [ ] Try edge cases defined in the PDF (long lines...)
______________________
___________________

# MIPS Simulator
## General
* [x] Handle basic arithmetic and bitwise instructions
* [ ] Handle `out` and `.word` instructions.

## Debug
* [x] Implement trace dump function (needed for project, but also helpful for debugging purposes)
* [x] Add command and memory dump prints if `DEBUG` is defined

## Fetch
* [x] Read file contents from `imemin.txt` and `dmemin.txt`.
## Decode
* [x] Decode opcodes via lookup table

## Execute
* [ ] Implement all MIPS instructions

## Tests
TBD
