import socket

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def mod_inverse(e, phi):
    d = 0
    x1, x2, y1 = 0, 1, 1
    temp_phi = phi

    while e > 0:
        temp1 = temp_phi // e
        temp2 = temp_phi - temp1 * e
        temp_phi, e = e, temp2
        x = x2 - temp1 * x1
        y = d - temp1 * y1
        x2, x1 = x1, x
        d, y1 = y1, y

    return d + phi if d < 0 else d

def generate_keys():
    p = 61
    q = 53
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 17
    while gcd(e, phi) != 1:
        e += 2
    d = mod_inverse(e, phi)
    return ((e, n), (d, n))

def decrypt(cipher, priv_key):
    d, n = priv_key
    decrypted = [chr(pow(c, d, n)) for c in cipher]
    return ''.join(decrypted)

host = '127.0.0.1'
port = 12345
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host, port))
server.listen(1)
print(f"Server started. Waiting for connection at {host}:{port}...")

conn, addr = server.accept()
print(f"Connected by {addr}")

public, private = generate_keys()
print(f"Public key: {public}")
print(f"Private key: {private}")

# Send the public key to the client
conn.send(f"{public[0]},{public[1]}".encode())

data = conn.recv(4096).decode()
print(data)
cipher = list(map(int, data.split(',')))

plain_text = decrypt(cipher, private)
print("Decrypted message:", plain_text)

conn.close()
server.close()
