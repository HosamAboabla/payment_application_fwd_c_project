#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

ST_accountsDB_t accounts[255] = { 
                        { 1000 , "123456789123456780" } , { 2000 , "123456789123456781" }, { 2000 , "123456789123456782" }, 
                        { 2000 , "123456789123456783" }, { 2000 , "123456789123456784" }, { 2000 , "123456789123456785" }, 
                        { 2000 , "123456789123456786" }, { 2000 , "123456789123456787" }, { 2000 , "123456789123456788" }, 
                        { 2000 , "123456789123456789" },
                    };

ST_transaction_t transactions[255];
uint32_t SEQUENCE_TRANSACTION_NUMBER = 0;


EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    // Check if the account exists using primary account number
    uint8_t accounts_len = accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    for(int i = 0 ; i < accounts_len ; i++)
    {
        if(strcmp(cardData->primaryAccountNumber , accounts[i].primaryAccountNumber) == 0)
            return SERVER_OK;
    }
    return DECLINED_STOLEN_CARD;
}

uint8_t getAccountIndex(ST_cardData_t *cardData)
{
    uint8_t accounts_len = accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    for(uint8_t i = 0 ; i < accounts_len ; i++)
    {
        if(strcmp(cardData->primaryAccountNumber , accounts[i].primaryAccountNumber) == 0)
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
    if(SEQUENCE_TRANSACTION_NUMBER >= 255 )
        return SAVING_FAILED;

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
    {
        transData->transState = DECLINED_STOLEN_CARD;
        return DECLINED_STOLEN_CARD;
    }

    // check if the amount is available or not
    if(isAmountAvailable(&transData->terminalData , &transData->cardHolderData) == LOW_BALANCE)
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        return DECLINED_INSUFFECIENT_FUND;
    }
    
    account_index = getAccountIndex(&transData->cardHolderData);

    // save transation data
    if( saveTransaction(transData) == SAVING_FAILED )
    {
        transData->transState = INTERNAL_SERVER_ERROR;
        return SAVING_FAILED;
    }
    
    accounts[account_index].balance -= transData->terminalData.transAmount;
    transData->transState = APPROVED;

    return APPROVED;
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    // if(transactionSequenceNumber >= 255)
    //     return TRANSACTION_NOT_FOUND;
    // if( transactions[transactionSequenceNumber] = *transData)
    // {
    //     *transData = transactions[transactionSequenceNumber];
    // }
    
}