//
// Created by Karim Negm on 9/11/2022.
//

#include "terminal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
    printf("Would you like to enter the transaction date manually or use the current date?\n"
           "1 --> Manual Entry \n"
           "2 --> Get System Time \n");
    uint8_t choice;
    do {
        printf("Enter your choice: ");
        scanf("%hhu", &choice);
        fflush(stdin);
    } while (choice != 1 && choice != 2);

    switch (choice) {
        case MANUAL_ENTRY: {
            uint8_t buff[12];
            printf("Enter transaction date (DD/MM/YYYY): ");
            fgets(buff, 12, stdin);
            if (strcspn(buff, "\n") != 10) { return WRONG_DATE; }
            if (buff[2] != '/' || buff[5] != '/') { return WRONG_DATE; }
            for (uint8_t i = 0; i < 10; i++) {
                if (i == 2 || i == 5) { continue; }
                if (!isdigit(buff[i])) { return WRONG_DATE; }
            }
            uint8_t DD[3] = {buff[0], buff[1], '\0'};
            uint8_t MM[3] = {buff[3], buff[4], '\0'};
            uint8_t YYYY[5] = {buff[6], buff[7], buff[8], buff[9], '\0'};
            uint8_t DD_int = atoi(DD);
            uint8_t MM_int = atoi(MM);
            uint16_t YYYY_int = atoi(YYYY);
            if (DD_int > 31 || DD_int == 0) { return WRONG_DATE; }
            if (MM_int > 12 || MM_int == 0) { return WRONG_DATE; }
            if (YYYY_int < 2000 || YYYY_int > 2099) { return WRONG_DATE; }
            strcpy(termData->transactionDate, buff);
            return TERMINAL_OK;
        }
        case GET_SYSTEM_TIME: {
            time_t Date = time(NULL);
            strftime(termData->transactionDate, 11, "%d/%m/%Y", localtime(&Date));
            return TERMINAL_OK;
        }
        default:
            return WRONG_DATE;
    }
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
    uint8_t Card_MM[3] = {cardData.cardExpirationDate[0], cardData.cardExpirationDate[1], '\0'};
    uint8_t Card_YY[3] = {cardData.cardExpirationDate[3], cardData.cardExpirationDate[4], '\0'};
    uint8_t Trans_MM[3] = {termData.transactionDate[3], termData.transactionDate[4], '\0'};
    uint8_t Trans_YY[3] = {termData.transactionDate[8], termData.transactionDate[9], '\0'};

    if (strcmp(Card_YY, Trans_YY) < 0) { return EXPIRED_CARD; }
    else if (strcmp(Card_YY, Trans_YY) == 0) {
        if (strcmp(Card_MM, Trans_MM) < 0) { return EXPIRED_CARD; }
        else { return TERMINAL_OK; }
    } else { return TERMINAL_OK; }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
    float temp;
    printf("Enter transaction amount: ");
    scanf("%f", &temp);
    fflush(stdin);
    if (temp <= 0) { return INVALID_AMOUNT; }
    else {
        termData->transAmount = temp;
        return TERMINAL_OK;
    }
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
    if (termData->transAmount > termData->maxTransAmount) { return EXCEED_MAX_AMOUNT; }
    else { return TERMINAL_OK; }
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
    uint8_t panLength = strcspn(cardData->primaryAccountNumber, "\n");
    uint16_t sum = 0;
    for (int i = 0; i < panLength; i++) {
        if (!isdigit(cardData->primaryAccountNumber[i])) { return INVALID_CARD; } // If any of the characters is not a digit, return an error.

        if (panLength % 2 != 0) {
                 if (i % 2 != 0) {
                    uint8_t temp = (cardData->primaryAccountNumber[i] - '0') * 2;
                    if (temp > 9) { temp -= 9; }
                    sum += temp;
                } else { sum += cardData->primaryAccountNumber[i] - '0'; }
        }

        else if (i % 2 == 0) { // If the index is even
            uint8_t digit = (cardData->primaryAccountNumber[i] - '0') * 2; // Multiply the digit by 2
            if (digit > 9) { digit -= 9; } // If the result is greater than 9, subtract 9
            sum += digit; // Add the result to the sum
        } else { sum += (cardData->primaryAccountNumber[i] - '0'); } // If the index is odd, add the digit to the sum
    }

    if (sum % 10 == 0) { return TERMINAL_OK; } // If the sum is divisible by 10, the PAN is valid
    else { return INVALID_CARD; } // If the sum is not divisible by 10, the PAN is invalid
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData) {
    float temp;
    printf("\n\n------------------Terminal Settings------------------\n"
           "Enter the maximum transaction amount: ");
    scanf("%f", &temp);
    fflush(stdin);
    if (temp <= 0) { return INVALID_MAX_AMOUNT; }
    else {
        termData->maxTransAmount = temp;
        return TERMINAL_OK;
    }
}