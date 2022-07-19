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

    if( isValidAccount(&card) != SERVER_OK)
    {
        printf("Decliend invalid account.");
        return ;
    }

    if( isAmountAvailable(&terminal , &card) != SERVER_OK)
    {
        printf("Decliend insuffecient funds.");
        return ;
    }
    

    ST_transaction_t transaction = { card , terminal };

    recieveTransactionData(&transaction);
    uint8_t account_index = getAccountIndex(&card);

    printf("=================== Transaction details ===================\n");
    printf("card holder name: %s\n" , transaction.cardHolderData.cardHolderName );
    printf("card pan: %s\n" , transaction.cardHolderData.primaryAccountNumber );
    printf("Expiry Date: %s\n" , transaction.cardHolderData.cardExpirationDate );
    printf("Transation Date: %s\n" , transaction.terminalData.transactionDate );
    printf("Terminal max amount: %f\n" , transaction.terminalData.maxTransAmount );
    printf("Terminal amount: %f\n" , transaction.terminalData.transAmount );
    printf("Your new balance = %f\n" , accounts[account_index].balance );
    printf("Transation state %d\n" , transaction.transState );

}
