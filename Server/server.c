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
    float account_balance;
    uint8_t account_pan[20];
    FILE* accounts_file;

    accounts_file = fopen("./DB/accounts.txt" , "r");

    while( fscanf( accounts_file , "%f %s" , &account_balance , account_pan ) != EOF)
    {
        if(strcmp( cardData->primaryAccountNumber , account_pan) == 0)
            return SERVER_OK;
    }
    fclose(accounts_file);

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

    float account_balance;
    uint8_t account_pan[20];
    FILE* accounts_file;

    accounts_file = fopen("./DB/accounts.txt" , "r");

    while( fscanf( accounts_file , "%f %s" , &account_balance , account_pan ) != EOF)
    {
        if(strcmp( cardData->primaryAccountNumber , account_pan) == 0)
        {
            if(termData->transAmount > account_balance)
                return LOW_BALANCE;
        }
    }
    fclose(accounts_file);

    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{

    FILE* transactions_file;
    uint32_t seqNumber = 1;
    transactions_file = fopen("./DB/transactions.txt" , "a+");

    if(transactions_file == NULL)
        return SAVING_FAILED;

    while( fscanf(transactions_file , "%*[^\n]\n" ) != EOF)
        seqNumber++;

    transData->transactionSequenceNumber = seqNumber;
    fprintf(transactions_file , "%s %s %s %f %f %s %d %ld\n" , 
     transData->cardHolderData.cardHolderName , transData->cardHolderData.primaryAccountNumber, transData->cardHolderData.cardExpirationDate,
     transData->terminalData.maxTransAmount , transData->terminalData.transAmount , transData->terminalData.transactionDate,
     transData->transState , transData->transactionSequenceNumber);
    fclose(transactions_file);

    return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    uint8_t accounts_len , account_index;
    accounts_len = sizeof(accounts) / sizeof(accounts[0]);
    
    transData->transState = APPROVED;
    // Check if the account exists using primary account number
    if( isValidAccount( &transData->cardHolderData) == DECLINED_STOLEN_CARD)
    {
        transData->transState = DECLINED_STOLEN_CARD;
    }

    // check if the amount is available or not
    if(isAmountAvailable(&transData->terminalData , &transData->cardHolderData) == LOW_BALANCE)
    {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
    }
    
    // save transation data
    if( saveTransaction(transData) == SAVING_FAILED )
    {
        return INTERNAL_SERVER_ERROR;
    }
    
    accounts[account_index].balance -= transData->terminalData.transAmount;
    transData->transState = APPROVED;

    return transData->transState;
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