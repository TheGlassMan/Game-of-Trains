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
void round1Move(Player* player, int playerNum, card cards[]);
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
            for (int i = 0; i < numPlayers; i++) {
                printf("Player %d Deck: \n", i + 1);
                showDeck(&players[i]);

            }
        for (int i = 0; i < numPlayers; i++) {
            round1Move(&players[i], i + 1, cards);
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

void round1Move(Player* player, int playerNum, card cards[]) {
    int i = rand() % 84;
    int k;
    int cardHand, cardDeck; //card hand is the one in hand and card deck is the ones from the deck you grab
    int randNum[4];
    card temp;
    bool validInput = false;
    printf("Player %d turn:\n", playerTurn);
    printf("Card on table: \n");
    for (int j = 0; j < 4; j++) {
        i = rand() % 84;
        if ((strcmp(cards[i].place, "Hand") == 0 && strcmp(cards[i].place, "Table") == 0)) {
            j--;
        } else {
            randNum[j] = i;
        }
    }
    for (k = 0; k < playerNum; k++) {
        printf("|%-12s| ", cards[randNum[k]].action);
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|     %d     | ", cards[randNum[k]].value);
    }
    printf("\n");
    for (k = 0; k < playerNum; k++) {
        printf("|  Option:[%d]| ", k + 1);
    }
    printf("\n\n");
    if (k == 1) {
        while (!validInput) {
            printf("Which card would you like to swap from your deck with Card[%d]? \n", k);
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
                strcpy(cards[randNum[cardDeck]].place, "Hand");
                temp = cards[randNum[cardDeck]];
                player->hand[cardHand] = temp;
                cards[randNum[cardDeck]] = player->hand[cardHand];
                strcpy(cards[randNum[cardDeck]].place, "Center");
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
    if (k >= 2) {
        while (!validInput) {
            printf("Which card from the deck would you like to keep and swap with your hand? %d\n", k);
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
            printf("Which card would you like to swap from your hand? \n");
            scanf("%d", &cardHand);
            cardHand--;

            if (cardHand >= 0 && cardHand < 7) {
                validInput = true;
                strcpy(cards[randNum[cardDeck]].place, "Hand");
                temp = cards[randNum[cardDeck]];
                player->hand[cardHand] = temp;
                cards[randNum[cardDeck]] = player->hand[cardHand];
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

