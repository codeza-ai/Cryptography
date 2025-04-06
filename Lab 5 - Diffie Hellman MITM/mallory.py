import socket

# Step 1: Listen for Alice
listener = socket.socket()
listener.bind(('localhost', 8000))
listener.listen(1)
print("Mallory waiting for Alice...")
alice_conn, _ = listener.accept()

# Step 2: Receive (p, g, A) from Alice
data = alice_conn.recv(1024).decode()
p, g, A = map(int, data.split(','))
print(f"Mallory got A from Alice: {A}")

# Step 3: Connect to Bob
bob_conn = socket.socket()
bob_conn.connect(('localhost', 8002))

# Step 4: Send fake (p, g, M) to Bob
M1 = 13  # Mallory’s fake A to Bob
bob_conn.send(f"{p},{g},{M1}".encode())

# Step 5: Receive B from Bob
B = int(bob_conn.recv(1024).decode())
print(f"Mallory got B from Bob: {B}")

# Step 6: Send fake B to Alice
M2 = 17  # Mallory’s fake B to Alice
alice_conn.send(str(M2).encode())

# Mallory now has two shared secrets
# Key with Alice: (M2)^a mod p (she will compute)
# Key with Bob: (M1)^b mod p (he will compute)

print("Mallory has split the communication.")
alice_conn.close()
bob_conn.close()
listener.close()

# Run bob.py first (waiting for connection).
# Run mallory.py (connects to Bob, waits for Alice).
# Run alice.py.