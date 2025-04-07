import socket

def encrypt(msg, pub_key):
    e, n = pub_key
    return [pow(ord(char), e, n) for char in msg]

host = '127.0.0.1'
port = 12345
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))

public_key_data = client.recv(1024).decode()
public_key = tuple(map(int, public_key_data.split(',')))
print(f"Public key received: {public_key}")
msg = input("Enter message to encrypt and send: ")
cipher = encrypt(msg, public_key)

cipher_str = ','.join(map(str, cipher))
client.send(cipher_str.encode())
print("Encrypted message sent.")

client.close()
