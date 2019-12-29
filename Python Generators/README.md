# Python Generators
The purpose of the Python programs in this folder is to generate an array, which then can be copied and pasted unto an Arduino program. Currently, there are 2 programs available:

- 4-Matrix.py: This software is to generate an array for a 4-matrix MAX7219 controlled LED Matrix (commonly found on Ebay and similar)
- Single Matrix.py: This software is to generate an array for a 4-matrix MAX7219 controlled LED Matrix (commonly found on Ebay and similar)

Both software generate a column-addressed array. That means that each array element is a column, each bit on an array element represents a row on the selected column.

Both softwares require Python3, as well as the following python libraries/packages in order to work:
- Tkinter
- pyperclip
- PIL (Only for 4-Matrix.py)
