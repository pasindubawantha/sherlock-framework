import json
import pandas
import math
import sys
import os
import numpy as np
import re
import shutil
import SimpleHTTPServer
import SocketServer


# PORT = 8000

data_directory = "../data/nab_tuned_results_noCDD_training_ratio/training_ratio_0.35"
write_directory = "./data"

reg_x = re.compile(r'\.(csv)')
csv_input_files = []
for path, dnames, fnames in os.walk(data_directory):
    csv_input_files.extend([os.path.join(path, f) for f in fnames if reg_x.search(f)])

csv_input_files.sort()

try:
    shutil.rmtree(write_directory)

except OSError:
    print("No previous ", write_directory)

os.mkdir(write_directory)

out_file = open("list.csv", "w")
out_file.write("file\n")
for f in csv_input_files:
    print(f)
    fname = f.split("/")[-1]
    newf = write_directory + "/" + fname
    shutil.copyfile(f, newf)
    print(newf)
    out_file.write(newf+"\n")

out_file.close()

# Handler = SimpleHTTPServer.SimpleHTTPRequestHandler

# httpd = SocketServer.TCPServer(("", PORT), Handler)

# print "serving at port", PORT
# print "http://localhost:"+str(PORT)
# httpd.serve_forever()