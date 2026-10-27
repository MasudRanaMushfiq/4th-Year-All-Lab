import hashlib
import base58
from ecdsa import SigningKey, SECP256k1


def sha256(data):
    return hashlib.sha256(data).digest()

def hash160(data):
    h = hashlib.new("ripemd160")
    h.update(sha256(data))
    return h.digest()

def checksum(data):
    return sha256(sha256(data))[:4]


def private_to_public(private_key):

    sk = SigningKey.from_string(bytes.fromhex(private_key), curve=SECP256k1)
    vk = sk.verifying_key

    x = vk.pubkey.point.x()
    y = vk.pubkey.point.y()

    prefix = b'\x02' if y % 2 == 0 else b'\x03'
    return prefix + x.to_bytes(32, "big")


def public_to_address(public_key):

    pub_hash = hash160(public_key)
    payload = b"\x00" + pub_hash
    check = checksum(payload)
    full = payload + check

    address = base58.b58encode(full).decode()

    print("\nOriginal Address :", address)

    modified = address[:2] + "a" + address[3:]
    print("Modified Address :", modified)

    return modified, pub_hash, payload, check, full


def validate_address(address):

    print("\n========== ADDRESS VALIDATION ==========")

    try:
        decoded = base58.b58decode(address)

        if len(decoded) != 25:
            print("Invalid Address Length")
            return

        payload = decoded[:-4]
        received = decoded[-4:]
        calculated = checksum(payload)

        print("Decoded Bytes :", decoded.hex())
        print("Version       :", hex(payload[0]))
        print("Public Hash   :", payload[1:].hex())
        print("Checksum      :", received.hex())
        print("Expected      :", calculated.hex())

        if received == calculated:
            print("\nResult : VALID ADDRESS")
        else:
            print("\nResult : INVALID ADDRESS")

    except Exception as e:
        print("Error :", e)


private_key = "1E99423A4ED27608A15A2616DE1B5A7C6E3F4C4B5D4798365A793102748664B4"

print("=" * 65)
print("BITCOIN ADDRESS GENERATION")
print("=" * 65)

public_key = private_to_public(private_key)

print("\nPrivate Key")
print(private_key)

print("\nPublic Key")
print(public_key.hex())

address, pub_hash, payload, check, full = public_to_address(public_key)

print("\n========== DETAILS ==========")
print("Public Key Hash :", pub_hash.hex())
print("Payload         :", payload.hex())
print("Checksum        :", check.hex())
print("Full Payload    :", full.hex())
print("Bitcoin Address :", address)

validate_address(address)