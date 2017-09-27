import os

for i in range(100):
	os.system("cat /proc/sys/kernel/random/entropy_avail")
	os.system("head -c 1600 /dev/urandom | hexdump")
