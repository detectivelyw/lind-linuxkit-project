import os

def replace_files(dir_root): 
    for dirpath, dirs, files in os.walk(dir_root): 
      for filename in files:
        fname = os.path.join(dirpath, filename)
        if fname.endswith('.new'):
          os.remove(fname[:-4])
          os.rename(fname, fname[:-4])

dir_root = "/home/detectivelyw/Documents/projects/tracks/linux-stable"
replace_files(dir_root)
        
