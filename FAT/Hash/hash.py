import hashlib

def hash_message(message):
    print(f"\nOriginal Message: {message}\n")
    md5_hash = hashlib.md5(message.encode()).hexdigest()
    print(f"MD5        : {md5_hash}")
    sha512_hash = hashlib.sha512(message.encode()).hexdigest()
    print(f"SHA-512    : {sha512_hash}")

if __name__ == "__main__":
    msg = input("Enter message to hash: ")
    hash_message(msg)
