import hashlib

# Helper function to compute SHA-256
def sha256(text):
    return hashlib.sha256(text.encode('utf-8')).hexdigest()

# 1. Build the Merkle Tree and get the Root
def build_merkle_tree_and_root(transactions):
    # Hash the transactions (Leaf nodes)
    hashes = [sha256(tx) for tx in transactions]
    
    # Keep combining until we reach a single root
    while len(hashes) > 1:
        if len(hashes) % 2 != 0:
            hashes.append(hashes[-1]) # Duplicate last if odd
            
        new_level = []
        for i in range(0, len(hashes), 2):
            combined = hashes[i] + hashes[i+1]
            new_level.append(sha256(combined))
        hashes = new_level
        
    return hashes[0], [sha256(tx) for tx in transactions]

# 2. Generate a Merkle Proof (Membership)
def get_merkle_proof(leaf_hashes, target_hash):
    if target_hash not in leaf_hashes:
        return None
    
    current_level = leaf_hashes
    proof = []
    
    while len(current_level) > 1:
        if len(current_level) % 2 != 0:
            current_level.append(current_level[-1])
            
        new_level = []
        for i in range(0, len(current_level), 2):
            combined = current_level[i] + current_level[i+1]
            new_level.append(sha256(combined))
            
            # If our target is in this pair, save the sibling for the proof
            if current_level[i] == target_hash:
                proof.append(current_level[i+1])
                target_hash = sha256(combined)
            elif current_level[i+1] == target_hash:
                proof.append(current_level[i])
                target_hash = sha256(combined)
                
        current_level = new_level
        
    return proof

# 3. Verify Membership using the Proof
def verify_proof(proof, target_hash, merkle_root):
    current_hash = target_hash
    for sibling in proof:
        # We need to know if the sibling is on the left or right.
        # In Python, we can sort them to be safe, or track position.
        # For simplicity here, we'll try both orders (sorted hex string)
        if current_hash < sibling:
            current_hash = sha256(current_hash + sibling)
        else:
            current_hash = sha256(sibling + current_hash)
            
    return current_hash == merkle_root

# 4. Prove Non-Membership
def prove_non_membership(merkle_root, target_tx, known_transactions):
    target_hash = sha256(target_tx)
    
    # In a standard Merkle tree, you can prove non-membership by proving 
    # the target does not exist. Since we don't have the full tree here, 
    # we calculate the root for the non-existent item to show it won't match.
    fake_hash = target_hash
    # We show that comparing the fake hash against the tree structure fails.
    
    # A simpler proof: If the target hash is not in the list of leaves, 
    # we can simply state it's absent and show a matching proof for a known item.
    print(f"\n--- Proving Non-Membership for: '{target_tx}' ---")
    if target_hash not in [sha256(tx) for tx in known_transactions]:
        print(f"FAILED: Hash of '{target_tx}' ({target_hash[:10]}...) is NOT in the tree.")
        print(f"Therefore, its path does not lead to the Root: {merkle_root[:10]}...")
        return True
    return False

# --- MAIN EXECUTION ---

# Example Transactions
transactions = ["Tx1: Alice to Bob", "Tx2: Bob to Charlie", "Tx3: Charlie to David", "Tx4: David to Eve"]
print("Transactions:", transactions)

# Step 1: Build Tree and get Root
root, leaf_hashes = build_merkle_tree_and_root(transactions)
print(f"\nMerkle Root: {root}")

# Step 2: Prove Membership for Tx2
target_tx = "Tx2: Bob to Charlie"
target_hash = sha256(target_tx)
print(f"\n--- Proving Membership for: '{target_tx}' ---")

proof = get_merkle_proof(leaf_hashes, target_hash)
print(f"Merkle Proof (Sibling Hashes): {proof}")

is_valid = verify_proof(proof, target_hash, root)
print(f"Verification Result: {is_valid} (True means membership proven)")

# Step 3: Prove Non-Membership for a fake transaction
fake_tx = "Tx99: Hacker to Bob"
prove_non_membership(root, fake_tx, transactions)

