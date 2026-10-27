import socket, threading, json, rsa
s = socket.socket()
s.connect(('127.0.0.1', 8000))
s.send(json.dumps({'name': 'Bob', 'port': 8002}).encode())
s.recv(1024)
s.close()
def handle(sock):
    try:
        data = json.loads(sock.recv(4096).decode())
        
        print("\nBob received : \nTransaction Data:", data['tx'],"\n")

        tx_bytes = json.dumps(data['tx']).encode()
        sig_bytes = bytes.fromhex(data['signature'])
        pub_key = rsa.PublicKey.load_pkcs1(data['pub_key'].encode())
        rsa.verify(tx_bytes, sig_bytes, pub_key)
        print("Verified TX:", data['tx'])
        sock.send(b"TX_VALID_ACK")
    except rsa.VerificationError:
        print("Alert: Transaction Tampered!")
        sock.send(b"TX_REJECTED")
    finally: sock.close()
srv = socket.socket()
srv.bind(('127.0.0.1', 8002))
srv.listen()
while True:
    sock, _ = srv.accept()
    threading.Thread(target=handle, args=(sock,)).start()
