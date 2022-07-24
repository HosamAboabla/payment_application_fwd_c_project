#include <stdio.h>
#include <stdlib.h>
#include "app.h"


extern ST_accountsDB_t accounts[255];

void appStart(void)
{
    // Enter card data
    ST_cardData_t card;
    getCardHolderName(&card);
    getCardExpiryDate(&card);
    getCardPAN(&card);

    if( isValidAccount(&card) != SERVER_OK)
    {
        printf("Decliend invalid account.");
        return ;
    }
    // Terminal
    ST_terminalData_t terminal;

    setMaxAmount(&terminal);

    getTransactionDate(&terminal);
    if(isCardExpired( card , terminal) != TERMINAL_OK)
    {
        printf("Declined Expired Card.");
        return ;
    }
    
    getTransactionAmount(&terminal);
    if (isBelowMaxAmount(&terminal) != TERMINAL_OK)
    {
        printf("Declined Amount Exceeding Limit.");
        return ;
    }


    if( isAmountAvailable(&terminal , &card) != SERVER_OK)
    {
        printf("Decliend insuffecient funds.");
        return ;
    }
    ST_transaction_t transaciont = { card , terminal , 0 , 0};
    uint8_t state = recieveTransactionData(&transaciont); 
    if ( state == APPROVED)
    {
        printf("Success.");
    }
    else
    {
        printf("Error");
    }

}
