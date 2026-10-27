import socket, threading, json
HOST, PORT = '127.0.0.1', 8000
peers = {}
def handle(sock):
    try:
        data = json.loads(sock.recv(1024).decode())
        sock.send(json.dumps(peers).encode())
        peers[data['name']] = data['port']
    except: pass
    finally: sock.close()
s = socket.socket()
s.bind((HOST, PORT))
s.listen()
while True:
    sock, _ = s.accept()
    threading.Thread(target=handle, args=(sock,)).start()
