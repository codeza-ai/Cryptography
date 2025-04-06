import socket
import random

def power(a, b, c):
    result = 1
    a = a % c
    while b > 0:
        if b % 2:
            result = (result * a) % c
        b = b >> 1
        a = (a * a) % c
    return result

def encrypt(m, public_key):
    p, g, y = public_key
    k = random.randint(2, p - 2)
    a = power(g, k, p)
    b = (ord(m) * power(y, k, p)) % p
    return (a, b)

# Connect to server
host = '127.0.0.1'
port = 12345
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))

# Receive public key
pubkey_data = client.recv(1024).decode()
p, g, y = map(int, pubkey_data.split(','))
public_key = (p, g, y)
print(f"\nReceived Public Key (p, g, y): {public_key}")

# Encrypt message
message = input("Enter a single character to encrypt: ")
cipher = encrypt(message, public_key)
print(f"Encrypted (a, b): {cipher}")

# Send encrypted data
a, b = cipher
client.send(f"{a},{b}".encode())

client.close()
