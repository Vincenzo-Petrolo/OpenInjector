from PIL import Image
import numpy as np
import csv
import math
import sys

RED     = (249, 64, 2)  # <---- SIGNALED
ORANGE  = (249, 117, 2) # <---- TIMEOUT
YELLOW  = (249, 237, 2) # <---- SETUP_ERROR
GREEN   = (97, 124, 98) # <---- OK - NO ERRORS BUT SLOWER
CYAN    = (2, 249, 212) # <---- OK - NO ERRORS AND FASTER THAN GOLDEN TIME
NULL    = (0,0,0)       # <---- NO BITS
GOLDEN_TIME = -1 # will be updated
MAX_COLS = int(sys.argv[2]) # maximum pixels in horizontal
STATISTICS_FILENAME = str(sys.argv[1])
INJECTION_ID="injection_id"
EXIT_STATUS="exit_status"


# reads golden time
def init():
    with open(STATISTICS_FILENAME, 'r') as csv_file:
        reader = csv.reader(csv_file)

        for row in reader:
            injection_id = str(row[0])
            if injection_id == "Golden time":
                GOLDEN_TIME = float(row[1])
                return

# parse given csv line to pixel colour
def cvs_pixel(exit_status):
    if exit_status == "SIGNALED":
        return RED
    elif exit_status == "TIMEOUT":
        return ORANGE
    elif exit_status == "SETUP_ERROR":
        return YELLOW
    else:
        try:
            if float(exit_status) > GOLDEN_TIME:
                return GREEN
            if float(exit_status) <= GOLDEN_TIME:
                return CYAN
        except ValueError:
            pass
        

def matrix_composer():
    # start reading from CSV
    with open(STATISTICS_FILENAME, 'r') as csv_file:
        reader = csv.reader(csv_file)

        list = []
        tmp_row = []
        i = 0
        for row in reader:
            if (i < MAX_COLS):
                tmp_row.append(cvs_pixel(str(row[1])))
                i += 1
            else:
                i = 0
                list.append(tmp_row)
                tmp_row = []
        #MAX_ROWS = math.ceil(float(len(list)/MAX_COLS))
        #for i in range(len(list),MAX_ROWS*MAX_COLS):
        #    list.append(NULL)
        array = np.array(list,dtype=np.uint8)
        new_image = Image.fromarray(array)
        new_image.save('new.png')



# very beginning of the program

init()
matrix_composer()
