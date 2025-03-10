# Given the followinginput:
# a = 3-> a real-valued scalarnumber
# x= (1,1,1,1,1,..., 1)-> a real-valued vector of dimension N = 20
# y= (4,4,4,4,4,..., 4)-> a real-valued vector of dimension N = 20
# write a computer programthat calculates the following scalar product: 
# z = a*x + y
# and printits value on a text file

import os
import numpy as np

OUTPUT_DIR = "output"
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

OUTPUT_FILE = "out_python.txt"

def main():
    
    # Define the input variables
    a = 3
    x = np.full(20, 1)
    y = np.full(20, 4)
    
    # Calculate the result z = a*x + y
    z = a * x + y # z is a vector of dimension N = 20 with all elements equal to 7
    
    # Write the result to a text file
    with open(os.path.join(OUTPUT_DIR, OUTPUT_FILE), "w") as f:
        f.write("z = " + str(z))
   

if __name__ == "__main__":
    main()
