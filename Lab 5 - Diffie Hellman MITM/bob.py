import socket
import random

# Wait for connection from Mallory
sock = socket.socket()
sock.bind(('localhost', 8002))
sock.listen(1)
print("Bob listening...")

conn, addr = sock.accept()
p, g, B = None, None, None

# Receive (p, g, A) from Mallory
data = conn.recv(1024).decode()
p, g, A = map(int, data.split(','))

b = random.randint(1, p-2)
B = pow(g, b, p)

# Send B back to Mallory
conn.send(str(B).encode())

# Compute shared key
shared_key = pow(A, b, p)
print(f"Bob's shared key: {shared_key}")
conn.close()
sock.close()
