# two library, sys is for exit, cs50 is for input integer
import sys
import cs50

def main(): # we define a main method
    while True: # python does not have do-while function but instead, pyton uses while True
        print("Height: ", end="") # print funciton in python will add a new line unless we put an empty "" at the end
        height = cs50.get_int() # call a method from a libary, we need to use the name of the libary + "." + method name, here is cs50.get_int()
        if height < 23 or height >= 0: # condition is a little bit different from c codes
            break # in python, break function only jump outside of loop
        
    for row in range(height): # row by row, for loop is a little different and since height is an integer, we use range() method to have (0,height)
        for i in range(height - row): 
            print(" ", end="") # print space
        for i in range(row + 1):  # print #
            print("#", end="")
        print("")

# main method in python
if __name__ == "__main__":
    main()