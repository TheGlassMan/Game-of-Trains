// NOTE: Current version only works for 2 players :(
// here is my github for documentation : https://github.com/TheGlassMan/Game-of-Trains
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
typedef struct {
    int value;
    char action[15];
    char place[15]; //i.e in hand, center of table, or in deck or in used pile
} card;
typedef struct {
    card hand[7];
} Player;
int playerTurn = 1, cardTable = 0;
int numPlayers = 0; //number of players 2-4
bool cardPicked = false; //indicates if card was pciked from pile so now we have 3 cards at center table
bool removeMiddle = false, removeRight = false, removeLeft = false;
void assignCards(card cards[], FILE* inp);
void shuffleCards(card cards[], int numCards);
void startDeck(card cards[], Player* player, int numCards);
void showDeck(Player* player); //Show player their deck
void round1Move(Player* player, int playerNum, card cards[], card centerCards[]);
void gamePlay(Player* player, int playerNum, card cards[], card centerCards[]);
bool winner(Player* player);
void cardAbility(Player* player, int playerNum, card cards[], card centerCards[]);
int main() {
    srand(time(NULL));
    card cards[84];
    Player players[4];
    card centerCards[6];
    bool winnerCheck = false;
    bool validInput = false;
    char answer;
    char filename[20];
    FILE *cardfile = NULL;
    printf("Would you like to use a custom deck? (y/n)\n");
    scanf(" %c", &answer);
    if (answer == 'y'){
        printf("Please enter the name of the file?\n");
        scanf("%s", filename);
        cardfile = fopen(filename, "r");
    }
    else{
        cardfile = fopen("sampledeck.txt", "r");
    }
    if (cardfile == NULL) {
        printf("File DNE");
    }
    else {
        assignCards(cards, cardfile);
        for (int i = 0; i < 84; i++) {
            strcpy(cards[i].place, "Deck");
        }
        fclose(cardfile);
        printf("How many players are playing? (2-4)\n");
        while (!validInput){
            scanf("%d", &numPlayers);
            if (numPlayers > 1 && numPlayers <= 4){
                validInput = true;
                break;
            }
            else{
                printf("Invalid number of players. Please enter a number between 2 and 4: \n");
            }
        }
        shuffleCards(cards, 84);
        for (int i = 0; i < numPlayers; i++) {
            startDeck(cards, &players[i], 7);
        }
        for (int i = 0; i < numPlayers; i++) {
            printf("Player %d Deck: \n", i + 1);
            showDeck(&players[i]);

        }
        for (int i = 0; i < numPlayers; i++) {
            round1Move(&players[i], i + 1, cards, centerCards);
        }
        int i = 0;
        playerTurn = 1;
        while (!winner(&players[i])) {
            gamePlay(&players[i], i + 1, cards, centerCards);
            winner(&players[i]);
            if (i == numPlayers - 1) {
                i = 0;
                playerTurn = 1;
            }
            else{
                i++;
                playerTurn++;
            }
        }
        printf("Game over!\n");
        printf("The winner is Player %d", i+1);
    }
    return 0;
}
void assignCards(card cards[], FILE* inp){
    for (int i = 0; i < 84; i++){
        fscanf(inp, "%d %s", &cards[i].value, cards[i].action);
    }
}

void startDeck(card cards[], Player* player, int numCards) {
    int rando;
    for (int i = 0; i < numCards; i++) {
        rando = rand() % 84;
        if (strcmp(cards[rando].place, "Hand") != 0){
            strcpy(cards[rando].place, "Hand");
            player->hand[i] = cards[rando];
        }
        else{
            i--;
        }
        }
    }

void shuffleCards(card cards[], int numCards) {
    for (int i = numCards - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

void showDeck(Player* player){
    for (int i = 0; i < 7; i++){
        printf("|%-12s| ", player->hand[i].action);
    }
    printf("\n");
    for (int i = 0; i < 7; i++) {
        printf("|     %d     | ", player->hand[i].value);
    }
    printf("\n");
    for (int i = 0; i < 7; i++) {
        printf("|  Option:[%d]| ", i+1);
    }
    printf("\n");
    printf("\n");
}

void round1Move(Player* player, int playerNum, card cards[], card centerCards[]) {
    int i;
    int k;
    int centerPlace = 1;
    int cardHand, cardDeck;//card hand is the one in hand and card deck is the ones from the deck you grab
    int randNum[10]; // used to grab from deck
    int cardKeep = 0; //card the person decided to keep and swap and the other gets discarded
    card temp;
    bool validInput = false;
    printf("Player %d turn:\n", playerTurn);
    printf("Cards on table: \n");
    for (int j = 0; j < 10; j++) {
        i = rand() % 84;
        if (strcmp(cards[i].place, "Hand") == 0 ||
            strcmp(cards[i].place, "Discard") == 0 ||
            strcmp(cards[i].place, "Center") == 0) {
            j--;
        }
        else{
            randNum[j] = i;
        }
    }
    for (int j = 0; j <= 9; j++) {
        if (strcmp(centerCards[j].place, "Center") == 0){
            cards[randNum[j]] = centerCards[j];
        }
    }
    //prints out random card to choose
    for (k = 0; k < playerNum; k++) {
        printf("|%-12s| ", cards[randNum[k]].action);
        if (k >= 1){
            printf("|%-12s| ", cards[randNum[k+1]].action);
        }
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|     %d     | ", cards[randNum[k]].value);
        if (k >= 1) {
            printf("|     %d     | ", cards[randNum[k + 1]].value);
        }
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|  Option:[%d]| ", centerPlace);
        centerPlace++;
        if (k >= 1){
            printf("|  Option:[%d]|", centerPlace);
            k++;
        }

    }
    printf("\n\n");
    if (k == 1) {
        while (!validInput) {
            printf("Which card would you like to swap from your deck with Card[%d]? \n", k);
            //prints out players card
            for (int i = 0; i < 7; i++) {
                printf("|%-12s| ", player->hand[i].action);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|     %d     | ", player->hand[i].value);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|  Option:[%d]| ", i + 1);
            }
            printf("\n");
            printf("\n");
            scanf("%d", &cardHand);
            cardDeck = 0;
            cardHand--;

            if (cardHand >= 0 && cardHand < 7) {
                validInput = true;
                centerCards[cardKeep] = player->hand[cardHand];
                strcpy(cards[randNum[cardDeck]].place, "Hand");
                player->hand[cardHand] = cards[randNum[cardDeck]];
                strcpy(centerCards[cardKeep].place, "Center");
            } else {
                printf("Invalid option. Please select a number between 1 and 7.\n");
            }
            printf("New Deck Player %d: \n", playerTurn);
            for (int i = 0; i < 7; i++) {
                printf("|%-12s| ", player->hand[i].action);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|     %d     | ", player->hand[i].value);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|  Option:[%d]| ", i + 1);
            }
            printf("\n");
            printf("\n");
        }
    }
    //since any player after player 1 grabs two cards and the one on the table
    if (k >= 2) {
        while (!validInput) {
            printf("Which card on the table would you like to keep and leave on the table? Can not be 1-%d\n",
                   playerTurn - 1);

            scanf("%d", &cardKeep);
            cardKeep--;
            if (cardKeep == 0) {
                printf("Invalid Input\n");
                scanf("%d", &cardKeep);
            }
            strcpy(cards[randNum[playerTurn]].place, "Discard");
            cards[randNum[playerTurn - 1]] = cards[randNum[cardKeep]];
            strcpy(cards[randNum[playerTurn - 1]].place, "Center");
            centerCards[playerTurn - 1] = cards[randNum[cardKeep]];
            printf("Cards on table: \n");
            centerPlace = 1;
            for (k = 0; k < playerNum; k++) {
                printf("|%-12s| ", cards[randNum[k]].action);
            }
            printf("\n");
            for (k = 0; k < playerNum; k++) {
                printf("|     %d     | ", cards[randNum[k]].value);
            }
            printf("\n");
            for (k = 0; k < playerNum; k++) {
                printf("|  Option:[%d]| ", centerPlace);
                centerPlace++;
            }
            printf("\n");
            printf("Which card would you like to swap with Option [%d] from your hand? \n", playerTurn);
            for (int i = 0; i < 7; i++) {
                printf("|%-12s| ", player->hand[i].action);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|     %d     | ", player->hand[i].value);
            }
            printf("\n");
            for (int i = 0; i < 7; i++) {
                printf("|  Option:[%d]| ", i + 1);
            }
            printf("\n");
            scanf("%d", &cardHand);
            cardHand--;
            if ((cardHand >= 0 && cardHand < 7) && (cardDeck >= 0 && cardDeck < 4)) {
                validInput = true;
                centerCards[playerTurn - 1] = player->hand[cardHand];
                strcpy(cards[randNum[cardDeck]].place, "Hand");
                player->hand[cardHand] = cards[randNum[playerTurn - 1]];
                strcpy(centerCards[playerTurn - 1].place, "Center");
            } else {
                printf("Invalid option. Please select a number between 1 and 7.\n");
            }
        }
        printf("New Deck Player %d\n", playerTurn);
        for (int i = 0; i < 7; i++) {
            printf("|%-12s| ", player->hand[i].action);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|     %d     | ", player->hand[i].value);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|  Option:[%d]| ", i + 1);
        }
        printf("\n");
        printf("\n");
//        printf("DEBUG CARDS ON TABLE VALUES:\n");
//        for (int i = 0; i < 4; i++){
//            printf("|%-12s| ", centerCards[i].action);
//        }
//        printf("\n");
//        for (int i = 0; i < 7; i++) {
//            printf("|     %d     | ", centerCards[i].value);
//        }
//        printf("\n");
//        for (int i = 0; i < 7; i++) {
//            printf("|  Option:[%d]| ", i+1);
//        }
    }
    playerTurn++;
}

void gamePlay(Player* player, int playerNum, card cards[], card centerCards[]){
    int turnChoice; // will be used to indicate if the player chose to grab from deck or ability
    bool validInput1 = false;
    bool validInput2 = false;
    bool validInput3 = false;
    bool validInput4 = false;
    int cardChoice, moveCard, moveCard2;
    int randNum[1];
    char *protectedLeft = strstr(player->hand[0].place, "(P)");
    char *protectedRight = strstr(player->hand[6].place, "(P)");
    char *protectedMiddle = strstr(player->hand[3].place, "(P)");
    card temp;
    //card picker :)
    if (removeLeft && protectedLeft == NULL){
        for (int j = 0; j < 1; j++) {
            int rando = rand() % 84;
            if (strcmp(cards[rando].place, "Hand") == 0 ||
                strcmp(cards[rando].place, "Discard") == 0 ||
                strcmp(cards[rando].place, "Center") == 0) {
                j--;
            } else {
                temp = player->hand[0];
                strcpy(cards[rando].place, "Hand");
                player->hand[0] = cards[rando];
                centerCards[cardTable+2] = temp;
                cardTable++;
            }
        }
        removeLeft = false;
    }
    else if (removeMiddle && protectedMiddle == NULL){
        for (int j = 0; j < 1; j++) {
            int rando = rand() % 84;
            if (strcmp(cards[rando].place, "Hand") == 0 ||
                strcmp(cards[rando].place, "Discard") == 0 ||
                strcmp(cards[rando].place, "Center") == 0) {
                j--;
            } else {
                temp = player->hand[3];
                strcpy(cards[rando].place, "Hand");
                player->hand[3] = cards[rando];
                centerCards[cardTable+2] = temp;
                cardTable++;
            }
        }
        removeMiddle = false;
    }
    else if (removeRight && protectedRight == NULL){
        for (int j = 0; j < 1; j++) {
            int rando = rand() % 84;
            if (strcmp(cards[rando].place, "Hand") == 0 ||
                strcmp(cards[rando].place, "Discard") == 0 ||
                strcmp(cards[rando].place, "Center") == 0) {
                j--;
            } else {
                temp = player->hand[6];
                strcpy(cards[rando].place, "Hand");
                player->hand[6] = cards[rando];
                centerCards[cardTable+2] = temp;
                cardTable++;
            }
        }
        removeRight = false;
    }
        for (int j = 0; j < 1; j++) {
            int rando = rand() % 84;
            if (strcmp(cards[rando].place, "Hand") == 0 ||
                strcmp(cards[rando].place, "Discard") == 0 ||
                strcmp(cards[rando].place, "Center") == 0) {
                j--;
            } else {
                randNum[j] = rando;
            }
        }
        printf("Player %d turn\n", playerTurn);
        printf("Cards on table:\n");
        //Prints cards on table
        if (cardPicked) {
            for (int i = 0; i < numPlayers + cardTable; i++) {
                if (strcmp(cards[i].place, "Discard") == 0) {
                    centerCards[i] = centerCards[i+1];
                }
                else{
                    printf("|%-12s| ", centerCards[i].action);
                }
            }
            printf("\n");
            for (int i = 0; i < numPlayers + cardTable; i++) {
                if (strcmp(cards[i].place, "Discard") != 0) {
                    printf("|     %d     | ", centerCards[i].value);
                }
            }
            printf("\n");
            for (int i = 0; i < numPlayers + cardTable; i++) {
                if (strcmp(cards[i].place, "Discard") != 0) {
                    printf("|  Option:[%d]| ", i + 1);
                }
            }
            printf("\n");
            printf("\n");
        } else {
            for (int i = 0; i < numPlayers; i++) {
                printf("|%-12s| ", centerCards[i].action);
            }
            printf("\n");
            for (int i = 0; i < numPlayers; i++) {
                printf("|     %d     | ", centerCards[i].value);
            }
            printf("\n");
            for (int i = 0; i < numPlayers; i++) {
                printf("|  Option:[%d]| ", i + 1);
            }
            printf("\n");
            printf("\n");
        }
        printf("Player %d cards:\n", playerTurn);
        //prints players cards
        for (int i = 0; i < 7; i++) {
            printf("|%-12s| ", player->hand[i].action);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|     %d     | ", player->hand[i].value);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|  Option:[%d]| ", i + 1);
        }
        printf("\n");
        printf("What would you like to grab card from pile or use ability of a card on the table?\nEnter number for option\n1: Grab card from deck\n2: Use ability\n");
        while (!validInput1) {
            scanf("%d", &turnChoice);
            if (turnChoice == 1 || turnChoice == 2) {
                validInput1 = true;
                break;
            } else {
                printf("Invalid input");
            }
        }
    if (turnChoice == 1){

        printf("Card you picked up:\n");
        printf("|%-12s| ", cards[randNum[0]].action);
        printf("\n");
        printf("|     %d     | ", cards[randNum[0]].value);
        printf("\n");
        printf("|  Option:[1]| ");
        printf("\n");
        printf("Which card in your hand would you like to swap with the card you picked up?\n");
        scanf("%d", &cardChoice);
        cardChoice--;
        strcpy(cards[randNum[0]].place, "Hand");
        cards[randNum[0]] = player->hand[cardChoice];
        centerCards[cardTable+2] = cards[randNum[0]];
        strcpy(centerCards[2].place, "Center");
        cardPicked = true;
        cardTable++;
        printf("Player %d cards:\n", playerTurn);
        //prints cards
        for (int i = 0; i < 7; i++) {
            printf("|%-12s| ", player->hand[i].action);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|     %d     | ", player->hand[i].value);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|  Option:[%d]| ", i + 1);
        }
    }
    else{
        printf("Which card ability would you like to use from the table cards?\n");
        while (!validInput2){
            scanf("%d", &cardChoice);
            cardChoice--;
            if (cardChoice >= 0 && cardChoice < 2){
                validInput2 = true;
                break;
            }
        }

        // down below will be card ability based on centercard.action
        while (!validInput3){
            if (strcmp(centerCards[cardChoice].action, "shift2Right") == 0){
                if (strcmp(player->hand[cardChoice].place, "Protected") == 0){
                    printf("Card is protected choose another card");
                }
                else{
                    printf("Which card would you like to move two to the right?(Cannot be 6 or 7)\n");
                    while (!validInput4){
                        scanf("%d", &moveCard);
                        moveCard--;
                        if (moveCard < 5){
                            validInput4 = true;
                            break;
                        }
                        else{
                            printf("Invalid choose another card\n");
                        }
                    }
                    temp = player->hand[moveCard];
                    for (int i = 0; i < 6; i++) {
                        player->hand[i] = player->hand[i + 1];
                    }
                    player->hand[moveCard+2] = temp;

                    strcpy(centerCards[cardChoice].place, "Discard");
                    validInput3 = true;
                }
            }
            else if (strcmp(centerCards[cardChoice].action, "protect") == 0){
                if (strcmp(player->hand[cardChoice].place, "Protected") == 0){
                    printf("Card is already protected choose another card");
                }
                else {
                    printf("Which card would you like to protect?\n");
                    scanf("%d", &cardChoice);
                    cardChoice--;
                    strcpy(player->hand[cardChoice].place, "Protected");
                    strcat(player->hand[cardChoice].action, "(P)");
                    strcpy(centerCards[cardChoice].place, "Discard");
                    validInput3 = true;

                }
            }
            else if (strcmp(centerCards[cardChoice].action, "shift2Left") == 0){
                if (strcmp(player->hand[cardChoice].place, "Protected") == 0){
                    printf("Card is protected choose another card\n");
                }
                else{
                    printf("Which card would you like to move two to the right?(Cannot be 0 or 1)\n");
                    while (!validInput4){
                        scanf("%d", &moveCard);
                        moveCard--;
                        if (moveCard > 1){
                            validInput4 = true;
                            break;
                        }
                        else{
                            printf("Invalid choose another card\n");
                        }
                    }
                    temp = player->hand[moveCard];
                    for (int i = 4; i > 2; i--) {
                        player->hand[i] = player->hand[i - 1];
                    }
                    player->hand[moveCard - 2] = temp;
                }
            }
            else if (strcmp(centerCards[cardChoice].action, "swapAdjacent") == 0){
                if (strcmp(player->hand[cardChoice].place, "Protected") == 0){
                    printf("Card is protected choose another card\n");
                }
                else{
                    printf("Which card from your hand would you like to swap with its neighbor (left or right)\n");
                    scanf("%d", &moveCard);
                    moveCard--;
                    printf("Would you like to swap with Option %d or Option %d?", moveCard - 1, moveCard + 1);
                    while (!validInput4){
                        scanf("%d", &moveCard2);
                        moveCard2--;
                        if (moveCard2 == moveCard - 1 || moveCard2 == moveCard + 1){
                            validInput4 = true;
                            break;
                        }
                        else{
                            printf("Please select either Option %d or Option %d \n", moveCard - 1, moveCard + 1);
                        }
                    }
                    //swaps cards
                    temp = cards[moveCard];
                    cards[moveCard] = cards[moveCard2];
                    cards[moveCard2] = temp;
                    //makes centerCard unusable
                    strcpy(centerCards[cardChoice].place, "Discard");
                    validInput3 = true;
                }
            }
            else if (strcmp(centerCards[cardChoice].action, "removeMiddle") == 0){
                    printf("Swapping middle card in your and your opponents hands with new card (unless opponents card is not protected)\n");
                    for (int j = 0; j < 1; j++) {
                        int rando = rand() % 84;
                        if (strcmp(cards[rando].place, "Hand") == 0 ||
                            strcmp(cards[rando].place, "Discard") == 0 ||
                            strcmp(cards[rando].place, "Center") == 0) {
                            j--;
                        }
                        else {
                            temp = player->hand[3];
                            strcpy(cards[rando].place, "Hand");
                            player->hand[3] = cards[rando];
                            centerCards[cardTable+2] = temp;
                        }
                    }
                if (cardTable == 0){
                    cardTable++;
                }
                    strcpy(centerCards[cardChoice].place, "Discard");
                    removeMiddle = true;
                    validInput3 = true;
                    cardPicked = true;
            }
            else if (strcmp(centerCards[cardChoice].action, "removeRight") == 0){
                printf("Swapping last card in your and your opponents hands with new card (unless opponents card is not protected)\n");
                    for (int j = 0; j < 1; j++) {
                        int rando = rand() % 84;
                        if (strcmp(cards[rando].place, "Hand") == 0 ||
                            strcmp(cards[rando].place, "Discard") == 0 ||
                            strcmp(cards[rando].place, "Center") == 0) {
                            j--;
                        } else {
                            temp = player->hand[6];
                            strcpy(cards[rando].place, "Hand");
                            player->hand[6] = cards[rando];
                            centerCards[cardTable+2] = temp;
                        }
                    }
                        if (cardTable == 0){
                            cardTable++;
                        }
                    strcpy(centerCards[cardChoice].place, "Discard");
                    removeRight = true;
                    validInput3 = true;
                    cardPicked = true;
            }
            else if (strcmp(centerCards[cardChoice].action, "removeLeft") == 0){

                    printf("Swapping first card in your and your opponents hands with new card (unless opponents card is not protected)\n");
                    for (int j = 0; j < 1; j++) {
                        int rando = rand() % 84;
                        if (strcmp(cards[rando].place, "Hand") == 0 ||
                            strcmp(cards[rando].place, "Discard") == 0 ||
                            strcmp(cards[rando].place, "Center") == 0) {
                            j--;
                        } else {
                            temp = player->hand[0];
                            strcpy(cards[rando].place, "Hand");
                            player->hand[0] = cards[rando];
                            centerCards[cardTable+2] = temp;
                        }
                    }
                         if (cardTable == 0){
                            cardTable++;
                        }
                    strcpy(centerCards[cardChoice].place, "Discard");
                    removeLeft = true;
                    validInput3 = true;
                    cardPicked = true;
            }
            else if (strcmp(centerCards[cardChoice].action, "swapSkip1Card") == 0){
                if (strcmp(player->hand[cardChoice].place, "Protected") == 0){
                    printf("Card is protected choose another card");
                }
                else{
                    printf("Which card from your hand would you like to swap with its neighbor one over (left or right if possible)\n");
                    while (!validInput4){
                        scanf("%d", &moveCard);
                        moveCard--;
                        if (moveCard >= 0 && moveCard <=6){
                            validInput4 = true;
                            break;
                        }
                        else{
                            printf("Please select a card 1 - 7");
                        }
                    }
                    validInput4 = false;

                    if (moveCard - 2 <= 1 && moveCard + 2 <= 6){
                        printf("Would you like to swap with Option %d or Option %d?", moveCard - 2, moveCard + 2);
                        while (!validInput4){
                            scanf("%d", &moveCard2);
                            moveCard2--;
                            if (moveCard2 == moveCard + 2 || moveCard2 == moveCard - 2){
                                validInput4 = true;
                                break;
                            }
                            else{
                                printf("Please select a card 1 - 7");
                            }
                        }
                        temp = cards[moveCard];
                        cards[moveCard] = cards[moveCard2];
                        cards[moveCard2] = temp;
                    }
                    else if (moveCard - 2 <= 1){
                        //move it 2 forward
                        temp = cards[moveCard];
                        cards[moveCard] = cards[moveCard + 2];
                        cards[moveCard + 2] = temp;
                    }
                    else{
                        // move it 2 back
                        temp = cards[moveCard];
                        cards[moveCard] = cards[moveCard - 2];
                        cards[moveCard - 2] = temp;
                    }
                }
                strcpy(centerCards[cardChoice].place, "Discard");
                validInput3 = true;
            }
        }
        printf("New deck:\n");
        for (int i = 0; i < 7; i++) {
            printf("|%-12s| ", player->hand[i].action);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|     %d     | ", player->hand[i].value);
        }
        printf("\n");
        for (int i = 0; i < 7; i++) {
            printf("|  Option:[%d]| ", i + 1);
        }
        printf("\n");

    }
}

bool winner(Player* player){
    int counter = 0;
    for (int i = 0; i < 6; i++){
        if (player->hand[i].value < player->hand[i+1].value){
            counter++;
        }
        else{
            break;
        }
    }
    if (counter == 6){
        return true;
    }
    else{
        return false;
    }
}
