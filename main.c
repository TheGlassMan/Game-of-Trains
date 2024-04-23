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

// add function to shuffle what card you get and know when a card is not in the stack and when in player hand or center
// add a function that detects how many players are playing
void assignCards(card cards[], FILE* inp);
void shuffleCards(card cards[], int numCards);
void startDeck(card cards[], Player* player, int numCards);
void showDeck(Player* player); //Show player their deck
bool isCardAvailable(card cards, card deck[], int numCards);
int main(){
    srand(time(NULL));
    FILE* cardfile = NULL;
    cardfile = fopen("sampledeck.txt", "r");
    card cards[84];
    Player players[4];
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
            showDeck(&players[0]); //player 1 goes first
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
    printf("+---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+\n");
    printf("|       Ability       |  |       Ability       |  |       Ability       |  |       Ability       |  |       Ability       |  |       Ability       |  |       Ability       |\n");
    printf("+---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+\n");
    for (int i = 0; i < 7; i++) {
        printf("| %-20s |  ", player->hand[i].action);
    }
    printf("\n");
    for (int i = 0; i < 7; i++) {
        printf("|       %5d         |  ", player->hand[i].value);
    }
    printf("\n");
    printf("+---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+  +---------------------+\n");
}
