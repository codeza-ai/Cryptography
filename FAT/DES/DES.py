# pip install pycryptodome

from Crypto.Cipher import DES
from Crypto.Util.Padding import pad, unpad
from Crypto.Util import Counter
import base64
import os

def get_key():
    while True:
        key_input = input("Enter an 8-character key: ")
        if len(key_input) == 8:
            return key_input.encode()  # Convert to bytes
        else:
            print("Key must be exactly 8 characters long. Try again.")

def encrypt(plain_text, key, mode):
    if mode == DES.MODE_ECB:
        cipher = DES.new(key, mode)
        padded_text = pad(plain_text.encode(), DES.block_size)
        encrypted_bytes = cipher.encrypt(padded_text)
    elif mode in [DES.MODE_CBC, DES.MODE_CFB, DES.MODE_OFB]:
        iv = os.urandom(DES.block_size)
        cipher = DES.new(key, mode, iv)
        padded_text = pad(plain_text.encode(), DES.block_size) if mode == DES.MODE_CBC else plain_text.encode()
        encrypted_bytes = iv + cipher.encrypt(padded_text)
    elif mode == DES.MODE_CTR:
        ctr = Counter.new(64, prefix=os.urandom(8))
        cipher = DES.new(key, mode, counter=ctr)
        encrypted_bytes = cipher.encrypt(plain_text.encode())
    else:
        raise ValueError("Unsupported mode")
    return base64.b64encode(encrypted_bytes).decode()

def decrypt(cipher_text, key, mode):
    encrypted_bytes = base64.b64decode(cipher_text)
    if mode == DES.MODE_ECB:
        cipher = DES.new(key, mode)
        decrypted_padded = cipher.decrypt(encrypted_bytes)
        return unpad(decrypted_padded, DES.block_size).decode()
    elif mode in [DES.MODE_CBC, DES.MODE_CFB, DES.MODE_OFB]:
        iv = encrypted_bytes[:DES.block_size]
        encrypted_data = encrypted_bytes[DES.block_size:]
        cipher = DES.new(key, mode, iv)
        decrypted_padded = cipher.decrypt(encrypted_data)
        return unpad(decrypted_padded, DES.block_size).decode() if mode == DES.MODE_CBC else decrypted_padded.decode()
    elif mode == DES.MODE_CTR:
        ctr = Counter.new(64, prefix=encrypted_bytes[:8])
        cipher = DES.new(key, mode, counter=ctr)
        decrypted = cipher.decrypt(encrypted_bytes[8:])
        return decrypted.decode()
    else:
        raise ValueError("Unsupported mode")

# Example usage
if __name__ == "__main__":
    key = get_key()
    message = input("Enter the message to encrypt: ")

    modes = {
        "ECB": DES.MODE_ECB,
        "CBC": DES.MODE_CBC,
        "CFB": DES.MODE_CFB,
        "OFB": DES.MODE_OFB,
        "CTR": DES.MODE_CTR
    }

    for mode_name, mode in modes.items():
        print(f"\nMode: {mode_name}")
        encrypted = encrypt(message, key, mode)
        print("Encrypted:", encrypted)

        decrypted = decrypt(encrypted, key, mode)
        print("Decrypted:", decrypted)
