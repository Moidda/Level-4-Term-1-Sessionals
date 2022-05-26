import rsa, math


text = input('Text: ')
print('')

data = {}
KEY_GENERATION = "Key-Generation"
ENCRYPTION = "Encryption"
DECRYPTION = "Decryption"

for k in [16, 32, 64, 128]:
    print("k = " + str(k))
    e, d, n = rsa.generateKeys(k)

    print('Generated Keys')
    print('{\'public\': (%d, %d), \'private\': (%d, %d)}' % (e, n, d, n), end='\n\n')

    cipher = rsa.RSA_Encryption(text, e, n)
    print('Ciphered Text:' + str(cipher), end='\n\n')

    decipher = rsa.RSA_Decryption(cipher, d, n)
    print('Deciphered Text: ' + decipher, end='\n\n')

    data[k] = {
        KEY_GENERATION: rsa.keyGenerationTime,
        ENCRYPTION: rsa.encryptionTime,
        DECRYPTION: rsa.decryptionTime
    }


print("+--K------Key Generation-----Encryption-------Decryption---+")
for k in data:
    print("| {:<5} | {:<.12f} | {:<.12f} | {:<.12f} |".format(k, data[k][KEY_GENERATION], data[k][ENCRYPTION], data[k][DECRYPTION]))
print("+----------------------------------------------------------+")