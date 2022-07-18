#include <stdio.h>
#include <stdlib.h>
#include "app.h"



void appStart(void)
{
    // Enter card data
    ST_cardData_t card;
    getCardHolderName(&card);
    getCardExpiryDate(&card);
    getCardPAN(&card);
    // printf("card holder name is: %s\n" , card.cardHolderName);
    // printf("card expiry date is: %s\n" , card.cardExpirationDate);
    // printf("card PAN is: %s\n" , card.primaryAccountNumber);


    // Terminal
    ST_terminalData_t terminal;
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

    // printf("%d\n" ,getTransactionAmount(&terminal));
    // printf("Transation amount = %f\n" , terminal.transAmount);
}
