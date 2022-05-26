from msilib.schema import Directory
import socket, time, os
import aes, rsa, constants


s = socket.socket()
port = 12345
s.bind(('', port))
s.listen(5)

text = 'CanTheyDoTheirFest'
key = 'BUET CSE17 Batch'
k = 32
e, d, n = rsa.generateKeys(k)

ciphertext = aes.AES_Encryption(text, key)
encryptedkey = [rsa.encrypt(ord(c), e, n) for c in key]
encryptedkey = str(encryptedkey)

print('\nciphertext \t= ' + ciphertext.get_bitvector_in_hex())
print('encrypted key \t= ' + encryptedkey)
print('Public Key \t= ' + str(e))
print('Private Key \t= ' + str(d))
print('n           \t= ' + str(n), end='\n\n')

try:
    path = os.path.join(os.getcwd(), constants.DIRECTORY_NAME)
    os.mkdir(path)
    print("Created " + path)
except OSError as error:
    print(error)

file = open(constants.ALICE_FILE_NAME, "w")
file.write(str(d) + ',')
file.write(str(n))
file.close()

while True:
    print("Waiting for client ...")
    c, addr = s.accept()    
    print ('Got connection from', addr )
    
    c.send(ciphertext.get_bitvector_in_ascii().encode())
    c.send(encryptedkey.encode())
    time.sleep(0.005)
    c.send(str(e).encode())
    time.sleep(0.005)
    c.send(str(n).encode())
    
    msg = c.recv(4096).decode()
    print("From Bob->" + msg + "<-")
    if msg != constants.ACK:
        print("Error on Bob's (Client) side")
        exit()
    
    file = open(constants.BOB_FILE_NAME)
    bobsDecipheredText = file.read()
    if bobsDecipheredText == text:
        print("SUCCESS!!")
        print("Bob's deciphered text matches with original text")
    else:
        print("FAILURE :(((")
        print("Bob's deciphered text doesn't match with original text")
        print("Bob's decipher->" + bobsDecipheredText + "<-")

    c.close()
    break