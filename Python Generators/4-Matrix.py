########################################################################################################################
# Written by Jamal Bouajjaj
# The purpose of this program is to generate an array for a 4-matrix LED module to be used with the 
#       SimpleMatrix library. This program allows in interactive GUI for the user to select which LEDs
#       will be on or off per matrix.
########################################################################################################################
# Imports
from tkinter import *
from tkinter import filedialog
from tkinter import messagebox
from PIL import Image
import pyperclip
########################################################################################################################
# Defines and Variables
background_color = "#7f8c8d"        # The software's background color
button_color_T = "#3498db"          # The button color if it's pressed(on)
button_color_F = "#e74c3c"          # The button color if it's unpressed(off)
default_font = ("Verdana", 12)      # Variable to define default font for GUI
title_font = ("Verdana", 14, "bold")    # Variable to define the title's font
ui = None                           # Placeholder for the GUI class
text = ""                           # Placeholder for text to be copied
########################################################################################################################

# Function to determine which button is clicked, change it's color and cooresponding boolean
def buttonClicked(r,c):
    global text
    text = ""
    #print("row = ", str(r))
    #print("col. = ", str(c))
    ui.matrix_array[c] &= ~(1<<(7-r))
    if(ui.buttons_value[r][c] == False):
        ui.matrix_array[c] |= (1<<(7-r))
        ui.buttons[r][c].config(background=button_color_T)
        ui.buttons_value[r][c] = True
    else:
        ui.buttons[r][c].config(background=button_color_F)
        ui.buttons_value[r][c] = False


# Function to generate the array's string to be copied onto the Arduino code
def copyText():
    global text
    text = ""
    text += "uint8_t %s[] = { \n" % ui.array_name_entry.get()
    for i in range(8*4):
        text += bin(ui.matrix_array[i])
        if i != 32-1:
            text += ","
        text += '\n'
    text += "};"
    pyperclip.copy(text)
    print("Array is copied")
    messagebox.showinfo("Copy", "Successfully copied the array")
    
# Function to fill the matrix to all OFF
def clearMatrix():
    for y in range(8):
        for x in range(32):
            ui.buttons[y][x].config(background=button_color_F)
            ui.matrix_array[x] &= ~(1<<(7-y))
            ui.buttons_value[y][x] = False

# Function to fill the matrix to all ON
def fillMatrix():
    for y in range(8):
        for x in range(32):
            ui.buttons[y][x].config(background=button_color_T)
            ui.matrix_array[x] |= (1<<(7-y))
            ui.buttons_value[y][x] = True

# Function to import a 32x8 picture to be displayed
def importPicture():
    file_path = filedialog.askopenfilename(initialdir="")
    try:
        im = Image.open(file_path)
    except:
        print("Please select a proper image format")
        messagebox.showinfo("Import Image", "Please select a proper image format")
        return False
    #print(im.size)
    if im.size != (32, 8):
        print("Image is not the right size (32 by 8 pixels)")
        messagebox.showinfo("Import Image", "Image is not the right size (32 by 8 pixels)")
        return False
    for y in range(8):
        for x in range(32):
            if(im.getpixel((x,y)) != (255,255,255)):
                ui.buttons[y][x].config(background=button_color_T)
                ui.buttons_value[y][x] = True
                ui.matrix_array[x] |= (1<<(7-y))
            else:
                ui.buttons[y][x].config(background=button_color_F)
                ui.buttons_value[y][x] = False
                ui.matrix_array[x] &= ~(1<<(7-y))

class GUI():
    def __init__(self, master):
        self.master = master
        self.matrix_array = []      # The LED matrix array placeholder, which is column-addressed
        for i in range(8*4):        # Fill the array with 0's to append to size (8 column)
            self.matrix_array.append(0)

        self.code_label = None      # The placeholder for the code's text

        self.buttons = []           # Create placeholder for TKinter buttons
        self.buttons_value = []     # Create placeholder for button values
        for i in range(8):          # Append to size (32x8)
            self.buttons.append([])
            self.buttons_value.append([])
            for k in range(8*4):
                self.buttons[i].append(None)
                self.buttons_value[i].append(False)                

        array_name_entry = None     # Placeholder for the array name's entry to copy array
    def generateGUI(self):
        self.master.tk_setPalette(background=background_color)
        Label(self.master, text="MAX7219 4-Matrix Generator", font=title_font).pack()
        Label(self.master, text="By Jamal Bouajjaj", font=default_font).pack()
        self.array_name_entry = Entry(self.master, font = default_font)
        self.array_name_entry.insert(0,"ARRAYNAME")
        self.array_name_entry.pack()

        button_frame = Frame(self.master, width=30*4*8, height=30*8)
        button_frame.pack()
        button_frame.grid_propagate(False) #disables resizing of frame

        img = PhotoImage(width=30*4, height=30)
        for r in range(8):
            button_frame.rowconfigure(r, weight=1)
            for c in range(8*4):
                self.buttons[r][c] = Button(button_frame, background=button_color_F, command=lambda r=r, c=c: buttonClicked(r,c))
                self.buttons[r][c].grid(row=r,column=c, sticky='nesw')
                button_frame.columnconfigure(c, weight=1)
        self.code_label = Label(self.master, text="Started", font = default_font)
        self.code_label.pack()
        Button(self.master, text="Copy Array", command = copyText).pack()
        Button(self.master, text="Import Picture", command = importPicture).pack()
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
