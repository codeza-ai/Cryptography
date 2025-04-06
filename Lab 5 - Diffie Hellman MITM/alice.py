import socket
import random

p = 23  # Prime number
g = 5   # Generator

a = random.randint(1, p-2)
A = pow(g, a, p)

# Connect to Mallory
sock = socket.socket()
sock.connect(('localhost', 8000))

# Send (p, g, A) to Mallory
sock.send(f"{p},{g},{A}".encode())

# Receive fake B from Mallory
B = int(sock.recv(1024).decode())

# Compute shared key
shared_key = pow(B, a, p)
print(f"Alice's shared key: {shared_key}")
sock.close()
