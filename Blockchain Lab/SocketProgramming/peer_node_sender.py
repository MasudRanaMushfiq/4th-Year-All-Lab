import socket, json
s = socket.socket()
s.connect(('127.0.0.1', 8000))
s.send(json.dumps({'name': 'Alice', 'port': 8001}).encode())
peers = json.loads(s.recv(1024).decode())
s.close()
if 'Bob' in peers:
    tx = {'from': 'Alice', 'to': 'Bob', 'amount': '5 BTC'}
    s2 = socket.socket()
    s2.connect(('127.0.0.1', peers['Bob']))
    s2.send(json.dumps(tx).encode())
    print("Response:", s2.recv(1024).decode())
    s2.close()
else:
    print("Bob not found")
