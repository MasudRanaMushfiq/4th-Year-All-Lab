import json
import hashlib
from ecdsa import SigningKey, SECP256k1

private_key = "1E99423A4ED27608A15A2616DE1B5A7C6E3F4C4B5D4798365A793102748664B4"

sk = SigningKey.from_string(bytes.fromhex(private_key), curve=SECP256k1)

vk = sk.verifying_key

transaction = {
    "sender": "Alice",
    "receiver": "Bob",
    "amount": 5
}

message = json.dumps(transaction).encode()

# modified = message[:2] + "a" + message[3:]

tx_hash = hashlib.sha256(message).digest()

signature = sk.sign(tx_hash)

transaction["signature"] = signature.hex()
transaction["public_key"] = vk.to_string().hex()

with open("I:\BC_Lab\SharedFolder_BitcoinTransaction\sharedFile.json", "w") as f:
    json.dump(transaction, f, indent=4)

print("Transaction Sent Successfully")
print("Saved in Shared Folder")