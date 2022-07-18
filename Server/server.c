#include <stdio.h>
#include <stdlib.h>

#include "server.h"


ST_accountsDB_t accounts[255] = { 
                        { 1000 , 0} , { 2000 , 1 }, { 2000 , 2 }, { 2000 , 3 }, { 2000 , 4 },
                        { 2000 , 5 }, { 2000 , 6 }, { 2000 , 7 }, { 2000 , 8 }, { 2000 , 9 },
                        { 2000 , 10 }, { 2000 , 11 }, { 2000 , 12 }, { 2000 , 13 }, { 2000 , 14 },
                        { 2000 , 15 }, { 2000 , 16 }, { 2000 , 17 }, { 2000 , 18 }, { 2000 , 19 },
                        { 2000 , 20 }, { 2000 , 21 }, { 2000 , 22 }, { 2000 , 23 }, { 2000 , 24 },
                        { 2000 , 25 }, { 2000 , 26 }, { 2000 , 27 }, { 2000 , 28 }, { 2000 , 29 },
                    };

ST_transaction_t transactions[255] = {0};
uint32_t SEQUENCE_TRANSACTION_NUMBER = 0;


EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    // Check if the account exists using primary account number
    uint8_t accounts_len = accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    for(int i = 0 ; i < accounts_len ; i++)
    {
        if(cardData->primaryAccountNumber == accounts[i].primaryAccountNumber)
            return SERVER_OK;
    }
    return DECLINED_STOLEN_CARD;
}

uint8_t getAccountIndex(ST_cardData_t *cardData)
{
    uint8_t accounts_len = accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    for(uint8_t i = 0 ; i < accounts_len ; i++)
    {
        if(cardData->primaryAccountNumber == accounts[i].primaryAccountNumber)
            return i;
    }
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData , ST_cardData_t *cardData)
{
    uint8_t account_index;
    
    account_index = getAccountIndex(cardData);

    if(termData->transAmount > accounts[account_index].balance)
        return LOW_BALANCE;
    
    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    transData->transactionSequenceNumber = SEQUENCE_TRANSACTION_NUMBER;
    transactions[SEQUENCE_TRANSACTION_NUMBER] = *transData;
    SEQUENCE_TRANSACTION_NUMBER++;

    return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    uint8_t accounts_len , account_index;
    accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    
    // Check if the account exists using primary account number
    if( isValidAccount( &transData->cardHolderData) == DECLINED_STOLEN_CARD)
        return DECLINED_STOLEN_CARD;

    // check if the amount is available or not
    if(isAmountAvailable(&transData->terminalData , &transData->cardHolderData) == LOW_BALANCE)
        return DECLINED_INSUFFECIENT_FUND;
    
    // save transation data
    saveTransaction(transData);
    accounts[account_index].balance -= transData->terminalData.transAmount;

    return APPROVED;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);