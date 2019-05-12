import os
import sys

kernel_src_path = "/home/detectivelyw/Documents/projects/tracks/linux-stable/"
blacklist_path = "/home/detectivelyw/Documents/projects/tracks/tools/get-basic-blocks/blacklist/nginx-test-fs.boot.log.02"
blacklist = dict()

def load_blacklist(blacklist_file):
  with open(blacklist_file) as f:  
    line = f.readline()
    while line:
      if ("We reached unpopular paths" in line):
        file_name = line.split(":")[1][1:]
        line_number = line.split(":")[2][:-2]
        if file_name not in blacklist:
          lines = set()
          lines.add(line_number)
          blacklist[file_name] = lines
        else:
          blacklist[file_name].add(line_number)
      line = f.readline()
  f.close()
  print blacklist

def remove_blacklist_lines():
  removed_lines_counter = 0
  for file_name in blacklist:
    kernel_src_file = kernel_src_path + file_name
    output_file = kernel_src_file + ".new" 
    line_counter = 0
    with open(output_file, "w+") as fo:  
      with open(kernel_src_file) as fi:
        line = fi.readline()
        line_counter += 1
        while line:
          if str(line_counter) in blacklist[file_name]:
            removed_lines_counter += 1
            if "{ if (kernel_init_done)" in line:
              fo.write("{\n")
          else:
            fo.write(line)
          line = fi.readline()
          line_counter += 1
      fi.close()
    fo.close()
  print "total blacklist lines remove: " + str(removed_lines_counter)

def main():
  load_blacklist(blacklist_path)
  remove_blacklist_lines()

main()
