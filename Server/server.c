#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"


EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    // Check if the account exists using primary account number
    float account_balance;
    uint8_t account_pan[20];
    FILE* accounts_file;

    accounts_file = fopen("./DB/accounts.txt" , "r");

    while( fscanf( accounts_file , "%s %f" , account_pan , &account_balance ) != EOF)
    {
        if(strcmp( cardData->primaryAccountNumber , account_pan) == 0)
            return SERVER_OK;
    }
    fclose(accounts_file);

    return DECLINED_STOLEN_CARD;
}

EN_transState_t updateBalance(ST_transaction_t* transData)
{
    uint8_t current_pan[20];
    float current_balance , new_balance;
    FILE* accounts_file;
    accounts_file = fopen("./DB/accounts.txt" , "r+");
    if( accounts_file == NULL)
        return INTERNAL_SERVER_ERROR;

    while( fscanf(accounts_file , "%s " , current_pan) != EOF)
    {
        
        if(strcmp(transData->cardHolderData.primaryAccountNumber , current_pan) == 0)
        {

            fscanf(accounts_file , "%f" , &current_balance);

            new_balance = current_balance - transData->terminalData.transAmount;
            fseek(accounts_file , -12 , SEEK_CUR);
            fprintf(accounts_file , "%f" , new_balance);
            break;
        }
        fscanf(accounts_file, "%*[^\n]\n");
    }
    fclose(accounts_file);

    return APPROVED;
    

}   

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData , ST_cardData_t *cardData)
{

    float account_balance;
    uint8_t account_pan[20];
    FILE* accounts_file;

    accounts_file = fopen("./DB/accounts.txt" , "r");

    while( fscanf( accounts_file , "%s %f" , account_pan , &account_balance ) != EOF)
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
    fprintf(transactions_file , "%s, %s %s %f %s %d %ld\n" , 
     transData->cardHolderData.cardHolderName , transData->cardHolderData.primaryAccountNumber, transData->cardHolderData.cardExpirationDate,
     transData->terminalData.transAmount , transData->terminalData.transactionDate,
     transData->transState , transData->transactionSequenceNumber);
    fclose(transactions_file);

    return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    uint8_t accounts_len , account_index;
    
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
    
    return updateBalance(transData);
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    FILE* file;
    uint8_t found = 0;
    file = fopen("./DB/transactions.txt" , "r");
    int state , temp;
    while( fscanf(file , "%[^,], %s %s %f %s %d %ld\n" , 
     &transData->cardHolderData.cardHolderName , &transData->cardHolderData.primaryAccountNumber, &transData->cardHolderData.cardExpirationDate,
     &transData->terminalData.transAmount , &transData->terminalData.transactionDate,
     &transData->transState , &transData->transactionSequenceNumber) != EOF)
    {
        if( transData->transactionSequenceNumber == transactionSequenceNumber)
        {
            found = 1;
            break;
        }
    }

    fclose(file);
    if(found)
        return SERVER_OK;
    
    transData = (ST_transaction_t*)  NULL;

    return TRANSACTION_NOT_FOUND;   
}