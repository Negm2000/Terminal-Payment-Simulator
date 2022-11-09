//
// Created by Karim Negm on 9/11/2022.
//

#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

ST_accountsDB_t accountsDBArray[256] = {};
ST_transaction_t transDBArray[256] = {0};
uint32_t accountSequenceNumber = 0;
uint32_t transSequenceNumber = 0;
char* transStateLUT[] = {"APPROVED", "DECLINED_INSUFFICIENT_FUND", "DECLINED_STOLEN_CARD", "INTERNAL_SERVER_ERROR"};


EN_serverError_t isValidAccount(ST_cardData_t *cardData) {
    EN_serverError_t ret = ACCOUNT_NOT_FOUND;
    for (int i = 0; i < 255; i++) {
        if (strcmp(&accountsDBArray[i].primaryAccountNumber, &cardData->primaryAccountNumber) == 0) {
            ret = SERVER_OK;
            accountSequenceNumber = i;
            break;
        }
    }
    return ret;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData) {
    if (termData->transAmount <= accountsDBArray[accountSequenceNumber].balance) {
        return SERVER_OK;
    } else {
        return LOW_BALANCE;
    }
}

EN_serverError_t saveTransaction(ST_transaction_t *transData) {
    if (transSequenceNumber > 255) {
        return SAVING_FAILED;
    }
    transData->transactionSequenceNumber = transSequenceNumber;
    transDBArray[transSequenceNumber] = *transData;
    transSequenceNumber++;
    return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData) {
    if (transactionSequenceNumber > 255 || (transactionSequenceNumber > transSequenceNumber - 1)) {
        return TRANSACTION_NOT_FOUND;
    }
    *transData = transDBArray[transactionSequenceNumber];
    return SERVER_OK;
}

EN_serverError_t isBlockedAccount(uint32_t Account_Number) {
    if (accountsDBArray[Account_Number].state == BLOCKED) {
        return BLOCKED_ACCOUNT;
    } else {
        return SERVER_OK;
    }
}

EN_transState_t receiveTransactionData(ST_transaction_t *transData) {

    EN_serverError_t Error;
    EN_transState_t transState = APPROVED;

    Error = isValidAccount(&transData->cardHolderData);
    if (Error != SERVER_OK) {
        transState = FRAUD_CARD;
        goto SaveTransaction;
    }

    Error = isBlockedAccount(accountSequenceNumber);
    if (Error != SERVER_OK) {
        transState = DECLINED_STOLEN_CARD;
        goto SaveTransaction;
    }

    Error = isAmountAvailable(&transData->terminalData);
    if (Error != SERVER_OK) {
        transState = DECLINED_INSUFFICIENT_FUND;
        goto SaveTransaction;
    } else { accountsDBArray[accountSequenceNumber].balance -= transData->terminalData.transAmount; }

    SaveTransaction:
    transData->transState = transState;
    Error = saveTransaction(transData);
    if (Error != SERVER_OK) {
        transData->transState = INTERNAL_SERVER_ERROR;
        transState = INTERNAL_SERVER_ERROR;
    }
    return transState;
}
