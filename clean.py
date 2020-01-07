import os
import re

def through_path(dir):
  # print dir
  list = os.listdir(dir)
  for path in list:
    ab_path = dir+'/'+path
    if os.path.isdir(ab_path):
      through_path(ab_path)
    else:
      if re.match("core\.\d+",path) != None or re.match("a.out",path) != None or re.match("a.exe",path) != None:
        os.remove(ab_path)

through_path(os.getcwd())
