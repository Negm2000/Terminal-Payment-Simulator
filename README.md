# Payment terminal simulator

Console program in C that walks a card payment through the three parties involved: card, terminal and server. Project for Udacity's EgFWD Embedded Systems track (2022).

- **`Card/`**: reads and validates the cardholder name, PAN and expiry date.
- **`Terminal/`**: reads the transaction date and amount, checks the card has not expired, checks the PAN with the Luhn algorithm, and enforces a maximum amount.
- **`Server/`**: looks the account up, rejects blocked accounts and insufficient balances, updates the balance and saves the transaction.
- **`Application/`**: the flow that ties them together, plus test routines and a Luhn PAN generator used to fill the test accounts.

Every function returns an error enum, and the application prints why a transaction was declined.

```bash
cmake -B build && cmake --build build
```
