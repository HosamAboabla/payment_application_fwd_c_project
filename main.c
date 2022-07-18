#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Card/card.h"
#include "Terminal/terminal.h"

int main()
{

    // ST_cardData_t card;
    // getCardHolderName(&card);
    // getCardExpiryDate(&card);
    // getCardPAN(&card);
    // printf("card holder name is: %s\n" , card.cardHolderName);
    // printf("card expiry date is: %s\n" , card.cardExpirationDate);
    // printf("card PAN is: %s\n" , card.primaryAccountNumber);

    ST_terminalData_t terminal;

    // getTransactionDate(&terminal);
    // printf("Transation date: %s\n" , terminal.transactionDate);
    // if(isCardExpired( card , terminal)== TERMINAL_OK)
    //     printf("Card is not expired\n");
    // else
    //     printf("Card is expired\n");

    // printf("%d\n" ,getTransactionAmount(&terminal));
    // printf("Transation amount = %f\n" , terminal.transAmount);
    
    return 0;
}