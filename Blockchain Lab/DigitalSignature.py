import hashlib
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes
from cryptography.exceptions import InvalidSignature

message = b"Hello Blockchain"
# message = message.encode('utf-8')
# Hash the message
message_hash = hashlib.sha256(message).hexdigest()
print("Message:", message.decode())
print("SHA-256 Digest:", message_hash)

# Generate RSA keys
private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
public_key = private_key.public_key()

# Sign the message
signature = private_key.sign(
    message,
    padding.PKCS1v15(),
    hashes.SHA256()
)

print("\nDigital Signature:")
print(signature.hex())

# Verify the signature
try:
    public_key.verify(
        signature,
        message,
        padding.PKCS1v15(),
        hashes.SHA256()
    )
    print("\nSignature Verified Successfully!")
except InvalidSignature:
    print("\nVerification Failed!")


