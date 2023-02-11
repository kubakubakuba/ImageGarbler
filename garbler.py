from random import randint
import os

class Garbler:
	def __init__(self, num_instructions, width, height, verbose=False, c_executable=None, weights={"sw": 3,"cp": 3,"mh": 3,"mv": 3, "rl": 3, "rr": 3, "ic": 3}):
		self.num_instructions = num_instructions
		self.width = width
		self.height = height
		self.verbose = verbose
		self.c_executable = c_executable
		self.data = []
		self.weights = weights

	def generate_instructions(self, instructions):
		if self.verbose:
			print(f"Generating new instruction file with ~{self.num_instructions} commands, using weights ")
		num_generated = 0
		while num_generated < self.num_instructions:
			if instructions["sw"] and randint(0, self.weights["sw"]):
				self.generate_swap()
				num_generated += 1
			
			if instructions["cp"] and randint(0, self.weights["cp"]):
				self.generate_copy()
				num_generated += 1
			
			if instructions["mv"] and randint(0, self.weights["mv"]):
				self.generate_mirrorV()
				num_generated += 1
			
			if instructions["mh"] and randint(0, self.weights["mh"]):
				self.generate_mirrorH()
				num_generated += 1

			if instructions["rl"] and randint(0, self.weights["rl"]):
				self.generate_rotateLeft()
				num_generated += 1

			if instructions["rr"] and randint(0, self.weights["rr"]):
				self.generate_rotateRight()
				num_generated += 1

			if instructions["ic"] and randint(0, self.weights["ic"]):
				self.generate_invertColors()
				num_generated += 1

	def generate_swap(self):
		generate_ok = 0
		while not generate_ok:		
			start_x = randint(0, self.width - 1)
			start_y = randint(0, self.height - 1)

			new_w = randint(1, self.width - start_x)
			new_h = randint(1, self.height - start_y)
			
			if(start_x + new_w + 1 >= self.width - new_w or start_y + new_h + 1 >= self.height - new_h):
				continue

			end_x = randint(start_x + new_w + 1, self.width - new_w)
			end_y = randint(start_y + new_h + 1, self.height - new_h)

			instruction = {
				"code": "sw",
				"sx": start_x,
				"sy": start_y,
				"w":  new_w,
				"h":  new_h,
				"dx": end_x,
				"dy": end_y
			}

			if self.verbose:
				print(f"Generating new swap: {instruction}")

			self.data.append(instruction)
			generate_ok = 1

	def generate_copy(self):
		raise NotImplementedError

	def generate_mirrorV(self):
		raise NotImplementedError

	def generate_mirrorH(self):
		raise NotImplementedError

	def generate_rotateLeft(self):
		raise NotImplementedError

	def generate_rotateRight(self):
		raise NotImplementedError
	
	def generate_invertColors(self):
		raise NotImplementedError
	
	def save_garble(self, filename):
		if self.verbose:
			print(f"Saving garbling commands into file {filename}")
		
		with open(filename, 'w') as f:
			for entry in self.data:
				f.write(f"{entry['code']} {entry['sx']} {entry['sy']} {entry['w']} {entry['h']} {entry['dx']} {entry['dy']}\n")

	def save_ungarble(self, filename):
		if self.verbose:
			print(f"Saving ungarbling commands into file {filename}")
		
		with open(filename, 'w') as f:
			for entry in self.data[::-1]:
				f.write(f"{entry['code']} {entry['sx']} {entry['sy']} {entry['w']} {entry['h']} {entry['dx']} {entry['dy']}\n")

	def garble_image(self, input_image, output_image, animate=False):
		if self.c_executable == None:
			raise ValueError

		self.save_garble(f"{input_image}.garble")

		if self.verbose:
			print(f"Garbling image {input_image} and saving it as {output_image}")

		if animate == True:
			os.system(f"./{self.c_executable} {input_image} {input_image}.garble {output_image} --anim")
		else:
			os.system(f"./{self.c_executable} {input_image} {input_image}.garble {output_image}")

	def ungarble_image(self, input_image, output_image, animate=False):
		if self.c_executable == None:
			raise ValueError

		self.save_ungarble(f"{input_image}.ungarble")

		if self.verbose:
			print(f"Ungarbling image {input_image} and saving it as {output_image}")

		if animate == True:
			os.system(f"./{self.c_executable} {input_image} {input_image}.ungarble {output_image} --anim")
		else:
			os.system(f"./{self.c_executable} {input_image} {input_image}.ungarble {output_image}")
