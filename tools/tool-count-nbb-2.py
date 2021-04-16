# usage: python tool.py [kernel-subdirectory]
# example: python tool.py fs

import os
import sys

total_pp_data_path = "total-pp.txt"
pp_files = dict()
blacklist = dict()
multiple_line_for = 0
macro_definition_counter = 0
macro_definition_output = 0

# decide if we are entering the middle of one statement
def is_line_incomplete(line):
  global multiple_line_for
  if (multiple_line_for > 0):
    if (line.count("(") > line.count(")")):
      multiple_line_for += line.count("(") - line.count(")")
    if (line.count("(") < line.count(")")):
      multiple_line_for -= line.count(")") - line.count("(")
      if (multiple_line_for == 0):
        return False
      else:
        return True
    else:
      return True
  
  if ((";" not in line) and ("{" not in line) and ("}" not in line) and (line != "\n")):
    if (line.count("(") != line.count(")")):
      if ("for (" in line):
        multiple_line_for += line.count("(") - line.count(")") 
      return True
    elif (("if (" in line) and (line.count("(") > line.count(")"))):  
      return True
    elif (("for (" in line) and (line.count("(") > line.count(")"))):
      return True
    elif (("while (" in line) and (line.count("(") > line.count(")"))):
      return True
    elif ("?" in line): 
      return True
    elif (("if (" not in line) and ("for (" not in line)):
      return True
    else:
      return False
  else: 
    if ("} while (" in line) and (line.count("(") > line.count(")")):
      return True
    if ((";" in line) and ("for (" in line) and (line.count("(") > line.count(")"))):
      multiple_line_for += line.count("(") - line.count(")")
      return True
    else:
      if (";" in line) and (("for (" in line) or ("if (" in line)) and (line.count("(") == line.count(")")):
        return False 
      elif (";" in line) and (line[-2] != ';') and (line[-3] != '*'):
        return True
      elif ("else if (" in line) and ("{" not in line) and (line.count("(") > line.count(")")):
        return True
      else:
        return False

def generate_blacklist():
  blacklist_lines_01 = set()
  line_number = 72
  blacklist_lines_01.add(line_number)
  blacklist["mm/mmap.c"] = blacklist_lines_01

def is_line_in_blacklist(line_number, file_name):
  if file_name not in blacklist:
    return False
  elif line_number in blacklist[file_name]: 
    return True
  else:
    return False

def is_syscall_define(line):
  global macro_definition_counter
  global macro_definition_output
  if ("SYSCALL_DEFINE" in line) or ("BPF_CALL_" in line) or ("module_param_named(" in line) or ("module_param(" in line) or ("module_init(" in line) or ("module_exit(" in line) or ("module_param_array(" in line) or ("STANDARD_PARAM_DEF(" in line) or ("static inline" in line) or ("module_param_array_named(" in line) or ("} else if" in line) or ("PCI_USER_" in line) or ("core_param(" in line) or ("PMU_FORMAT_ATTR(" in line) or ("bool __init" in line) or ("early_param_on_off(" in line):
    if not ("} else if" in line):
      macro_definition_counter += 1 
      macro_definition_output = 1
    return True
  else:
    return False

def is_in_struct(line):
  if (("struct" in line) and ("{" in line)):
    return True
  else:
    return False

def generate_pp_file(kernel_sub_dir): 
  output_pp_file = kernel_sub_dir + "-pp.txt"
  kernel_dir_path = "SF:/home/liyiwen/Documents/projects/tracks/linux-stable/" + kernel_sub_dir  
  with open(total_pp_data_path) as fp:  
    with open(output_pp_file, "w") as f:
      line = fp.readline()
      while line:
        if (line.startswith(kernel_dir_path)):
          f.write(line)
        line = fp.readline()
    f.close()
  fp.close()

def process_pp_file(pp_file_name):
  with open(pp_file_name) as f:
    line = f.readline()
    while line:
      file_name = line.split(" ")[0]
      pp_line_number = line.split(" ")[1]
      if file_name not in pp_files:
        pp_files[file_name] = set()
      pp_files[file_name].add(pp_line_number) 
      line = f.readline()
  f.close()
  # print pp_files

def generate_npp_bb(kernel_sub_dir):
  global macro_definition_counter
  global macro_definition_output
  total_npp_functions = 0
  total_pp_functions = 0
  total_lines_inserted = 0
  total_lines_in_blacklist = 0
  macro_inline_to_instrument = 0
  macro_inline_instrumented = 0
  total_npp_bb = 0
  prefix = "SF:/home/liyiwen/Documents/projects/tracks/linux-stable/"

  generate_blacklist()

  for pp_file_name in pp_files:
    file_name = pp_file_name[len(prefix):-1]
    print "now processing file: " + file_name

    # debug
    # if (file_name != "drivers/tty/serial/8250/8250_port.c"):
    #   continue
    if (file_name.startswith("arch/x86/include/asm/")) or (file_name.startswith("arch/x86/lib/kaslr.c")) or (file_name.startswith("lib/cmdline.c")) or (file_name.startswith("include/linux/compiler.h")) or (file_name.startswith("include/asm-generic/getorder.h")) or (file_name.startswith("include/linux/err.h")) or (file_name.startswith("lib/zlib_inflate/inflate.c")):
      continue

    bb_output_file = "bb_output/bb_output.bb"
    src_file_path = "/home/liyiwen/Documents/projects/clang-kernel-build-4.14.24/clang-kernel-build/linux-stable/" + file_name
    cmd = "./MyClangTool " + src_file_path + " > " + bb_output_file + " 2>&1"
    os.system(cmd)

    # read the basic-block data generated by our Clang tool into the memory
    # here we first analyze the functions to see if any of them are non popular
    # we only add the first basic-block in our npp_bb (non-popular basic block)
    # if it is an non-popular function
    npp_bb = set()
    pp_func = set()
    npp_func = set()
    npp_func_bb = dict()
    with open(bb_output_file) as fp:  
      line = fp.readline()
      cnt = 0
      while line:
        cnt += 1
        if (line.startswith(file_name)):
          processed_line = line.split(' ')
          # print processed_line
          line_number = processed_line[0].split(':')[1]
          if ("<scratch space>" in line): 
            line_func_name = processed_line[3]
            line_func_start = processed_line[4].split(':')[1]
            line_func_end = processed_line[5].split(':')[1]
          else: 
            if ("<Spelling=" in line):
              if ("<Spelling=" in processed_line[1]):
                line_func_name = processed_line[2]
                line_func_start = processed_line[3].split(':')[1]
                line_func_end = processed_line[4].split(':')[1]
              elif ("<Spelling=" in processed_line[3]):
                line_func_name = processed_line[1]
                line_func_start = processed_line[2].split(':')[1]
                line_func_end = processed_line[4].split(':')[1]
              else:
                line_func_name = processed_line[1]
                line_func_start = processed_line[2].split(':')[1]
                line_func_end = processed_line[3].split(':')[1]
            else: 
              line_func_name = processed_line[1]
              line_func_start = processed_line[2].split(':')[1]
              line_func_end = processed_line[3].split(':')[1]

          if not ((int(line_number) >= int(line_func_start)) and (int(line_number) <= int(line_func_end))): 
            line = fp.readline()
            continue
         
          # if this is the first time we see this function, check whether it is popular 
          # print line_func_name 
          if ((line_func_name not in pp_func) and (line_func_name not in npp_func)):
            is_pp_func = False
            for test in range(int(line_func_start), int(line_func_end) + 1, 1): 
              if str(test)+"\n" in pp_files[pp_file_name]: 
                is_pp_func = True
                total_pp_functions += 1
                break 
            if is_pp_func == True: 
              pp_func.add(line_func_name) 
              # print "*** We got popular function: " + line_func_name
            else: 
              npp_func.add(line_func_name) 
              npp_func_bb[line_func_name] = line_number
              npp_bb.add(line_number)
              # print "*** We got non-popular function: " + line_func_name
               
          if line_func_name in pp_func: 
            if line_number+"\n" not in pp_files[pp_file_name]: 
              npp_bb.add(line_number)
          if line_func_name in npp_func: 
            npp_bb.add(line_number)
        line = fp.readline()
    fp.close()   
    # finally add in the first basic block for each non-popular function
    print "non-popular functions: " + str(len(npp_func))
    print "popular functions: " + str(len(pp_func))

  print "****************"
  print "total npp_bb = " + str(len(npp_bb))

def main():
  print "Our program has started ..."
  print "We are going to instrument kernel dir: " + sys.argv[1]
  print "First, checking the required input files to conduct our kernel instrumentation..." 

  kernel_sub_dir_pp_path = sys.argv[1] + "-pp.txt"
  exists = os.path.isfile(kernel_sub_dir_pp_path)
  if exists: 
    print "Popular paths data found. Now processing..."
    process_pp_file(kernel_sub_dir_pp_path)
    generate_npp_bb(sys.argv[1])
  else: 
    print "Kernel sub dir popular paths data file not found."
    print "Now trying to generate it..."
    exists = os.path.isfile(total_pp_data_path)
    if exists: 
      print "Total popular paths data found. Now processing..." 
      generate_pp_file(sys.argv[1])
      print "Popular paths file generated. Now processing..."
      process_pp_file(kernel_sub_dir_pp_path)   
      generate_npp_bb(sys.argv[1])
    else: 
      print "Critical total popular paths data missing."
      print "Unable to proceed. Program terminated." 

main()
