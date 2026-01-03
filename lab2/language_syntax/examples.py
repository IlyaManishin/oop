from interface import *

#EXAMPLES

if exists("name.wav"):
    file = open("name.wav")
else:
    file = create("name1.wav")

const1 = 1
const2 = 2
interval = file.get_interval(const1, const2)

other = open("name1.wav")
other.write_interval(interval)

