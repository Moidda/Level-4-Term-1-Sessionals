import time
import aes

plainText = input("Plain Text: ")
key = input("Key: ")

cipherText = aes.AES_Encryption(plainText, key)
print('Cipher Text:')
print(cipherText.get_bitvector_in_hex() + ' [IN HEX]')
print(cipherText.get_bitvector_in_ascii() + ' [IN ASCII]\n')


decipheredText = aes.AES_Decryption(cipherText.get_bitvector_in_ascii(), key)
print('Deciphered Text:')
print(decipheredText.get_bitvector_in_hex() + ' [IN HEX]')
print(decipheredText.get_bitvector_in_ascii() + ' [IN ASCII]\n')


print("Execution Time")
print("Key Scheduling: " + str(aes.keyScheduleTime) + " seconds")
print("Encryption Time: " + str(aes.encryptionTime) + " seconds")
print("Decryption Time: " + str(aes.decryptionTime) + " seconds")