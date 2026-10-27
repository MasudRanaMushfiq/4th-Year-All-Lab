import hashlib
import time


class Block:
    def __init__(self, transactions, previous_hash):
        self.time = time.time()
        self.transactions = transactions
        self.previous_hash = previous_hash
        self.nonce = 0
        self.hash = self.get_hash()

    def get_hash(self):
        data = str(self.time) + str(self.transactions) + str(self.previous_hash) + str(self.nonce)
        return hashlib.sha256(data.encode()).hexdigest()

    def mine(self, difficulty):
        print("Mining block...")
        start = time.time()

        while self.hash[:difficulty] != "0" * difficulty:
            self.nonce += 1
            self.hash = self.get_hash()

        end = time.time()

        print("Block Mined")
        print("Hash     :", self.hash)
        print("Nonce    :", self.nonce)
        print("Time     :", end - start, "seconds")


difficulty = 4

print("Mining Genesis Block")

tx = [
    "transaction1",
    "transaction2",
    "transaction3"
]

block = Block(tx, "0" * 64)
block.mine(difficulty)


print("\nMining Block 1")

tx = [
    "transaction4",
    "transaction5",
    "transaction6"
]

block = Block(tx, block.hash)
block.mine(difficulty)


print("\nMining Block 2")

tx = [
    "transaction7",
    "transaction8",
    "transaction9"
]

block = Block(tx, block.hash)
block.mine(difficulty)

