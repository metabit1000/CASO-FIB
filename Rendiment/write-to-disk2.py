import time

f=open("basura.txt","w+")
start_time = time.time()
a = 50000000
for i in range(a):
	f.write("0123456789")#cada Char es 1 byte y escribim 10 chars 50 000 000 vegades
print("Temps invertit en segons:")
segons = (time.time() - start_time)
print(str(segons)+ " segons")
print("\n")
print("Bandwith:")
print(str(a/segons/100000) + " MB per segon")

f.close