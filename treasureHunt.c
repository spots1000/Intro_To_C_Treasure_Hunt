/*
*   Anthony Dalia
*   COP3223C
*   Section 0003
*   Assignment #8 - Treasure Hunt
*   Dec 3rd - 2017
*/

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Structs

struct pirate
{

    int dig;
    int carry;
};

struct map
{

    int sand;
    int treasure;
};

//Function Signatures
void arraysInit (struct map tiles[][3], struct pirate crew[4]);
void crewStatus (struct pirate crew[]);
void mapStatus (struct map tiles[][3]);
void useCrew (struct map tiles[][3], struct pirate crew[], int pirateNum, int *tGathered);
void endCheck (struct map tiles[][3], int *hour, int tGathered, int *loopCtrl, int *tLeft);
void treasureNum ( struct map tiles[][3], int *tLeft);

//Contains runtime code that refrences the below functions.
void main(void)
{

    //Structs
    struct map tiles[3][3];
    struct pirate crew[4];

    //Ints
    int hour = 0;
    int tGathered = 0;
    int tLeft;
    int pirateNum = -1;
    int loop = 1;
    int remaining = 0;
    int i,j;

    //Setup Code
    arraysInit(tiles, crew);

    //Loop until the game is done
    while (loop == 1)
    {
        printf("\n");

        crewStatus(crew);

        printf("\n");

        for (i = 0; i < 4; i++)
        {
            useCrew(tiles,crew,i,&tGathered);
            printf("\n");
        }

        printf("\n");
        endCheck(tiles, &hour, tGathered, &loop, &tLeft);
    }
}

//Pre-Conditions : takes in a 3x3 grid of dig spots and an array of 4 pirates
//Post-Conditions : sets up thoes arrays based on an input file provided by user
void arraysInit(struct map tiles[][3], struct pirate crew[4])
{
    //File Stuff
    FILE *data;
    char fileName[40];

    //Ask about the file
    printf("Please enter the name of the file that contains the map data: \n");
    scanf("%s", fileName);

    //Open the file
    data = fopen(fileName, "r");

    int i, j;

    //Loop to fill the map with values
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            fscanf(data, "%d", &tiles[i][j].sand);
            fscanf(data, "%d", &tiles[i][j].treasure);
        }
    }

    //Loop to fill the pirates with values
    for (i = 0; i < 4; i++)
    {

        fscanf(data, "%d", &crew[i].dig);
        fscanf(data, "%d", &crew[i].carry);
    }

    fclose(data);
}

//Pre-Conditions : takes in the 3x3 grid of dig spots
//Post-Condition : prints out the dig spot status in desired format
void mapStatus(struct map tiles[][3])
{

    //Temporary code
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {

            if (tiles[i][j].sand > 0)
            {
                printf("%ds", tiles[i][j].sand);
            }
            else if (tiles[i][j].treasure > 0)
            {
                printf("%dT", tiles[i][j].treasure);
            }
            else
            {
                printf("-");
            }

            printf("    ");
        }

        printf("\n");
    }
}

//Pre-Conditions : Takes in an array of 4 pirates
//Post-Condition : Prints out the stats of each pirate in the desired format
void crewStatus (struct pirate crew[])
{

    printf("Crew    Dig    Carry\n");

    int i;
    for (i = 0; i < 4; i++)
    {

        printf("%d        %d       %d\n", (i+1), crew[i].dig, crew[i].carry);
    }
}

//Pre-Conditions  : Takes in an array of 3x3 dig spots and a reference to the remaining treasure var
//Post-Conditions : Corrects remaining treasure var to the latest num of remaining treasure
void treasureNum ( struct map tiles[][3], int *remaining)
{

    int i,j, leftTreasure = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {

            leftTreasure += tiles[i][j].treasure;
        }
    }

    *remaining = leftTreasure;
}

//Pre-Conditions : Takes in an array of 3x3 dig spots, an array of 4 pirates, the number of the target pirate, and a refrence to the number of treasure gathered
//Post-Conditions : Updates the map to reflect the action of the desired pirate.
void useCrew (struct map tiles[][3], struct pirate crew[], int pirateNum, int *tGathered)
{

    printf("Where would you like to send crew member %d?\n", pirateNum+1);

    //Vars
    int x,y;
    char s;

    //Print Map
    mapStatus(tiles);
    printf("\n");

    //Scan cords
    scanf("%d%c%d", &x,&s,&y);

    //Fix Cords
    x -= 1;
    y -= 1;

    //Effect change
    if (tiles[x][y].sand > 0)
    {

        tiles[x][y].sand -= crew[pirateNum].dig;

        if (tiles[x][y].sand <= 0)
        {

            printf("You have removed all the sand from this section");
            tiles[x][y].sand = 0;
        }
        else if (tiles[x][y].sand > 0)
        {

            printf("You have removed some of the sand from this section.");
        }
    }
    else if (tiles[x][y].treasure > 0)
    {

        tiles[x][y].treasure -= crew[pirateNum].carry;
        *tGathered += crew[pirateNum].carry;

        if (tiles[x][y].treasure > 0)
        {

            printf("You take some the treasure back to the ship.");
        }
        else if (tiles[x][y].treasure <= 0)
        {

            printf("You take all of the treasure back to the ship!");
            tiles[x][y].treasure = 0;
        }
    }
    else
    {
        printf("There is nothing left to do in this section.");
    }
}

//Pre-Conditons : Takes in an array 3x3 dig spots, refrence to the hour, the number of treasure gathered, a refrence to the loop control variable, and a refrence to the number of treasure left
//Post-Conditions : Ends the game or updates the hour to move the game forwards.
void endCheck (struct map tiles[][3], int *hour, int tGathered, int *loopCtrl, int *tLeft)
{

    //Update remaining treasure
    treasureNum(tiles, tLeft);

    //See if we have gathered all the treasure
    if (tLeft == 0)
    {

        *loopCtrl == -1;
        printf("All of the pirate's treasure belongs to you now!");
    }
    else if (*hour == 7)
    {

        *loopCtrl == -1;
        printf("You are forced to evacuate the island. You have claimed %d pieces of the pirate's treasure.", tGathered);
    }
    else
    {
        *hour += 1;

        printf("\nYou have %d hours left to dig up the treasure.\n", (8-*hour));
    }

}
