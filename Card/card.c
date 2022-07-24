#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8_t card_holder_name[25];
    printf("Please enter card holder name: ");
    _flushall();
    scanf("%[^\n]",card_holder_name);


    if(card_holder_name == NULL || strlen(card_holder_name) > 24 || strlen(card_holder_name) < 20 )
        return WRONG_NAME;
    strcpy( cardData->cardHolderName , card_holder_name);

    return OK;
}




EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    // Format "MM/YY"
    uint8_t card_expiry_data[6];
    printf("Please enter card expiry date: ");
    _flushall();

    scanf("%s" , card_expiry_data);

    if(card_expiry_data == NULL || strlen(card_expiry_data) != 5 || card_expiry_data[2] != '/')
        return WRONG_EXP_DATE;
    
    strcpy(cardData->cardExpirationDate , card_expiry_data);

    return OK;

}










EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    uint8_t accountNumber[20];

    printf("Please enter your PAN: ");
    _flushall();
    scanf("%[^\n]" , accountNumber);

    if(accountNumber == NULL || strlen(accountNumber) < 16 || strlen(accountNumber) > 19)
        return WRONG_PAN;

    strcpy(cardData->primaryAccountNumber , accountNumber);

    return OK;
}