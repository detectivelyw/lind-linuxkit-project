import os

kernel_filename_mapping = dict()
dir_root = "/home/liyiwen/Documents/projects/tracks/linux-stable"

def load_kernel_filename_mapping(data):
  with open(data) as f:
    line = f.readline()
    while line:
      filename = line.split(",")[0]
      filenum = line.split(",")[1][:-1]
      kernel_filename_mapping[filename] = filenum
      line = f.readline()
  f.close()

def replace_printk_with_hypercall(dir_root):
  counter = 0
  for dirpath, dirs, files in os.walk(dir_root):
    for filename in files:
      fname = os.path.join(dirpath, filename)
      if ".new" in fname:
        continue
      with open(fname) as fin:
        line = fin.readline()
        if "extern int kernel_init_done;" not in line: 
          fin.close()
          continue
        else:
          output_fname = fname + ".new"
          with open(output_fname, "w+") as fout:
            fout.write(line)
            line = fin.readline()
            fout.write("long sys_my_hypercall(unsigned long file_name, unsigned long line_num);\n")
            line = fin.readline()
            while line:
              if "We reached unpopular paths" in line:
                newline = line.replace("printk(\"We reached unpopular paths: %s:%i\\n\", __FILE__, __LINE__);","sys_my_hypercall((unsigned long) " + kernel_filename_mapping[fname[len(dir_root):]] + ", (unsigned long) __LINE__);") 
                line = newline
                print line
                counter = counter + 1
              fout.write(line)
              line = fin.readline()
          fout.close()
          fin.close()
  print "Total lines replaced: " + str(counter)

data = "kernel_filename_mapping.txt"
load_kernel_filename_mapping(data)
replace_printk_with_hypercall(dir_root)

