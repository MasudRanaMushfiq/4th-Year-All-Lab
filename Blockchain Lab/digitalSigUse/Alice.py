import socket, json, rsa
pub_key, priv_key = rsa.newkeys(512)
s = socket.socket()
s.connect(('127.0.0.1', 8000))
s.send(json.dumps({'name': 'Alice', 'port': 8001}).encode())
peers = json.loads(s.recv(1024).decode())
s.close()
if 'Bob' in peers:
    tx = {'from': 'Alice', 'to': 'Bob', 'amount': '5 BTC'}

    print("\nAlice send : \nTransaction Data:", tx,"\n")

    tx_bytes = json.dumps(tx).encode()
    signature = rsa.sign(tx_bytes, priv_key, 'SHA-256')
    # tx = {'from': 'Alice', 'to': 'Bob', 'amount': '500 BTC'}

    payload = {
        'tx': tx,
        'signature': signature.hex(),
        'pub_key': pub_key.save_pkcs1().decode()
    }
    s2 = socket.socket()
    s2.connect(('127.0.0.1', peers['Bob']))
    s2.send(json.dumps(payload).encode())
    print("Response:", s2.recv(1024).decode())
    s2.close()
