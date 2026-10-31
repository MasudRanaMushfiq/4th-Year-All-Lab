from bitcoinlib.transactions import Transaction
from bitcoinlib.keys import Key

sender_key = Key()
receiver_key = Key()

print('Sender Key ', sender_key)
print("Receiver Key ", receiver_key)

prev_tx = Transaction()

prev_tx.add_output(
    value=200000,
    address = sender_key.address()
)

print("Previous Transactions Created")

prev_tx_id = prev_tx.txid

print("\nPrevious Tx Id: ", prev_tx_id)

tx = Transaction()

print("New Transaction Created")


tx.add_input(
    prev_txid = prev_tx_id,
    output_n = 0
)

tx.add_output(
    value=10000,
    address = receiver_key.address()
)

tx.sign(sender_key)

initial_balance = prev_tx.outputs[0].value  # Get initial balance from previous tx
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


