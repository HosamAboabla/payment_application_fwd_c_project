#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    // Format DD/MM/YYYY
    uint8_t date[11];

    printf("Please enter transaction date: ");
    _flushall();
    scanf("%s" , date);

    if(date == NULL || strlen(date) != 10 || date[2] != '/' || date[5] != '/')
        return WRONG_DATE;

    strcpy(termData->transactionDate , date);

    return OK;
    
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    // Expiry date format "MM/YY"
    // transation date format "DD/MM/YYYY"
    int expiry_date_year , expiry_date_month , transation_date_year , transaction_date_month;

    expiry_date_year = (cardData.cardExpirationDate[3] - '0' ) * 10 + (cardData.cardExpirationDate[4] - '0' ) + 2000;
    expiry_date_month = ( cardData.cardExpirationDate[0] - '0') * 10 + ( cardData.cardExpirationDate[1] - '0');

    transation_date_year = (termData.transactionDate[6] - '0' ) * 1000 
                         + (termData.transactionDate[7] - '0' ) * 100
                         + (termData.transactionDate[8] - '0' ) * 10
                         + (termData.transactionDate[9] - '0' );

    transaction_date_month = (termData.transactionDate[3] - '0') * 10 + (termData.transactionDate[4] - '0');



    if( expiry_date_year < transation_date_year )
        return EXPIRED_CARD;
    else if( expiry_date_year == transation_date_year && expiry_date_month < transaction_date_month)
        return EXPIRED_CARD;

    return TERMINAL_OK;
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    float amount;
    printf("Please enter transation amount: ");
    _flushall();
    scanf("%f" , &amount);

    if(amount <= 0)
        return INVALID_AMOUNT;
    
    termData->transAmount = amount;

    return TERMINAL_OK;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    if( termData->transAmount > termData->maxTransAmount)
        return EXCEED_MAX_AMOUNT;
    
    return TERMINAL_OK;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    float max_amount;
    printf("Please enter maximum transation amount: ");
    _flushall();

    scanf("%f" , &max_amount);

    if( max_amount <= 0)
        return INVALID_MAX_AMOUNT;
    
    termData->maxTransAmount = max_amount;
    return TERMINAL_OK;
}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);