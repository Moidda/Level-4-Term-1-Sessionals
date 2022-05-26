from BitVector import *
import random, math, time


ERR = 0.0001
encryptionTime = 0.0
decryptionTime = 0.0
keyGenerationTime = 0.0


def generatePrime(k: int):
    while True:
        bv = BitVector(intVal = 0)
        bv = bv.gen_random_bits(k)  
        check = bv.test_for_primality()
        if abs(check - 1) < ERR:
            return bv.intValue()


def generatePrimesPQ(k: int):
    p = generatePrime(k//2)
    q = generatePrime(k//2)
    while q == p:
        q = generatePrime(k//2)

    return p, q


# PUBLIC KEY
def generate_e(phi_n):
    while True:
        e = random.randint(2, phi_n-1)
        if math.gcd(e, phi_n) == 1:
            return e


# PRIVATE KEY
def generate_d(e, phi_n):
    return pow(e, -1, phi_n)


def generateKeys(k: int):
    start = time.time()
    
    p, q = generatePrimesPQ(k)
    n = p*q
    phi_n = (p-1)*(q-1)
    e = generate_e(phi_n)
    d = generate_d(e, phi_n)

    end = time.time()
    global keyGenerationTime
    keyGenerationTime = end-start
    
    return e, d, n


def encrypt(x: int, e: int, n: int):
    return pow(x, e, n)


def decrypt(x: int, d: int, n: int):
    return pow(x, d, n)


def RSA_Encryption(text: str, e: int, n: int):
    """Encodes the text character by character and returns a list of integers"""
    start = time.time()

    cipher = [encrypt(ord(c), e, n) for c in text]
    
    end = time.time()
    global encryptionTime
    encryptionTime = end-start
    
    return cipher


def RSA_Decryption(cipher, d: int, n: int):
    """Takes in a list of integers and returns a text in ascii"""
    start = time.time()
    
    decipher = ''.join([chr(decrypt(x, d, n)) for x in cipher])
    
    end = time.time()
    global decryptionTime 
    decryptionTime = end - start
    
    return decipher
