
import hashlib

class Block:
    def __init__(self, data, previous_hash):
        self.data = data
        self.previous_hash = previous_hash
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        return hashlib.sha256((self.data + self.previous_hash).encode()).hexdigest()


class Blockchain:
    def __init__(self):
        self.chain = [Block("Genesis Block", "0")]

    def add_block(self, data):
        self.chain.append(Block(data, self.chain[-1].hash))

    def validate_chain(self):
        for i in range(1, len(self.chain)):
            current = self.chain[i]
            previous = self.chain[i - 1]

            if current.hash != current.calculate_hash():
                print("Invalid hash for block", i)
                return False

            if current.previous_hash != previous.hash:
                print("Invalid previous hash for block", i)
                return False

        print("Blockchain is valid")
        return True

    def get_chain_length(self):
        return len(self.chain)

    def get_chain_hashrate(self):
        return sum(int(block.hash, 16) for block in self.chain)

    def check_for_51_percent_attack(self):
        total_hashrate = self.get_chain_hashrate()

        for i, block in enumerate(self.chain):
            block_hash = int(block.hash, 16)

            if block_hash / total_hashrate > 0.50:
                print("51% attack detected at block", i)
                return True

        print("No 51% attack detected")
        return False


print("Scenario 1: NO 51% ATTACK")

blockchain = Blockchain()
blockchain.add_block("Alice pays Bob 5 BTC")
blockchain.add_block("Bob pays Charlie 3 BTC")
blockchain.add_block("Charlie pays David 2 BTC")

print("Chain Length:", blockchain.get_chain_length())
blockchain.validate_chain()
blockchain.check_for_51_percent_attack()


print("\nScenario 2: 51% ATTACK")

blockchain2 = Blockchain()
blockchain2.add_block("Alice pays Bob 100000000000000000000000000000000000999999999999999915 BTC ")
blockchain2.add_block("Bob pays Charlie 1059 BTC")
blockchain2.add_block("Charlie pays David 1099 BTC")

blockchain2.check_for_51_percent_attack()

blockchain2.chain[1].hash = "f" * 64
blockchain2.validate_chain()

