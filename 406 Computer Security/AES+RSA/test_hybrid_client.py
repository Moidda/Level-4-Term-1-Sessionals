import socket, rsa, aes          
import constants


s = socket.socket()    
port = 12345        
s.connect(('127.0.0.1', port))

ciphertext = s.recv(4096).decode()
encryptedKey = s.recv(4096).decode()
encryptedKey = eval(encryptedKey)
publicKey_e = s.recv(4096).decode()
publicKey_n = s.recv(4096).decode()

file = open(constants.ALICE_FILE_NAME, "r")
privateKey_d, privateKey_n = file.read().split(",")
privateKey_d = int(privateKey_d)
privateKey_n = int(privateKey_n)
file.close()

print("+---------------Received---from---Alice---------------")
print("+ Ciphered Text (CT)  ->" + ciphertext)
print("+ Encrypted Key (EK)  ->" + str(encryptedKey))
print("+ Public Key    (PUK) ->" + "{" + publicKey_e + "," + publicKey_n + "}")
print("+---------------Polled---from---Directory-------------")
print("+ Private Key   (PRK) ->" + "{" + str(privateKey_d) + "," + str(privateKey_n) + "}")
print("+-----------------------------------------------------")

aesKey = ''.join([chr(rsa.decrypt(x, privateKey_d, privateKey_n)) for x in encryptedKey])
print("aes key = " + aesKey)

decipheredText = aes.AES_Decryption(ciphertext, aesKey)
decipheredText = decipheredText.get_bitvector_in_ascii()
print("Deciphered Text = " + decipheredText)

while decipheredText[-1] == constants.PAD:
    decipheredText = decipheredText[:-1]

file = open(constants.BOB_FILE_NAME, "w")
file.write(decipheredText)
file.close()

s.send(constants.ACK.encode())

s.close()