n = int(input('enter your choice: '))

def Isprime(n):
    if n<=1:
        return False
    else:
        for i in range(2,n):
            if n%i==0:
                return False
            else:
                return True
if Isprime(n):
    print(f"{n} is a prime number")
else:
    print(f"{n} is not a prime number")
def list():
    for n in range(100):
        if Isprime(n):
            print(n,end='\n', flush=True)
        print()
list()            

        

