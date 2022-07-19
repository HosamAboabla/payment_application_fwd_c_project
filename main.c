#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Application/app.h"
int main()
{
    ST_cardData_t card;
    // getCardExpiryDate(&card);

    ST_terminalData_t terminal;
    getCardPAN(&card);
    // setMaxAmount(&terminal);
    getTransactionAmount(&terminal);
    // isValidAccount(&card);
    if( isAmountAvailable(&terminal , &card)   == SERVER_OK)
    {
        printf("Valid amount.");
    }
    else{
        printf("Invalid amount.");
    }













    // appStart();

    // getCardHolderName(&card);
    // printf("card holder name is: %s\n" , card.cardHolderName);
    // printf("card expiry date is: %s\n" , card.cardExpirationDate);
    // printf("card PAN is: %s\n" , card.primaryAccountNumber);
    // printf("Transation date: %s\n" , terminal.transactionDate);
    // if(isCardExpired( card , terminal)== TERMINAL_OK)
    //     printf("Card is not expired\n");
    // else
    //     printf("Card is expired\n");

    // printf("%d\n" ,getTransactionAmount(&terminal));
    // printf("Transation amount = %f\n" , terminal.transAmount);
    
    return 0;
}