from interface import *

#EXAMPLES

if (exists("name.wav")):
    file = open("name.wav")
else:
    file = create("name1.wav")

interval = file.get_interval(1, 2)

other = open("name1.wav")
other.write_interval(interval)

