// NOTE: Current version only works for 2 players :(
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
typedef struct {
    int value;
    char action[15];
    char place[10]; //i.e in hand, center of table, or in deck or in used pile
} card;
typedef struct {
    card hand[7];
} Player;
int playerTurn = 1;

void assignCards(card cards[], FILE* inp);
void shuffleCards(card cards[], int numCards);
void startDeck(card cards[], Player* player, int numCards);
void showDeck(Player* player); //Show player their deck
bool isCardAvailable(card cards, card deck[], int numCards);
void round1Move(Player* player, int playerNum, card cards[], card centerCards[]);
void gamePlay(Player* player, int playerNum, card cards[], card centerCards[]);
bool winner(Player* player);
int main() {
    srand(time(NULL));

    card cards[84];
    Player players[4];
    card centerCards[6];
    bool winnerCheck = false;
    int numPlayers; //number of players 2-4
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
        fclose(cardfile);
        printf("How many players are playing? (2-4)\n");
        scanf("%d", &numPlayers);
        while (numPlayers < 2 || numPlayers > 4) {
            printf("Invalid number of players. Please enter a number between 2 and 4: \n");
            scanf("%d", &numPlayers);
        }
        for (int i = 0; i < numPlayers; i++) {
            startDeck(cards, &players[i], 7);
        }
        for (int i = 0; i < numPlayers; i++) {
            round1Move(&players[i], i + 1, cards, centerCards);
        }
        int i = 0;
        while (!winner(&players[i])) {
            gamePlay(&players[i], i + 1, cards, centerCards);
            winner(&players[i]);
            if (i == 4) {
                i = 0;
            }
            else{
                i++;
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
    shuffleCards(cards, 84);
    for (int i = 0; i < numCards; i++) {
        strcpy(cards[i].place, "Hand");
        player->hand[i] = cards[i];
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
bool isCardAvailable(card cards, card deck[], int numCards) {
    for (int i = 0; i < numCards; i++) {
        if (deck[i].value == cards.value && strcmp(deck[i].action, cards.action) == 0) {
            return true;
        }
    }
    return false;
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
    //Start the acutal game lol
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
