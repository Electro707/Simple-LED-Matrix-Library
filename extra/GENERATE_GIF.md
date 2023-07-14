# Generating GIF images
If you would like to generate GIF images of the display from C code, head over to the [build_gif folder](build_gif).

## I just want an example
Run `build.sh trainn`. Wait for the output to finish, and check the `out` folder, where you should see a GIF of a train.

## Custom Program
C a C program either in the base directory or in the `programs` folder (or technically anywhere). It is highly recommended to copy [_ctemplate.cpp](_ctemplate.cpp) as your base template as it has some boiler plate stuff.

Then call `build.sh` with the following arguments:
- Argument 1: The function name that is the program entry point
- Argument 2: Optional, the file to run

Argument 2 is option and will default to [docs_prog.cpp](docs_prog.cpp). This is because it is used when generating GIFs for the documentation.

## Inner Workings
This whole magic works by having a Python script call the "library's entry point" (which is really your application's entry point). Python will pass it's own function callback to the C function. This callback gets called everytime the Simple LED Matrix library generates a new frame. Python will take this frame and generate images in the `gen_imgs` directory. After all frames are done (i.e the C function exits), Python will take all images in that directly and compile it into a GIF.