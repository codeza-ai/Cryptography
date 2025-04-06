#pip install pycryptodome

from Crypto.Cipher import DES
from Crypto.Util.Padding import pad, unpad
import base64

def get_key():
    while True:
        key_input = input("Enter an 8-character key: ")
        if len(key_input) == 8:
            return key_input.encode()  # Convert to bytes
        else:
            print("Key must be exactly 8 characters long. Try again.")

def encrypt(plain_text, key):
    cipher = DES.new(key, DES.MODE_ECB)
    padded_text = pad(plain_text.encode(), DES.block_size)
    encrypted_bytes = cipher.encrypt(padded_text)
    return base64.b64encode(encrypted_bytes).decode()

def decrypt(cipher_text, key):
    cipher = DES.new(key, DES.MODE_ECB)
    encrypted_bytes = base64.b64decode(cipher_text)
    decrypted_padded = cipher.decrypt(encrypted_bytes)
    return unpad(decrypted_padded, DES.block_size).decode()

# Example usage
if __name__ == "__main__":
    key = get_key()
    message = input("Enter the message to encrypt: ")

    encrypted = encrypt(message, key)
    print("🔒 Encrypted:", encrypted)

    decrypted = decrypt(encrypted, key)
    print("🔓 Decrypted:", decrypted)

