
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"
#include "..\Server\server.h"
#include <stdio.h>
#include <string.h>

extern ST_accountsDB_t accountsDBArray[256];
extern ST_transaction_t transDBArray[256];
extern uint32_t accountSequenceNumber;
extern char *transStateLUT[];

int main(int argc, char *argv[]) {

    while (1) {
        printf("\nGo to test:\n"
               "(1) --> getTransactionDate\n"
               "(2) --> isCardExpired\n"
               "(3) --> getTransactionAmount\n"
               "(4) --> setMaxAmount\n"
               "(5) --> isBelowMaxAmount\n"
               "(6) --> isValidCardPAN\n");
        int choice;
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice) {
            case 1:
                printf("-------------Transaction Date Test-------------\n");
                ST_terminalData_t termData;
                uint8_t ERROR = getTransactionDate(&termData);
                switch (ERROR) {
                    case TERMINAL_OK:
                        printf("Valid Transaction Date: %s\n", termData.transactionDate);
                        break;
                    case WRONG_DATE:
                        printf("Wrong Date\n");
                        break;
                }
                break;

            case 2: // Card Expiration Date Test
                printf("-------------Card Expiration Date Test-------------\n");
                {
                    ST_cardData_t cardData;
                    ST_terminalData_t termData;
                    uint8_t _ERROR_;
                    do { _ERROR_ = getCardExpiryDate(&cardData); } while (_ERROR_ != OK);
                    do { _ERROR_ = getTransactionDate(&termData); } while (_ERROR_ != TERMINAL_OK);

                    uint8_t ERROR = isCardExpired(cardData, termData);
                    switch (ERROR) {
                        case TERMINAL_OK:
                            printf("Transaction Approved\n", termData.transactionDate);
                            break;
                        case EXPIRED_CARD:
                            printf("Card Expired\n");
                            break;
                    }
                }
                break;

            case 3: // Transaction Amount Test
                printf("-------------Transaction Amount Test-------------\n");
                {
                    ST_terminalData_t termData;
                    uint8_t ERROR = getTransactionAmount(&termData);
                    switch (ERROR) {
                        case TERMINAL_OK:
                            printf("Transaction Amount: %f\n", termData.transAmount);
                            break;
                        case INVALID_AMOUNT:
                            printf("Wrong Amount\n");
                            break;
                    }
                }
                break;
            default:
                break;

            case 4: // Set Max Amount Test
                printf("-------------Set Max Amount Test-------------\n");
                {
                    ST_terminalData_t termData;
                    uint8_t ERROR = setMaxAmount(&termData);
                    switch (ERROR) {
                        case TERMINAL_OK:
                            printf("Max Amount: %f\n", termData.maxTransAmount);
                            break;
                        case INVALID_MAX_AMOUNT:
                            printf("Wrong Amount\n");
                            break;
                    }
                }
                break;

            case 5: // Is Below Max Amount Test
                printf("--------------Is Below Max Amount Test---------------\n");
                {
                    ST_terminalData_t termData;
                    setMaxAmount(&termData);
                    getTransactionAmount(&termData);
                    uint8_t ERROR = isBelowMaxAmount(&termData);
                    switch (ERROR) {
                        case TERMINAL_OK:
                            printf("Transaction Approved\n");
                            break;
                        case EXCEED_MAX_AMOUNT:
                            printf("Transaction Exceeds Max Amount\n");
                            break;
                    }
                }
                break;

            case 6: // Is Valid Card PAN Test
                printf("--------------Is Valid Card PAN Test---------------\n");
                ST_cardData_t cardData;
                printf("Enter Card PAN: ");
                scanf("%s", cardData.primaryAccountNumber);
                {
                    uint8_t ERROR = isValidCardPAN(&cardData);
                    switch (ERROR) {
                        case OK:
                            printf("%s Passes Luhn\n", cardData.primaryAccountNumber);
                            break;
                        case WRONG_PAN:
                            printf("Invalid Card PAN, Luhn Check Failed\n");
                            break;
                    }
                }
                break;


        }
    }
}