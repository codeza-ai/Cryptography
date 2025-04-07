import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '127.0.0.1'  # localhost
port = 12345

server_socket.bind((host, port))
server_socket.listen(1)
print(f"Server listening on {host}:{port}...")

conn, addr = server_socket.accept()
print(f"Connected by {addr}")

data = conn.recv(1024).decode()
print(f"Received from client: {data}")

response = "Hello from server!"
conn.send(response.encode())

conn.close()