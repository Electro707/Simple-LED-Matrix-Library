#!/usr/bin/env python
"""
Application that calls a test application (which directly invokes the SimpleLEDMatrix library)
and handles receiving frame buffers per display update by storing them as images, then
the images are merged together as an .GIF file.

This is useful for documenting what each function does, and what the examples do.
"""
from xml.etree import ElementTree
from copy import deepcopy
from ctypes import cdll
import ctypes
from cairosvg import svg2png
from PIL import Image
import glob
import os

f_name = 1
delay_arr = []
image_folder = 'gen_imgs'
make_folder = 'build/'
output_folder = 'out'

@ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.POINTER(ctypes.c_char), ctypes.c_int)
def got_display(a, delay):
    # Everytime this function is called, the LED matrix is beign updated.
    # Use this to create an image of the LED matrix
    
    global f_name
    document = ElementTree.parse('led_matrix.svg')
    root = document.getroot()
    led_bodies = [root.find(".//*[@id='led0_0']")]
    body = root.find(".//*[@id='body1']")
    
    for d in range(4):
        if d != 0:
            el = deepcopy(body)
            del el.attrib['id']
            el.attrib["x"] = str(d*32)
            root.append(el)
        for r in range(8):
            for c in range(8):
                if r==0 and c==0 and d==0:
                    el = led_bodies[0]
                else:
                    el = deepcopy(led_bodies[0])
                    el.attrib["id"] = 'led{:d}_{:d}'.format(r, c)
                    el.attrib["cy"] = str(30 - (r*4))
                    el.attrib["cx"] = str(2 + (c*4) + (d*8*4))

                if ord(a[(d*8)+r]) & (1 << c) != 0:
                    el.attrib["style"] = el.attrib["style"].replace("fefefe", "ff0000")

                if not(r==0 and c==0 and d==0):
                    led_bodies.append(el)
    
    # Add the LEDs after all is done so they appear on top
    for l in led_bodies:
        root.append(l)
    svg2png(bytestring=ElementTree.tostring(root, encoding='utf8', method='xml'),write_to=f'{image_folder}/led_natrix_{f_name:03d}.png', dpi=300)
    delay_arr.append(float(delay))
    print(f"Done with frame {f_name}")
    f_name += 1

if not os.path.isdir(f"{image_folder}"):
    os.mkdir(f"{image_folder}")
if not os.path.isdir(f"{output_folder}"):
    os.mkdir(f"{output_folder}")

# Load the test dll
lib = cdll.LoadLibrary(f'./{make_folder}libtest.so')
lib.test_f(got_display)
# Create the frames
frames = []
imgs = sorted(glob.glob(f"{image_folder}/*.png"))
for i in imgs:
    new_frame = Image.open(i)
    frames.append(new_frame)

frames[0].save(f'{output_folder}/png_to_gif.gif', optimize=True,
               append_images=frames[1:],
               save_all=True,
               duration=delay_arr, loop=0)
