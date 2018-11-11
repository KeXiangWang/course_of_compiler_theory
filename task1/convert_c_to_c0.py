import os
import sys

def get_c_file(filepath, file_name = "test_original_C"):
	c_file_list = []
	files = os.listdir(filepath)
	for fi in files:
		if(fi.find(file_name)==0 and (fi[-1]=='c' or fi[-1]=='C')):
			print("Got: ", fi)
			c_file_list.append(fi)
	return c_file_list

def rewrite(filepath, filename, file_name = "test_original_C", new_file_name = "test_c0"):
	dot_ori = filename.rfind('.')
	extra = filename[len(file_name): dot_ori]
	new_file = os.path.join(filepath,new_file_name+extra+".txt")
	with open(filename, 'r')  as f:
		target_f = open(new_file, 'w')
		lines = f.readlines()
		lines.append("")
		line2 = zip(lines[0:-1], lines[1:])
		for line, next_line in line2:
			if(len(line) >= 4 and (line.strip()[0:3] == "// " or line.strip()[0] == '#')):
				continue
			if(len(next_line) >= 4 and next_line.strip()[0:3] == "// "):
				new_next_line = next_line.replace("// ", "")
				target_f.write(new_next_line)
				continue
			target_f.write(line)

def main():
	if(len(sys.argv)==1):
		c_files = get_c_file(os.getcwd())
		for file in c_files:
			rewrite(os.getcwd(), file)
	elif(len(sys.argv)==2):
		c_files = get_c_file(os.getcwd(), sys.argv[1])
		for file in c_files:
			rewrite(os.getcwd(), file, sys.argv[1])
	else:
		c_files = get_c_file(os.getcwd(), sys.argv[1])
		for file in c_files:
			rewrite(os.getcwd(), file, sys.argv[1], sys.argv[2])

main()