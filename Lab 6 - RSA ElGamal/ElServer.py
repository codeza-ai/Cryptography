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

def generate_keys():
    p = 467  # A large prime number
    g = 2    # A primitive root modulo p

    x = random.randint(2, p - 2)        # Private key
    y = power(g, x, p)                  # Public key component

    public_key = (p, g, y)
    private_key = x

    return public_key, private_key

def decrypt(ciphertext, private_key, p):
    a, b = ciphertext
    s = power(a, private_key, p)
    plain = (b * power(s, p - 2, p)) % p  # b / s mod p
    return chr(plain)

# Server setup
host = '127.0.0.1'
port = 12345
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host, port))
server.listen(1)

print(f"Server listening on {host}:{port}...")
conn, addr = server.accept()
print(f"Connected by {addr}")

# Generate keys
public_key, private_key = generate_keys()
p, g, y = public_key
print(f"\Public Key (p, g, y): {public_key}")
print(f"Private Key (x): {private_key}\n")

# Send public key to client
conn.send(f"{p},{g},{y}".encode())

# Receive encrypted data
data = conn.recv(1024).decode()
a_str, b_str = data.split(',')
cipher = (int(a_str), int(b_str))

# Decrypt and display
plain_char = decrypt(cipher, private_key, p)
print(f"Decrypted message: {plain_char}")

conn.close()
server.close()
