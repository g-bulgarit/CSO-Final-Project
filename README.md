# TODO
> Last updated: 20/11/2021

## General
* [ ] Consider directory refactor (+ compile target change), to reduce clutter

## Assembler
* Implement file I/O
  * [x] Read ASM file
  * [x] Write to output textfile
* Scan file twice, once to find all loops and once more to generate code
  * [x] Scan once to get all labels
  * [x] On second pass, replace lables with addresses
  * [x] Handle all 3 possibilities for $imm input data types
  * [ ] Handle `.word` instruction


## Tests
* [ ] Compare our final assembler output against the fibbonaci example given (after loop handling is finished)
  * Most lines pass, some lines fail - possibly a mistake in our bit-arithmetics.
* [ ] Try edge cases defined in the PDF (long lines...)
______________________
___________________

# MIPS Simulator
## General
* [ ] Handle basic arithmetic and bitwise instructions
* [ ] Handle `out` and `.word` instructions.

## Debug
* [ ] Implement trace dump function (needed for project, but also helpful for debugging purposes)

## Fetch
* [ ] Read file contents from `imemin.txt` and `dmemin.txt`.
## Decode
* [x] Decode opcodes via lookup table

## Execute
* [ ] Implement all MIPS instructions

## Tests
TBD