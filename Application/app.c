//
// Created by Karim Negm on 9/11/2022.
//
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ST_accountsDB_t accountsDBArray[256];
extern ST_transaction_t transDBArray[256];
extern uint32_t accountSequenceNumber;
extern char *transStateLUT[];

void appStart(void) {

    ST_cardData_t *cardData = (ST_cardData_t *) malloc(sizeof(ST_cardData_t));
    ST_terminalData_t *termData = (ST_terminalData_t *) malloc(sizeof(ST_terminalData_t));
    EN_cardError_t cardError;
    EN_terminalError_t termError;



// Terminal Setup
    do {
        switch (termError = setMaxAmount(termData)) {
            case INVALID_MAX_AMOUNT:
                printf("Max Transaction Amount Must Be Greater than Zero.\n");
                break;

            case TERMINAL_OK:
                printf("Max Transaction Amount Set = %f\n", termData->maxTransAmount);
                printf("-----------------------------------------------------\n\n");
                break;

            default:
                break;
        }
    } while (termError != TERMINAL_OK);


    do {
        switch (cardError = getCardHolderName(cardData)) {
            case WRONG_NAME:
                printf("Name must be between 20 to 24 characters\n");
                break;

            case OK:
                printf("Welcome %s\n", cardData->cardHolderName);
                break;

            default:
                break;
        }
    } while (cardError != OK);

    do {
        switch (cardError = getCardExpiryDate(cardData)) {
            case WRONG_EXP_DATE:
                printf("Invalid Date must be in the format MM/YY\n");
                break;

            case OK:
                printf("Card Expiry Date: %s\n", cardData->cardExpirationDate);
                break;

            default:
                break;
        }
    } while (cardError != OK);

    do {
        switch (cardError = getCardPAN(cardData)) {

            case WRONG_PAN:
                printf("PAN must be 16 to 19 digits\n");
                break;

            case OK:
                printf("Card PAN: %s", cardData->primaryAccountNumber);
                break;

            default:
                break;
        }
    } while (cardError != OK);

    switch (isValidCardPAN(cardData)) {
        case INVALID_CARD:
            printf("Transaction Declined: PAN does not pass the Luhn check\n");
            return;

        case TERMINAL_OK:
            printf("PAN passes the Luhn check\n\n");
            break;
    }

    switch (getTransactionDate(termData)) {
        case TERMINAL_OK:
            printf("Transaction Date: %s\n", termData->transactionDate);
            break;

        case WRONG_DATE:
            printf("Invalid Date must be in the format DD/MM/YYYY\n");
            return;

        default:
            break;
    }

    switch (isCardExpired(*cardData, *termData)) {
        case TERMINAL_OK:
            printf("VALID: Transaction Predates Expiration Date.\n\n");
            break;

        case EXPIRED_CARD:
            printf("Transaction Declined: Card is expired.\n");
            return;

        default:
            break;

    }

    do {
        switch (termError = getTransactionAmount(termData)) {
            case TERMINAL_OK:
                printf("Transaction Amount: %f\n", termData->transAmount);
                break;

            case INVALID_AMOUNT:
                printf("Invalid Amount: Transaction Must Be Greater Than Zero.\n");
                break;

            default:
                break;
        }
    } while (termError != TERMINAL_OK);


    switch (isBelowMaxAmount(termData)) {
        case EXCEED_MAX_AMOUNT:
            printf("Transaction Declined: Transaction Exceeds Max Transaction Amount\n");
            return;

        case TERMINAL_OK:
            printf("\nValid Transaction Amount, Connecting to Server...\n\n");
            
            
            break;
    }

    ST_transaction_t *transData = (ST_transaction_t *) malloc(sizeof(ST_transaction_t));
    transData->cardHolderData = *cardData;
    transData->terminalData = *termData;

    switch (receiveTransactionData(transData)) {
        case APPROVED:
            printf("Transaction Approved\n");
            printf("ACCOUNT NUMBER %d --> New Account Balance: %f\n\n", accountSequenceNumber + 1,
                   accountsDBArray[accountSequenceNumber].balance);
            break;

        case FRAUD_CARD:
            printf("Account Does Not Exist.\n\n");
            break;

        case DECLINED_STOLEN_CARD:
            printf("Transaction Declined Stolen Card\n\n");
            break;
           

        case DECLINED_INSUFFICIENT_FUND:
            printf("Transaction Declined Insufficient Fund\n\n");
            break;

        case INTERNAL_SERVER_ERROR:
            printf("Transaction Internal Server Error\n\n");
            break;
    }
}


// Luhn Generator for Testing Purposes ---> Fills accountsDBArray with 16 digit Luhn PANs

void cardGenerator(void) {
    // 16 Luhn credit card generator
    ST_cardData_t *temp = malloc(sizeof(ST_cardData_t));
    for (int i = 0; i < 255; i++) {
        do {
            for (int j = 0; j < 16; j++) { temp->primaryAccountNumber[j] = (rand() % 10) + '0'; }
            temp->primaryAccountNumber[16] = '\n';
            temp->primaryAccountNumber[17] = '\0';
        } while (isValidCardPAN(temp) == INVALID_CARD);

        strcpy(accountsDBArray[i].primaryAccountNumber, temp->primaryAccountNumber);
        accountsDBArray[i].balance = (rand() % 10000) + 1;
        accountsDBArray[i].state = (rand() % 2);
       printf("ACCOUNT NUMBER %d PAN: %sBalance: %f State: %d\n\n", i + 1, accountsDBArray[i].primaryAccountNumber,
              accountsDBArray[i].balance, accountsDBArray[i].state);
    }
}




int main() {
    cardGenerator();
    appStart();
    
    

    uint8_t choice;
    do {
        printf("Would you like any other services?\n");
        printf("(1) ---> New Transaction\n(2) ---> View a Transaction\n(3) ---> Exit\n");
        scanf("%c", &choice);
        fflush(stdin);

        switch (choice) {
            case '1':
                
                appStart();
                break;

            case '2':
                
                printf("Enter Transaction Number: ");
                uint32_t transactionNumber;
                scanf("%d", &transactionNumber);
                fflush(stdin);
                ST_transaction_t *transData = (ST_transaction_t *) malloc(sizeof(ST_transaction_t));
                if (getTransaction(transactionNumber, transData) != TRANSACTION_NOT_FOUND) {
                    printf("Transaction Number: %d\n", transData->transactionSequenceNumber);
                    printf("Transaction PAN: %s", transData->cardHolderData.primaryAccountNumber);
                    printf("Transaction Amount: %f\n", transData->terminalData.transAmount);
                    printf("Transaction Date: %s\n", transData->terminalData.transactionDate);
                    printf("Transaction Status: %s\n", transStateLUT[transData->transState]);
                } else {
                    printf("Transaction Not Found\n");
                }
                
                
                break;

            default:
                break;
        }
    } while (choice != '3');
    printf("Thank you for using our services\n");
    
}
