//
// Created by Karim Negm on 9/11/2022.
//
#ifndef EGFWD_CARD_H
#define EGFWD_CARD_H
#include <stdint.h>

typedef enum EN_cardError_t // Definition of the card error type
{
    OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

typedef struct ST_cardData_t { // Structure of the card data
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[21];
    uint8_t cardExpirationDate[6];
} ST_cardData_t;


EN_cardError_t getCardHolderName(ST_cardData_t *cardData); // Function that gets and stores the cardholder name and returns the error code.
EN_cardError_t getCardPAN(ST_cardData_t *cardData);  // Function that stores the card PAN and returns the error code.
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData); // Function that stores the card expiry date and returns the error code.

#endif //EGFWD_CARD_H
