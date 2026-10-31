# Bitcoin Transaction Signing — Explained

A line-by-line explanation of `test.py`, which builds a Bitcoin transaction, signs it with
`SIGHASH_ALL`, and verifies the signature.

## Contents

1. [About `bitcoinlib`](#1-about-bitcoinlib)
2. [The complete program](#2-the-complete-program)
3. [Stage 1 — Imports](#3-stage-1--imports)
4. [Stage 2 — Understanding SIGHASH](#4-stage-2--understanding-sighash)
5. [Stage 3 — Generate a key pair](#5-stage-3--generate-a-key-pair)
6. [Stage 4 — Build the transaction](#6-stage-4--build-the-transaction)
7. [Stage 5 — Hash and sign](#7-stage-5--hash-and-sign)
8. [Stage 6 — Print and verify](#8-stage-6--print-and-verify)
9. [The whole flow in one picture](#9-the-whole-flow-in-one-picture)
10. [Reference tables](#10-reference-tables)
11. [Common confusions](#11-common-confusions)
12. [Exam summary](#12-exam-summary)

---

## 1. About `bitcoinlib`

`bitcoinlib` is a full Bitcoin library covering
**keys, transactions, scripts, addresses, wallets, and hashing**.



### `Transaction`

Represents a Bitcoin transaction. Structurally:

```text
Transaction
│
├── Inputs
│    ├── Input 0
│    ├── Input 1
│    └── ...
│
├── Outputs
│    ├── Output 0
│    ├── Output 1
│    └── ...
│
└── Signatures
```

### `Key`

Provides Bitcoin public/private key functionality. Bitcoin uses **asymmetric cryptography**:

```text
                 Key Pair
                /        \
               /          \
       Private Key      Public Key
           │                 │
           │                 │
        Signing          Verification
```

- The **private key is secret** — it creates signatures.
- The **public key can be shared** — it verifies signatures.

### The two print lines

```python
print("BITCOIN TRANSACTION SIGNING")
print("=" * 40)
```

`"=" * 40` produces `========================================`. Both lines are purely cosmetic
and have no effect on the transaction.

---

## 4. Stage 2 — Understanding SIGHASH

```python
SIGHASH_ALL = 1
```

**SIGHASH = Signature Hash.** It decides **which parts of a transaction are included in the hash
that gets signed**.

### First, what is a hash?

A hash function takes data and produces a fixed-size value:

```text
Transaction Data
      │
      ▼
    Hash
      │
      ▼
"abc123..."
```

If the data changes even slightly, the hash changes completely.

### What `SIGHASH_ALL` means

The signature commits to **all inputs and all outputs**:

```text
Transaction
│
├── Input 0 ──┐
├── Input 1 ──┤
│             │
├── Output 0 ─┤──→ SIGHASH_ALL
└── Output 1 ─┘
```

**Consequence:** if anyone modifies a committed part of the transaction after signing, the
signature no longer validates. This is what makes the transaction tamper-evident.

Other modes exist (see [reference tables](#10-reference-tables)); `SIGHASH_ALL` is by far the
most common and the most restrictive.

---

## 5. Stage 3 — Generate a key pair

```python
key = Key()
```

Creates a new key object containing both halves of a key pair:

```text
Key()
 │
 ├── Private Key
 │
 └── Public Key
```

### Why do we need a private key?

Bitcoin needs proof that whoever spends a previous output is authorized to do so. The private
key provides that proof by producing a digital signature:

```text
Transaction
     │
     ▼
Transaction Hash
     │
     +
     │
Private Key
     │
     ▼
Digital Signature
```

> **Note:** `Key()` generates a *random* key each run, so the signature hash and raw hex printed
> at the end will be different every time you execute the program.

---

## 6. Stage 4 — Build the transaction

### 6.1 Create an empty transaction

```python
tx = Transaction()
```

```text
tx
│
├── Inputs:  none
├── Outputs: none
└── Signatures: none
```

### 6.2 Dummy previous transaction ID

```python
prev_txid = "1" * 64
```

Produces:

```text
1111111111111111111111111111111111111111111111111111111111111111
```

A real transaction ID is a **64-character hexadecimal string** (32 bytes), which is why 64 is
used here. This one is a **placeholder** — the program demonstrates the *signing process*, it is
not spending a real UTXO.

### 6.3 Add the input

```python
tx.add_input(
    prev_txid=prev_txid,
    output_n=0,
    keys=key.public(),
    value=110000
)
```

This says: *"I want to spend a particular output from a previous transaction."*

#### `prev_txid` — which previous transaction

Identifies the transaction whose output we are spending.

#### `output_n=0` — which output of that transaction

A previous transaction can have many outputs:

```text
Previous Transaction
│
├── Output 0 → Alice → 50,000 sat
├── Output 1 → Bob   → 110,000 sat
└── Output 2 → Carol → 200,000 sat
```

`output_n=0` means *"spend output number 0."* It is **not** an amount, and **not** an output of
the current transaction.

#### `keys=key.public()` — the public key for this input

`key.public()` returns the public key derived from the private key. Associating it with the
input lets the library construct and later check the unlocking information:

```text
PRIVATE KEY
     │
     │ signs
     ▼
 SIGNATURE
     │
     │ checked using
     ▼
PUBLIC KEY
```

> **Honest caveat:** in a *real* transaction, supplying a public key does not by itself prove you
> own the UTXO. The previous output's locking script, value, and script type all matter. For this
> assignment the conceptual meaning is enough.

#### `value=110000` — the amount held by the UTXO being spent

This is **required**. Modern (SegWit) signature hashing commits to the input's amount, so without
it the library raises:

```text
TransactionError: Need value of input 0 to create transaction signature
```

### 6.4 Receiver and amount

```python
receiver = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"
amount = 100000
```

`receiver` is a **legacy P2PKH mainnet address** (it starts with `1`).

`amount` is in **satoshis**, Bitcoin's smallest unit:

```text
1 BTC = 100,000,000 satoshis (10 Cror)

100,000 satoshis = 0.001 BTC
```

### 6.5 Add the output

```python
tx.add_output(
    value=amount,
    address=receiver
)
```

`value` is how much to send; `address` is where it goes.

```text
INPUT
110,000 sat
    │
    ▼
┌────────────────────┐
│    Transaction     │
└────────────────────┘
    │
    ▼
OUTPUT
100,000 sat
    │
    ▼
Receiver Address
```

The missing 10,000 satoshi is explained in [common confusions](#11-common-confusions).

---

## 7. Stage 5 — Hash and sign

### 7.1 Create the signature hash

```python
signature_hash = tx.signature_hash(
    sign_id=0,
    hash_type=SIGHASH_ALL
)
```

This builds the exact hash that will be signed.

- **`sign_id=0`** — build the hash for **input 0** (our transaction only has one input).
- **`hash_type=SIGHASH_ALL`** — which parts of the transaction to commit to; resolves to `1`.

```text
Transaction
     │
     ▼
SIGHASH_ALL rules
     │
     ▼
Data to be signed
     │
     ▼
Cryptographic Hash
```

### 7.2 Sign the input

```python
tx.sign(
    key.private_byte,
    index_n=0,
    hash_type=SIGHASH_ALL
)
```

This is where the **private key** is actually used.

- **`key.private_byte`** — the private key in byte form.
- **`index_n=0`** — which input to sign.
- **`hash_type=SIGHASH_ALL`** — same signing mode as above.

```text
Signature Hash
      +
Private Key
      │
      ▼
Digital Signature
```

> ⚠️ **Watch the parameter names.** `signature_hash()` uses `sign_id`, but `sign()` uses
> `index_n`. Both mean *"the first transaction input"* — the inconsistency comes from the
> `bitcoinlib` API. Passing `sign_id` to `sign()` raises
> `TypeError: Transaction.sign() got an unexpected keyword argument 'sign_id'`.

The library then stores the resulting signature inside the transaction's input.

---

## 8. Stage 6 — Print and verify

```python
print("\nTransaction Details")
print("-------------------")
print("Receiver :", receiver)
print("Amount   :", amount, "satoshi")
print("SIGHASH  : SIGHASH_ALL (1)")
```

`\n` inserts a blank line. These lines just summarize what was built — printing the SIGHASH flag
explicitly is useful for the assignment.

### Signature hash

```python
print(signature_hash.hex())
```

`.hex()` converts the raw binary hash into a readable hexadecimal string.

### Raw transaction

```python
print(tx.raw_hex())
```

`raw_hex()` serializes the whole signed transaction into the byte format that would actually be
broadcast to the Bitcoin network:

```text
Transaction Object
       │
       ▼
Serialization
       │
       ▼
Raw Transaction Bytes
       │
       ▼
Hexadecimal   →   0100000001....
```

### Verification

```python
print(tx.verify())
```

Checks the signature against the public key and the transaction data:

```text
Transaction
     │
     ▼
Check Signature
     │
     ▼
  Valid?
  /     \
Yes      No
 │        │
True     False
```

A correct run prints **`True`**.

---

## 9. The whole flow in one picture

**The key side:**

```text
                 Key()
                  │
          ┌───────┴────────┐
          ▼                ▼
    Private Key        Public Key
          │                │
          │                ▼
          │          Add to Input
          ▼
     Sign Transaction
          │
          ▼
      Signature
```

**The money side:**

```text
Previous Transaction
        │
        │ prev_txid
        │ output_n = 0
        ▼
      INPUT
        │
        │ 110,000 sat
        ▼
   TRANSACTION
        │
        │ SIGHASH_ALL
        ▼
 Signature Hash
        │
        │ + Private Key
        ▼
    Signature
        │
        ▼
      OUTPUT
        │
        │ 100,000 sat
        ▼
Receiver Address
```

---

## 10. Reference tables

### SIGHASH modes

| SIGHASH                | Value  | Meaning                              |
| ---------------------- | -----: | ------------------------------------ |
| `SIGHASH_ALL`          | `1`    | Sign all inputs and outputs          |
| `SIGHASH_NONE`         | `2`    | Sign inputs, but not outputs         |
| `SIGHASH_SINGLE`       | `3`    | Sign corresponding input/output      |
| `SIGHASH_ANYONECANPAY` | `0x80` | Modifier: commit to only one input   |

`ANYONECANPAY` is a **flag combined with** one of the first three (e.g. `1 | 0x80 = 0x81`), not a
standalone mode.

### Bitcoin address types

A Bitcoin address is **not** a fixed length — it depends on the type and encoding.

| Address type  | Typical prefix | Example                              | Length     |
| ------------- | -------------- | ------------------------------------ | ---------- |
| Legacy P2PKH  | `1`            | `1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa` | 34 chars   |
| Legacy P2SH   | `3`            | `3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy` | 34 chars   |
| SegWit P2WPKH | `bc1q`         | `bc1q...`                            | 42 chars   |
| SegWit P2WSH  | `bc1q`         | `bc1q...`                            | 62 chars   |
| Taproot       | `bc1p`         | `bc1p...`                            | 62 chars   |

### Units

```text
1 BTC   = 100,000,000 satoshi
0.001 BTC =     100,000 satoshi
```

---

## 11. Common confusions

### `value=110000` vs `amount=100000`

These are two **different** amounts — do not mix them up.

| Variable        | Where           | Meaning                                     |
| --------------- | --------------- | ------------------------------------------- |
| `value=110000`  | in `add_input`  | The previous UTXO **contains** 110,000 sat  |
| `amount=100000` | in `add_output` | **Send** 100,000 sat to the receiver        |

```text
Input:      110,000 sat
Output:     100,000 sat
Difference:  10,000 sat
```

Because there is **no change output**, that 10,000 satoshi difference is conceptually the
**transaction fee** (in a real transaction it would go to the miner).

### `sign_id` vs `index_n`

Both identify **input 0**, but they belong to different methods:

| Method               | Parameter  |
| -------------------- | ---------- |
| `tx.signature_hash()`| `sign_id`  |
| `tx.sign()`          | `index_n`  |

### `output_n` is not an amount

`output_n=0` selects **which output of the previous transaction** you are spending. It has
nothing to do with money or with your own transaction's outputs.

---

## 12. Exam summary

If asked to explain the program, focus on these seven lines:

| Line                            | What it does                                                        |
| ------------------------------- | ------------------------------------------------------------------- |
| `key = Key()`                   | Generates the key pair used for signing                             |
| `tx = Transaction()`            | Creates a new, empty Bitcoin transaction                            |
| `SIGHASH_ALL = 1`               | Selects the SIGHASH_ALL signature-hash mode                         |
| `tx.add_input(...)`             | Specifies which previous UTXO is being spent                        |
| `tx.add_output(...)`            | Specifies the receiver and the amount                               |
| `signature_hash = tx.signature_hash(...)` | Hashes the transaction data per the SIGHASH rules          |
| `tx.sign(...)`                  | Uses the private key to digitally sign the input                    |

**The one-sentence version:**

> The **input** tells Bitcoin where the coins come from, the **output** tells Bitcoin where they
> go, **SIGHASH** determines what transaction data the signature commits to, and the **private
> key** creates that signature.
