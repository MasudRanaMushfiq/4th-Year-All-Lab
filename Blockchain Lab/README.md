# Blockchain Lab

This folder contains practical exercises and examples for blockchain and
cryptography concepts, including Bitcoin keys and transactions, hashing,
digital signatures, and Ethereum interaction.

## Requirements

- Python 3
- A terminal such as Git Bash, PowerShell, or a Linux shell

## Commands & What They Do

### Virtual Environment

Create an isolated Python environment before installing packages:

```bash
python -m venv .venv        # create the isolated environment folder
```

Activate it on Windows using Git Bash:

```bash
source .venv/Scripts/activate
```

Activate it on Linux or macOS:

```bash
source .venv/bin/activate
```

Exit the environment on any platform:

```bash
deactivate
```

### Install Packages

Run these commands after activating the virtual environment:

```bash
python -m pip install --upgrade pip   # update pip
pip install bitcoinlib                # Bitcoin transactions, keys, and wallets
pip install cryptography              # encryption, signing, and certificates
pip install pycryptodome              # hashing and cryptographic primitives
pip install web3                      # interact with Ethereum (optional)
```

The same commands work in Linux and macOS terminals. On Linux, if `python`
is not available, use `python3` when creating the environment and
`python -m pip` after activating it.

### Run Code

With the virtual environment activated, run the Bitcoin transaction example:

```bash
python practise.py
```

On Linux or macOS, use `python3 practise.py` if `python` is not mapped to
Python 3.

## Quick Job of Each Library

- **bitcoinlib** - Builds and signs Bitcoin transactions and manages keys and
  wallets.
- **cryptography** - Provides symmetric and asymmetric encryption, digital
  signatures, and certificates.
- **pycryptodome** - Provides hashing algorithms such as SHA and Keccak, as
  well as AES and RSA building blocks.
- **web3** - Communicates with Ethereum nodes, sends transactions, and reads
  smart contracts.
- **venv** - Keeps project packages isolated so they do not conflict with
  packages used by other projects.

## Example Script

[`practise.py`](./practise.py) creates Bitcoin keys, builds a previous
transaction and a new signed transaction, then prints the transaction ID,
money-flow information, and signed transaction hex.

