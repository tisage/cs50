import sys
import cs50

def main():
	print("O hai! ", end="")
	while True: # do-while loop in python
	    print("How much change is owed?")
	    change = cs50.get_float()
	    if change > 0:
	        break

	remain = round(change * 100)
	coins = 0

	while remain > 0:
		if remain >= 25:
			remain -= 25
			coins+=1
		elif remain >= 10:
			remain -= 10
			coins+=1
		elif remain >= 5:
			remain -= 5
			coins+=1
		elif remain >= 1:
			remain -= 1
			coins+=1
	print(coins)
	
if __name__ == "__main__":
    main()