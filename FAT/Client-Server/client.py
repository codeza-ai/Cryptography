import socket

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '127.0.0.1'
port = 12345

client_socket.connect((host, port))

message = "Hello from client!"
client_socket.send(message.encode())

data = client_socket.recv(1024).decode()
print(f"Received from server: {data}")

client_socket.close()