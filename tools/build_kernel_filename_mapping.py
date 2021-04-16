import os

def build_filename_mapping(dir_root): 
    index = 1
    mapping_output = "kernel_filename_mapping.txt"
    with open(mapping_output, "w+") as f:
      for dirpath, dirs, files in os.walk(dir_root): 
        for filename in files:
          fname = os.path.join(dirpath, filename)
          outputline = fname[len(dir_root):] + "," + str(index) + "\n"
          print outputline
          f.write(outputline)
          index = index + 1
    f.close()

dir_root = "/home/liyiwen/Documents/projects/tracks/linux-stable"
build_filename_mapping(dir_root)        
