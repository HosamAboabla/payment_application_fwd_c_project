#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Card/card.h"


int main()
{

    ST_cardData_t card;
    getCardHolderName(&card);
    getCardExpiryDate(&card);
    getCardPAN(&card);

    printf("card holder name is: %s\n" , card.cardHolderName);
    printf("card expiry date is: %s\n" , card.cardExpirationDate);
    printf("card PAN is: %s\n" , card.primaryAccountNumber);

    // unsigned char test[100];
    // unsigned char test2[100];
    // printf("Please enter your name: ");
    // scanf("%s" , test);
    // strcpy(test2 , test);
    // printf("hello %s\n" , test2);

    return 0;
}