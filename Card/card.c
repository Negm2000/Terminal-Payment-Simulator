//
// Created by Karim Negm on 9/11/2022.
//

#include "card.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 24
#define MIN_NAME_LENGTH 20

EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
    printf("Enter Card Holder Name: ");
    uint8_t buffer[26]; // Buffer to store the cardholder name
    fgets(buffer, 26, stdin); // Get the cardholder name from the user. The maximum length is 25 characters + NULL.
    fflush(stdin); // Clear the input buffer to avoid any unwanted characters from overflowing to the next entry.

    uint8_t nameLength = strcspn(buffer,"\n"); // Get the length of the cardholder name (Without the new line character).

    if (nameLength < MIN_NAME_LENGTH) { return WRONG_NAME; } // If the name is less than 20 characters, return an error.

    else if (nameLength >
             MAX_NAME_LENGTH) { return WRONG_NAME; } // If the name is more than 24 characters, return an error

    else {
        strcpy(cardData->cardHolderName, buffer);
        return OK;
    } // If the name is between 20 and 24 characters, store the name in the card data structure and return OK.
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Enter Card Expiry Date: ");
    uint8_t buffer[7];
    fgets(buffer, 7, stdin);
    fflush(stdin); // Clear the input buffer to avoid any unwanted characters from overflowing to the next entry.

    for (int i = 0; i < 5; i++) {
        if (i == 2) { continue; } // Skip the '/' character.
        if (!isdigit(buffer[i])) { return WRONG_EXP_DATE; } // If any of the characters is not a digit, return an error.
    }
    if (buffer[2] != '/') { return WRONG_EXP_DATE; } // If the expiry date is not in the format MM/YY, return an error.
    else if (buffer[0] != '0' && buffer[0] != '1') { return WRONG_EXP_DATE; } // If the month is greater than 12, return an error.
    else if (buffer[0] == '1' && buffer[1] > '2')  { return WRONG_EXP_DATE; } // If the month is greater than 12, return an error.
    else if (buffer[0] == '0' && buffer[1] == '0')  { return WRONG_EXP_DATE; } // Month can't be 00
    else if (strcspn(buffer, "\n") != 5) { return WRONG_EXP_DATE; } // If the expiry date is not in the format MM/YY, return an error.

    else{
        strcpy(cardData->cardExpirationDate, buffer);
        return OK;
    } // If the expiry date is in the format MM/YY, store the expiry date in the card data structure and return OK.
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){

    printf("Enter Card PAN: ");
    uint8_t buffer[22];
    fgets(buffer, 21, stdin);
    fflush(stdin); // Clear the input buffer to avoid any unwanted characters from overflowing to the next entry.

    uint8_t panLength = strcspn(buffer, "\n"); // Get the length of the PAN (Without the new line character).

    if (panLength > 19 || panLength < 16) { return WRONG_PAN; }
    for (uint8_t i = 0; i < panLength; i++) {
        if (!isalnum(buffer[i])) { return WRONG_PAN; } // If the PAN is not an alpha or numeric character, return an error.
    }
    strcpy(cardData->primaryAccountNumber, buffer);
    return OK;// If the PAN is alphanumeric and between 16 and 19 characters, store the PAN in the card data structure and return OK.
}


