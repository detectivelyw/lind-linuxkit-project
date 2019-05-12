import os

dir_root = "/home/detectivelyw/Documents/projects/tracks/linuxkit-auto-experiment/"
raw_data_path = dir_root + "data/"
processed_data_path = dir_root + "processed-data/"
gcov_parsed_data_path = dir_root + "gcov-parsed-data/"

raw_data_dict = dict()
processed_data_dict = dict()
parsed_data_dict = dict()

def scan_files(): 
  file_num = 0
  for dirpath, dirs, files in os.walk(raw_data_path): 
    for filename in files:
      fname = os.path.join(dirpath, filename)
      if fname.split("/")[8].split("-")[3] != fname.split("/")[8].split("-")[-1]: 
        file_name = fname.split("/")[8].split("-")[2] + "-" + fname.split("/")[8].split("-")[3]
      else:
        file_name = fname.split("/")[8].split("-")[2]
      number = (int)(fname.split("/")[8].split("-")[-1].split(".")[0])
      if file_name not in raw_data_dict:
        raw_data_dict[file_name] = number
        file_num += 1
      else:
        if raw_data_dict[file_name] < number:
          raw_data_dict[file_name] = number
  # print raw_data_dict
  # print file_num

  for dirpath, dirs, files in os.walk(processed_data_path): 
    for filename in files:
      fname = os.path.join(dirpath, filename)
      if fname.split("/")[8].split("-")[3] != fname.split("/")[8].split("-")[-1]: 
        file_name = fname.split("/")[8].split("-")[2] + "-" + fname.split("/")[8].split("-")[3]
      else:
        file_name = fname.split("/")[8].split("-")[2]
      number = (int)(fname.split("/")[8].split("-")[-1].split(".")[0])
      if file_name not in processed_data_dict:
        processed_data_dict[file_name] = number
      else:
        if processed_data_dict[file_name] < number:
          processed_data_dict[file_name] = number
  # print processed_data_dict

  for dirpath, dirs, files in os.walk(gcov_parsed_data_path): 
    for filename in files:
      fname = os.path.join(dirpath, filename)
      if fname.split("/")[8].split("-")[4] != fname.split("/")[8].split("-")[-1]: 
        file_name = fname.split("/")[8].split("-")[2] + "-" + fname.split("/")[8].split("-")[3]
      else:
        file_name = fname.split("/")[8].split("-")[2]
      number = (int)(fname.split("/")[8].split("-")[-1].split(".")[0])
      if file_name not in parsed_data_dict:
        parsed_data_dict[file_name] = number
      else:
        if parsed_data_dict[file_name] < number:
          parsed_data_dict[file_name] = number
  # print parsed_data_dict

def generate_data_to_process():
  OutputFileName = "process-raw-data-script.sh"
  with open(OutputFileName, "w+") as f:
    f.write("#!/bin/bash\n")
    for data in raw_data_dict:
      start_num = 0
      if (data not in processed_data_dict) and (data not in parsed_data_dict):
        output = "./process-raw-data.sh " + data + " 1 " + str(raw_data_dict[data]) + "\n" 
        f.write(output)
      else: 
        if data in processed_data_dict: 
          if processed_data_dict[data] <= raw_data_dict[data]:
            start_num = processed_data_dict[data]
        if data in parsed_data_dict: 
          if (parsed_data_dict[data] <= raw_data_dict[data]) and (parsed_data_dict[data] > start_num):
            start_num = parsed_data_dict[data]
        if start_num < raw_data_dict[data]:
          output = "./process-raw-data.sh " + data + " " + str(start_num) + " " + str(raw_data_dict[data]) + "\n" 
          f.write(output)
  f.close()

def generate_data_to_parse():
  OutputFileName = "parse-gcov-data-script.sh"
  with open(OutputFileName, "w+") as f:
    f.write("#!/bin/bash\n")
    for data in processed_data_dict:
      if data not in parsed_data_dict:
        output = "./parse-gcov-data.sh " + data + " 1 " + str(processed_data_dict[data]) + "\n" 
        f.write(output)
      else: 
        if parsed_data_dict[data] < processed_data_dict[data]:
          output = "./parse-gcov-data.sh " + data + " " + str(parsed_data_dict[data] + 1) + " " + str(processed_data_dict[data]) + "\n" 
          f.write(output)
  f.close()

def scan_parsed_files():
  output = ""
  for dirpath, dirs, files in os.walk(gcov_parsed_data_path): 
    for filename in files:
      fname = os.path.join(dirpath, filename)
      if fname.split("/")[8].split("-")[4] != fname.split("/")[8].split("-")[-1]: 
        file_name = fname.split("/")[8].split("-")[2] + "-" + fname.split("/")[8].split("-")[3]
      else:
        file_name = fname.split("/")[8].split("-")[2]
      number = (int)(fname.split("/")[8].split("-")[-1].split(".")[0])
      if file_name not in parsed_data_dict:
        parsed_data_dict[file_name] = number
      else:
        if parsed_data_dict[file_name] < number:
          parsed_data_dict[file_name] = number
  for data in parsed_data_dict: 
    if parsed_data_dict[data] >= 10: 
      output += "\"" + data + "\"" + ", "
  print output
      
def main():
  # scan_files()
  # generate_data_to_process()
  # generate_data_to_parse()
  scan_parsed_files()
        
main()
