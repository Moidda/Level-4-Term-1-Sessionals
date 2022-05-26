from BitVector import *
import copy, time
import constants


keyScheduleTime = 0.0
encryptionTime = 0.0
decryptionTime = 0.0


Sbox = (
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
)

InvSbox = (
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D,
)

Mixer = [
    [BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03")],
    [BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02")]
]

InvMixer = [
    [BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09")],
    [BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D")],
    [BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B")],
    [BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E")]
]


rcon = [
    BitVector(hexstring="01") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="02") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="04") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="08") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="10") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="20") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="40") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="80") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="1B") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00"),
    BitVector(hexstring="36") + BitVector(hexstring="00") + BitVector(hexstring="00") + BitVector(hexstring="00")
]


def subByte(byte: BitVector):
    sub = Sbox[byte.intValue()]
    return BitVector(intVal=sub, size=8)


def subByteInv(byte: BitVector):
    sub = InvSbox[byte.intValue()]
    return BitVector(intVal=sub, size=8)


def g(word: BitVector, roundNo: int):
    wordCopy = copy.deepcopy(word)
    wordCopy = wordCopy << 8
    for i in range(0, 4):
        wordCopy[i*8:(i+1)*8] = subByte(wordCopy[i*8:(i+1)*8])

    wordCopy = wordCopy^rcon[roundNo-1]
    return wordCopy


def expandKey(key: BitVector, roundNo: int):
    w = [key[i:i+32] for i in range(0, len(key), 32)]
    w4 = w[0]^g(w[3], roundNo)
    w5 = w[1]^w4
    w6 = w[2]^w5
    w7 = w[3]^w6
    return w4+w5+w6+w7


def getMatrix(bv: BitVector):
    mat = [[BitVector(size=0) for i in range(4)] for i in range(4)]
    x = 0
    for j in range(4):
        for i in range(4):
            mat[i][j] = bv[x*8:(x+1)*8]
            x += 1

    return mat


def getBitVector(mat):
    bv = BitVector(size=0)
    for j in range(4):
        for i in range(4):
            bv += mat[i][j]
    return bv


def printMatrix(mat):
    for row in mat:
        for bv in row:
            print(bv.get_bitvector_in_hex(), end=' ')
        print('')    


def addRoundKey(mat1, mat2):
    bv1 = getBitVector(mat1)
    bv2 = getBitVector(mat2)
    return getMatrix(bv1^bv2)


def substitutionBytes(mat):
    matcopy = copy.deepcopy(mat)
    for i in range(4):
        for j in range(4):
            matcopy[i][j] = subByte(matcopy[i][j])
    return matcopy


def substitutionBytesInv(mat):
    matcopy = copy.deepcopy(mat)
    for i in range(4):
        for j in range(4):
            matcopy[i][j] = subByteInv(matcopy[i][j])
    return matcopy


def shiftRows(mat):
    matcopy = copy.deepcopy(mat)
    for i in range(1, 4):
        row = copy.deepcopy(matcopy[i])
        for j in range(4):
            row[j] = matcopy[i][(j+i)%4]
        matcopy[i] = copy.deepcopy(row)
    return matcopy


def shiftRowsInv(mat):
    matcopy = copy.deepcopy(mat)
    for i in range(1, 4):
        row = copy.deepcopy(matcopy[i])
        for j in range(4):
            row[j] = matcopy[i][(j-i+4)%4]
        matcopy[i] = copy.deepcopy(row)
    return matcopy



def galiosMult(bv1, bv2):
    AES_modulus = BitVector(bitstring='100011011')
    bv3 = bv1.gf_multiply_modular(bv2, AES_modulus, 8)
    return bv3


def mixColumn(fixedMat, stateMat):
    mat3 = copy.deepcopy(fixedMat)
    for i in range(4):
        for j in range(4):
            temp = galiosMult(fixedMat[i][0], stateMat[0][j])
            for k in range(1, 4):
                temp ^= galiosMult(fixedMat[i][k], stateMat[k][j])
            mat3[i][j] = temp
    return mat3


# CREATING KEY SCHEDULE THROUGH KEY EXPANDING
def getKeySchedule(key: str):
    if len(key) != 16:
        print('Keys should be of length 16')
        exit()

    keySchedule = BitVector(textstring=key)
    prvKey = copy.deepcopy(keySchedule)
    for i in range(1, 11):
        prvKey = expandKey(prvKey, i)
        keySchedule += prvKey
    
    return keySchedule


def encrypt(plainText: BitVector, keySchedule: BitVector):
    # ROUND 0
    state = addRoundKey(getMatrix(plainText), getMatrix(keySchedule[0:128]))

    # ROUND 1...9
    for i in range(1, 10):
        state = substitutionBytes(state)
        state = shiftRows(state)
        state = mixColumn(Mixer, state)
        state = addRoundKey(state, getMatrix(keySchedule[i*128:(i+1)*128]))

    # ROUND 10
    state = substitutionBytes(state)
    state = shiftRows(state)
    state = addRoundKey(state, getMatrix(keySchedule[10*128:11*128]))

    return getBitVector(state)


def decrypt(cipherText: BitVector, keySchedule: BitVector):
    # ROUND 0
    state = addRoundKey(getMatrix(cipherText), getMatrix(keySchedule[10*128:11*128]))

    # ROUND 1...9
    for i in range(1, 10):
        state = shiftRowsInv(state)
        state = substitutionBytesInv(state)
        state = addRoundKey(state, getMatrix(keySchedule[(10-i)*128 : (11-i)*128]))
        state = mixColumn(InvMixer, state)

    # ROUND 10
    state = shiftRowsInv(state)
    state = substitutionBytesInv(state)
    state = addRoundKey(state, getMatrix(keySchedule[0:128]))

    return getBitVector(state)


def AES_Encryption(plainText: str, key: str):
    while len(plainText)%16 != 0:
        plainText += constants.PAD
    
    start = time.time()
    keySchedule = getKeySchedule(key)
    end = time.time()
    global keyScheduleTime
    keyScheduleTime = end-start

    start = time.time()
    cipherText = BitVector(size=0)
    for i in range(0, len(plainText), 16):
        cipherText += encrypt(BitVector(textstring=plainText[i:i+16]), keySchedule)
    
    end = time.time()
    global encryptionTime
    encryptionTime = end-start

    return cipherText


def AES_Decryption(cipherText: str, key: str):
    keySchedule = getKeySchedule(key)
    start = time.time()
    decipherText = BitVector(size=0)
    for i in range(0, len(cipherText), 16):
        decipherText += decrypt(BitVector(textstring=cipherText[i:i+16]), keySchedule)
    
    end = time.time()
    global decryptionTime
    decryptionTime = end-start

    return decipherText

