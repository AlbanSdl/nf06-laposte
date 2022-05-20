#include "main.h"

int test() {
    printf("Bonjour le monde!\n");
    return 0;
}

void EnterCar()
{
    int nbcar;
    printf("enter the number of car");
    scanf("%d", nbcar);
    car tabcar[nbcar];
    for (int i=0, i<nbcar, i++)
    {
        printf("enter the capacity of vehicule", i+1);
        scanf ("%d", tabcar[i].capacity);
        printf("enter the battery of vehicule", i+1);
        scanf ("%d", tabcar[i].battery);
        printf("enter the recharging time of vehicule", i+1);
        scanf ("%f", tabcar[i].rechargetps);
    }
}

void readfile()
{
    fopen()
}
int main()
{
    printf("Hello World");

    return 0;
}
