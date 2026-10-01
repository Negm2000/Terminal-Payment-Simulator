# Payment terminal simulator

Console program in C that walks a card payment through the three parties involved: card, terminal and server. Project for Udacity's EgFWD Embedded Systems track (2022).

- **`Card/`**: reads and validates the cardholder name, PAN and expiry date.
- **`Terminal/`**: reads the transaction date and amount, checks the card has not expired, checks the PAN with the Luhn algorithm, and enforces a maximum amount.
- **`Server/`**: looks the account up, rejects blocked accounts and insufficient balances, updates the balance and saves the transaction.
- **`Application/`**: the flow that ties them together, plus test routines and a Luhn PAN generator used to fill the test accounts.

## Example session

An approved payment, with what was typed shown after each prompt:

```
------------------Terminal Settings------------------
Enter the maximum transaction amount: 5000
Max Transaction Amount Set = 5000.000000
-----------------------------------------------------

Enter Card Holder Name: Test Cardholder Name One
Welcome Test Cardholder Name One

Enter Card Expiry Date: 12/27
Card Expiry Date: 12/27

Enter Card PAN: 4849925533374380
Card PAN: 4849925533374380
PAN passes the Luhn check

Would you like to enter the transaction date manually or use the current date?
1 --> Manual Entry
2 --> Get System Time
Enter your choice: 2
Transaction Date: 02/10/2026
VALID: Transaction Predates Expiration Date.

Enter transaction amount: 250
Transaction Amount: 250.000000

Valid Transaction Amount, Connecting to Server...

Transaction Approved
ACCOUNT NUMBER 2 --> New Account Balance: 3209.000000
```

Change the last digit of the card number and the terminal stops it before the server is contacted:

```
Enter Card PAN: 4849925533374381
Card PAN: 4849925533374381
Transaction Declined: PAN does not pass the Luhn check
```

Every function returns an error enum, and the application prints why a transaction was declined.

```bash
cmake -B build && cmake --build build
```
