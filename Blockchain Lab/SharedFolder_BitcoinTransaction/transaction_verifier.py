import json
import hashlib
from ecdsa import VerifyingKey, SECP256k1

with open("I:\BC_Lab\SharedFolder_BitcoinTransaction\sharedFile.json", "r") as f:
    transaction = json.load(f)

signature = bytes.fromhex(transaction["signature"])
public_key = bytes.fromhex(transaction["public_key"])

data = {
    "sender": transaction["sender"],
    "receiver": transaction["receiver"],
    "amount": transaction["amount"]
}

message = json.dumps(data).encode()
tx_hash = hashlib.sha256(message).digest() 

vk = VerifyingKey.from_string(public_key, curve=SECP256k1)

try:
    vk.verify(signature, tx_hash)

    print("========== RECEIVER ==========")
    print("Sender   :", data["sender"])
    print("Receiver :", data["receiver"])
    print("Amount   :", data["amount"], "BTC")
    print()
    print("Transaction Verified")
    print("Bitcoin Received Successfully")

except:
    print("Invalid Signature")
    print("Transaction Rejected")