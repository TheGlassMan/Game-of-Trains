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
int main(){
    srand(time(NULL));
    FILE* cardfile = NULL;
    cardfile = fopen("sampledeck.txt", "r");
    card cards[84];
    Player players[4];
    card centerCards[4];
    bool winnerCheck = false;
    int numPlayers; //number of players 2-4

    if (cardfile == NULL){
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
        //while (!winnerCheck){
            for (int i = 0; i < numPlayers; i++) {
                printf("Player %d Deck: \n", i + 1);
                showDeck(&players[i]);

            }
        for (int i = 0; i < numPlayers; i++) {
            round1Move(&players[i], i + 1, cards, centerCards);
        }


        //}
        return 0;
    }
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
    int cardOnTable = 4;
    int randNum[4]; // used to grab from deck
    int cardKeep = 0; //card the person decided to keep and swap and the other gets discarded
    card temp;
    bool validInput = false;
    printf("Player %d turn:\n", playerTurn);
    printf("Card on table: \n");
    for (int j = 0; j < cardOnTable; j++) {
        i = rand() % 84;
        if ((strcmp(cards[i].place, "Hand") == 0 && strcmp(cards[i].place, "Table") == 0)) {
            j--;
        } else {
            randNum[j] = i;
        }
    }
    for (int j = 0; j <= 0; j++) {
        k = 0;
        if (strcmp(centerCards[j].place, "Center") == 0){
            cards[randNum[k]] = centerCards[j];
            printf("|%-12s| ", centerCards[j].action);
            printf("\n");
            printf("|     %d     | ", centerCards[j].value);
            printf("\n");
            printf("|  Option:[%d]| ", centerPlace);
            printf("\n");
            centerPlace++;
            //playerNum--;
        }
    }
    //prints out random card to choose
    for (k = 0; k < playerNum; k++) {
        printf("|%-12s| ", cards[randNum[k+1]].action);
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|     %d     | ", cards[randNum[k+1]].value);
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|  Option:[%d]| ", centerPlace);
        centerPlace++;
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
            printf("Which card from the center table would you like to keep and swap with your hand?\n");
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

            scanf("%d", &cardDeck);
            cardDeck--;
            printf("Which card would you like to swap from your hand? \n");
            scanf("%d", &cardHand);
            cardHand--;

            if ((cardHand >= 0 && cardHand < 7) && (cardDeck >= 0 && cardDeck < 4)) {
                validInput = true;
                strcpy(cards[randNum[cardDeck]].place, "Hand"); //fix this here too
                temp = cards[randNum[cardDeck]];
                cards[randNum[cardDeck]] = player->hand[cardHand];
                player->hand[cardHand] = temp;
                strcpy(cards[randNum[cardDeck]].place, "Center");
            } else {
                printf("Invalid option. Please select a number between 1 and 7.\n");
            }
        }
        printf("New Deck Player %d\n", playerTurn);
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

    playerTurn++;
}

