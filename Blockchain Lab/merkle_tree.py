import hashlib;

# Function to calculate SHA-256 hash
def sha256(data):
    return hashlib.sha256(data.encode()).hexdigest()


# Function to calculate Merkle Root
def merkle_root(transactions):

    # Step 1: Hash all transactions
    hashes = [sha256(tx) for tx in transactions]

    print("Initial Transaction Hashes:\n")
    for h in hashes:
        print(h)

    print("\nBuilding Merkle Tree...\n")

    # Step 2: Continue until one hash remains
    while len(hashes) > 1:

        # If odd number of hashes, duplicate the last one
        if len(hashes) % 2 == 1:
            hashes.append(hashes[-1])

        new_hashes = []

        # Combine two hashes at a time
        for i in range(0, len(hashes), 2):

            combined = hashes[i] + hashes[i + 1]

            parent_hash = sha256(combined)

            print("Left :", hashes[i])
            print("Right:", hashes[i + 1])
            print("Parent Hash:", parent_hash)
            print("-" * 70)

            new_hashes.append(parent_hash)

        hashes = new_hashes

    return hashes[0]


# -------------------------
# Main Program
# -------------------------

transactions = [
    "Alice pays Bob 10 BTC",
    "Bob pays Charlie 9 BTC",
    "Charlie pays David 2 BTC",
    "David pays Eva 1 BTC"
]

root = merkle_root(transactions)

print("\nMerkle Root tree:")

print(root)

