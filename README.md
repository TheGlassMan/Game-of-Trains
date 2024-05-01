This is strictly for education purposes

### The following code has errors as of (5/1/2024):
* Protect can crash the game if the string is too long
  * (Has to do with string overflow)
* When discarding cards from the center they are "invisible"
  * this is mostly to do with this line of code:
    ```
    if (strcmp(cards[i].place, "Discard") == 0) {
    centerCards[i] = centerCards[i+1]; //tried fixing it with this but no luck
    }
    else{
    printf("|%-12s| ", centerCards[i].action);
    }
* Game being unplayable with more than 2 people bc of printing issues
