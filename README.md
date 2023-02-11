# ImageGarbler
Implementation of prg-exam from [CourseWare](https://cw.fel.cvut.cz/wiki/courses/b0b36prp/resources/exam).
According to videos from prof. Faigl: [link](https://www.youtube.com/playlist?list=PLQ5Wg6tJelytTMT0M7bZbkBuXF8JiQV4O).

#Compiling
Compile the project by running `make`, or manually by running `clang garbler.c save_jpeg.c save_png.c utils.c xwin_sdl.c -o garbler`.

#Usage
The program can be directly used from the console as `./garbler input instructions output [--anim]`,
or alternatively, you can use the garbler.py class in following manner:

```python
from garbler import Garbler
g = Garbler(10000, 500, 510, verbose=True, c_executable="garbler")
	instruction_types = {
		"sw": 1,
		"cp": 0,
		"mh": 0,
		"mv": 0,
		"rl": 0,
		"rr": 0,
		"ic": 0
	}
	g.generate_instructions(instruction_types)
	
  g.garble_image("linalg.png", "garbled.png")
	g.ungarble_image("garbled.png", "ungarbled.jpg")
	
  g.save_garble("garbled.txt")
	g.save_ungarble("ungarbled.txt")
```

The Garbler class is called with following arguments:
`Garbler(num_instructions, width, height, [verbose], [c_executable], [weights])`

Firstly, 'generate_instructions(dictionary)' method needs to be called.

Then, the garbled image, as well as the instructions to its garble and ungarble can be generated with the usage of the compiled project of prg-exam. This is done with the usage of `garble_image(image, garbled)` and `ungarble_image(garbled, ungarbled)`. Then the instructions are saved as 'filename.garble' and 'filename.ungarble'. Then the path to the executable needs to be specified with `c_executable` argument.

Any time, you can just save the garble and ungarble instructions using `save_garble(file)` and `save_ungarble(file)` methods. This does not require the executable.

Currently, only swap instruction is implemented, however all of the other instructions (copy, mirror horizontal, mirror vertical, rotate left, rotate right, invert colors) will be hopefully implemented sometimes.

There is a testing image and a set of ungarble instruction for you to test the implementation of your swap funtion.
