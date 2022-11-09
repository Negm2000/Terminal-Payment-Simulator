//
// Created by Karim Negm on 9/11/2022.
//

#ifndef EGFWD_SERVER_H
#define EGFWD_SERVER_H

#include <stdint.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"

typedef enum EN_accountState_t
{
     RUNNING,
     BLOCKED
}EN_accountState_t;

typedef enum EN_transState_t
{
    APPROVED, DECLINED_INSUFFICIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR, FRAUD_CARD
}EN_transState_t;

typedef enum EN_serverError_t
{
    SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t ;


typedef struct ST_accountsDB_t
{
    float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;


typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transState_t transState;
    uint32_t transactionSequenceNumber;
}ST_transaction_t;

EN_transState_t receiveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData);
EN_serverError_t isBlockedAccount(uint32_t Account_Number);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData);
EN_serverError_t saveTransaction(ST_transaction_t *transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);


#endif //EGFWD_SERVER_H
