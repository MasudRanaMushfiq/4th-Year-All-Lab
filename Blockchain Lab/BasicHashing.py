import hashlib

# SHA-256 Function
def sha256(message):
    return hashlib.sha256(message.encode()).hexdigest()

# Property 1: Deterministic
 
print("Property 1: Deterministic")

msg = "Hello Blockchain"

hash1 = sha256(msg)

hash2 = sha256(msg)

print("Message :", msg)
print("Hash 1  :", hash1)
print("Hash 2  :", hash2)

if hash1 == hash2:
    print("Result : Same input gives same hash.\n")

# Property 2: Avalanche Effect
 
print("Property 2: Avalanche Effect")

msg1 = "Hello Blockchain"

msg2 = "Hello blockchain"   # Changed only one letter (B → b)

print("Message 1 :", msg1)
print("Hash      :", sha256(msg1))

print()

print("Message 2 :", msg2)
print("Hash      :", sha256(msg2))

print("\nResult : A tiny change completely changes the hash.\n")


 
# Property 3: Fixed Length
 
print("Property 3: Fixed Length")

short = "Hi"

long = "This is a very long blockchain message for testing SHA256"

hash_short = sha256(short)
hash_long = sha256(long)

print("Short Hash Length :", len(hash_short))
print("Long Hash Length  :", len(hash_long))

print("\nResult : SHA-256 always produces 64 hexadecimal characters.\n")


 
# Property 4: One-Way Function
 
print("Property 4: One-Way Function")

message = "Blockchain"
digest = sha256(message)

print("Message :", message)
print("Hash    :", digest)

print("\nCan we recover the original message from this hash?")
print("Answer : No")

