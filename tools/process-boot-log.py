import os
import sys

blacklist = dict()

def read_log(log_file):
  with open(log_file) as f:  
    line = f.readline()
    while line:
      if ("We reached unpopular paths" in line):
        try:
          line = line.split("]")[1]
          file_name = line.split(":")[1][1:]
          line_number = line.split(":")[2]
        except:
          line = f.readline()
          continue        

        if file_name not in blacklist:
          lines = set()
          lines.add(line_number)
          blacklist[file_name] = lines
        else:
          blacklist[file_name].add(line_number)

      line = f.readline()
  f.close()

def scan_logs(log_path): 
  for dirpath, dirs, files in os.walk(log_path): 
    for filename in files:
      fname = os.path.join(dirpath, filename)
      read_log(fname)

def write_blacklist(output_path):
  with open(output_path, "w+") as fo:
    for item in blacklist:
      for num in blacklist[item]:
        outputline = ""
        outputline += item + "," + num[:-2] + "\n"
        fo.write(outputline)     
  fo.close()

def main():
  log_path = "/home/liyiwen/Documents/projects/tracks/tools/boot-log/"
  output_path = "/home/liyiwen/Documents/projects/tracks/tools/blacklist/blacklist.txt"
  scan_logs(log_path)
  print blacklist
  write_blacklist(output_path)

main()

