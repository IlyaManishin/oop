from interface import *

#EXAMPLES

if exists("name.wav"):
    file = open_wav("name.wav")
else:
    file = create_wav("name.wav")

start = time("0:0:10")
end =  time("0:0:20")
interval = file.get_interval(start, end)

other = open_wav("name1.wav")
other.write_interval(interval)

