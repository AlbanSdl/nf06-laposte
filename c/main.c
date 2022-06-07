#include "main.h"

void enter_car()
{
    int nbcar;
    printf("Enter the number of car: ");
    scanf("%d", &nbcar);
    Car tabcar[nbcar];
    for (int i=0; i < nbcar; i++)
    {
        printf("Enter the capacity of vehicule %d: ", i+1);
        scanf ("%d", &tabcar[i].capacity);
        printf("Enter the battery of vehicule %d: ", i+1);
        scanf ("%d", &tabcar[i].battery);
        printf("Enter the recharging time of vehicule %d: ", i+1);
        scanf ("%f", &tabcar[i].rechargetps);
    }
}
