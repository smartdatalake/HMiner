import os.path
from os import path
import sys
import json

with open(sys.argv[2]) as config_file:
    config = json.load(config_file)
    outfile = config["analysis_out"]
    
    results_exist = False

    if path.exists(outfile):
        results_exist = True

    if "ranking" in config:
        if path.exists(config["ranking"]["ranking_out"]):
            results_exist = results_exist and True
        else:
            results_exist = results_exist and False
    
    if results_exist:
        sys.exit(-1)
