########################################################################################################################
# Written by Jamal Bouajjaj
# The purpose of this program is to generate an array for a single matrix to be used with the SimpleMatrix library.
#       This program allows in interactive GUI for the user to select which LEDs will be on or off per matrix.
########################################################################################################################
# Imports
from tkinter import *
from tkinter import messagebox
import pyperclip
########################################################################################################################
# Defines and Variables
background_color = "#7f8c8d"        # The software's background color
button_color_T = "#3498db"          # The button color if it's pressed(on)
button_color_F = "#e74c3c"          # The button color if it's unpressed(off)
default_font = ("Verdana", 12)      # Variable to define default font for GUI
title_font = ("Verdana", 14, "bold")    # Variable to define the title's font
text = ""                           # Placeholder for text to be copied
ui = None                           # Placeholder for the GUI class
########################################################################################################################

# Function to determine which button is clicked, change it's color and cooresponding boolean
def buttonClicked(r,c):
    #print("Row = %s. Col= %s. \n" % (str(r), str(c)))
    ui.matrix_array[c] &= ~(1<<(7-r))  # Reset the corresponding bit for the clicked button in the matrix array
    if(ui.buttons_value[r][c] == False):   # If the button value was previously set to false
        ui.matrix_array[c] |= (1<<(7-r))   # Set the corresponding bit for the clicked button in the matrix array
        ui.buttons[r][c].config(background=button_color_T) # Set the button's color to blue
        ui.buttons_value[r][c] = True              # Set the corresponding boolean in the button array to indicate it's on
    else:   
        ui.buttons[r][c].config(background=button_color_F)  # Set the button's color to red
        ui.buttons_value[r][c] = False             # Set the corresponding boolean in the button array to indicate it's off
    generateText()      # Generate the text after each update

# Function to generate the array's string to be copied onto the Arduino code
def generateText(e=None):
    global text
    text = ""
    text += "uint8_t %s[] = {\n" % ui.array_name_entry.get()
    for i in range(8):
        text += bin(ui.matrix_array[i])
        if i != 7:
            text += ","
        text += '\n'
    text += "};"
    ui.code_label.config(text = text)

# Function to copy the array which will be pasted onto the Arduino code
def copyText():
    global text
    generateText()
    pyperclip.copy(text)
    print("Successfully copied the array")
    messagebox.showinfo("Copy", "Successfully copied the array")

# Function to fill the matrix to all OFF
def clearMatrix():
    for y in range(8):
        for x in range(8):
            ui.buttons[y][x].config(background=button_color_F)
            ui.matrix_array[x] &= ~(1<<(7-y))
            ui.buttons_value[y][x] = False
    generateText()

# Function to fill the matrix to all ON
def fillMatrix():
    for y in range(8):
        for x in range(8):
            ui.buttons[y][x].config(background=button_color_T)
            ui.matrix_array[x] |= (1<<(7-y))
            ui.buttons_value[y][x] = True
    generateText()

# The GUI class
class GUI():
    def __init__(self, master):
        self.master = master
        self.matrix_array = []   # The LED matrix array placeholder, which is column-addressed
        for i in range(8):  # Fill the array with 0's to append to size (8 column)
            self.matrix_array.append(0)
            
        self.code_label = None   # The placeholder for the code's text

        self.buttons = []        # Create placeholder for TKinter buttons
        self.buttons_value = []  # Create placeholder for button values
        # Append to size (8x8)
        for i in range(8):
            self.buttons.append([])
            self.buttons_value.append([])
            for k in range(8):
                self.buttons[i].append(None)
                self.buttons_value[i].append(False)   
              
        self.array_name_entry = None             # Placeholder for the array name's entry to copy array
    # Function for creating the GUI
    def generateGUI(self):
        self.master.tk_setPalette(background=background_color)
        Label(self.master, text="MAX7219 Single Matrix Generator", font=title_font).pack()
        Label(self.master, text="By Jamal Bouajjaj", font=default_font).pack()
        self.array_name_entry = Entry(self.master, font = default_font, justify='center', background="white")
        self.array_name_entry.insert(0,"ARRAYNAME")
        self.array_name_entry.pack()

        button_frame = Frame(self.master, width=350, height=350)
        button_frame.pack()
        button_frame.grid_propagate(False)      # Disables resizing of frame

        for r in range(8):
            button_frame.rowconfigure(r, weight=1)
            for c in range(8):
                self.buttons[r][c] = Button(button_frame, background=button_color_F, command=lambda r=r, c=c: buttonClicked(r,c))
                self.buttons[r][c].grid(row=r,column=c, sticky='nesw')
                button_frame.columnconfigure(c, weight=1)
        self.code_label = Label(self.master, text="Started", font = default_font)
        self.code_label.pack()
        Button(self.master, text="Copy Array", command = copyText).pack()
        Button(self.master, text="Clear Display", command = clearMatrix).pack()
        Button(self.master, text="Fill Display", command = fillMatrix).pack()

########################################################################################################################
# Start of main program
try:
    root = Tk()
    ui = GUI(root)
    ui.generateGUI()
    root.mainloop()
except Exception as e:
    print(e)
    print("Error")
    input("Press any key to exit program")
