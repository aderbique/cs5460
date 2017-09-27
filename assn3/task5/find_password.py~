import os, sys
fname = '/home/seed/Desktop/cs5460/assn3/task5/dictionary.txt'

string_part_1 = 'openssl enc -aes-128-cbc -in plaintext.txt -out enc_plaintext -K '
string_part_2 = ' -iv 0000000000000000'



f = open(fname)
for line in f:
	string = string_part_1 + line + string_part_2
	os.system(string)
	

