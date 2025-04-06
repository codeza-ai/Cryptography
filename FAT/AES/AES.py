#pip install pycryptodome

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes
import base64

def get_key():
    while True:
        key_input = input("Enter a key (16/24/32 characters): ")
        if len(key_input) in [16, 24, 32]:
            return key_input.encode()
        else:
            print("Key must be 16, 24, or 32 characters long.")

def encrypt(plain_text, key):
    iv = get_random_bytes(16)  # AES block size is 16 bytes
    cipher = AES.new(key, AES.MODE_CBC, iv)
    padded_text = pad(plain_text.encode(), AES.block_size)
    encrypted_bytes = cipher.encrypt(padded_text)
    # Prepend IV to ciphertext and encode
    return base64.b64encode(iv + encrypted_bytes).decode()

def decrypt(cipher_text, key):
    raw_data = base64.b64decode(cipher_text)
    iv = raw_data[:16]
    encrypted_bytes = raw_data[16:]
    cipher = AES.new(key, AES.MODE_CBC, iv)
    decrypted_padded = cipher.decrypt(encrypted_bytes)
    return unpad(decrypted_padded, AES.block_size).decode()

# Example usage
if __name__ == "__main__":
    key = get_key()
    message = input("Enter the message to encrypt: ")

    encrypted = encrypt(message, key)
    print("Encrypted:", encrypted)

    decrypted = decrypt(encrypted, key)
    print("Decrypted:", decrypted)
