from bitcoinlib.transactions import Transaction
from bitcoinlib.keys import Key

sender_key = Key()
receiver_key = Key()

print("Sender Private Key:")
print(sender_key.wif())

print("\nSender Address:")
print(sender_key.address())

print("\nReceiver Address:")
print(receiver_key.address())

previous_tx = Transaction()

# Give the sender an output of 200,000 satoshis
previous_tx.add_output(
    value=200000,
    address=sender_key.address()
)

print("\nPrevious transaction created.")

previous_txid = previous_tx.txid

print("\nPrevious Transaction ID:")
print(previous_txid)

tx = Transaction()

print("\nNew transaction created.")

tx.add_input(
    prev_txid=previous_txid,
    output_n=0
)

print("Input added.")

# Add output to send money to receiver
tx.add_output(
    value=100000,
    address=receiver_key.address()
)

print("Output added.")

# Sign the transaction
tx.sign(sender_key)

print("Transaction signed successfully.")

# Dynamically extract values from transactions
initial_balance = previous_tx.outputs[0].value  # Get initial balance from previous tx
amount_sent = tx.outputs[0].value               # Get amount sent from new tx
remaining_balance = initial_balance - amount_sent

print("\n" + "="*50)
print("MONEY FLOW SUMMARY")
print("="*50)

print("\n Sender's Initial Balance: {initial_balance:,} satoshis")
print(" Amount Sent to Receiver: {amount_sent:,} satoshis")
print(" Remaining Balance (Change): {remaining_balance:,} satoshis")

print("\nVerification: {initial_balance:,} = {amount_sent:,} + {remaining_balance:,}")

print("\nSigned Transaction HEX:")
print(tx.raw_hex())

