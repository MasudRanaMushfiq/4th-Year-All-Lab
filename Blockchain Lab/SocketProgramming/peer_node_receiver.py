import socket, threading, json
s = socket.socket()
s.connect(('127.0.0.1', 8000))
s.send(json.dumps({'name': 'Bob', 'port': 8002}).encode())
s.recv(1024)
s.close()
def handle(sock):
    print("Received:", json.loads(sock.recv(1024).decode()))
    sock.send(b"ACK")
    sock.close()
srv = socket.socket()
srv.bind(('127.0.0.1', 8002))
srv.listen()
while True:
    sock, _ = srv.accept()
    threading.Thread(target=handle, args=(sock,)).start()
