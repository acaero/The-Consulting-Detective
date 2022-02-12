// Project: Win@Cluedo by David Schäfer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//This structure shall contain every card that is in the deck
typedef struct x
{
    int number;
    char *card;
    int cat;
    struct x *next;
}
x;

//dRunner is gonna be used by multiple functions to search through x-nodes
x *dRunner;

//Creates a node-structure where each node holds a player and a maximum of 3 cards (one of each category), from which that player definitely has at least one.
//The player and cards are represented as integers, which correspond to the place of the string in the corresponding name_players-array
//When the number 99 is assigned to one of these ints, it basically means that that variable is emtpy
typedef struct TBC
{
    int player;
    int location;
    int suspect;
    int weapon;
    struct TBC *next;
} TBC;

//dRunner is gonna be used by multiple functions to search through tbc-nodes
TBC *tRunner;


//array sizes
int number_locations;
int number_suspects;
int number_weapons;
int number_players;
int greatest_number;
char *version;


//Solutions, 99 if unknown
int final_location = 99;
int final_suspect = 99;
int final_weapon = 99;

//functions
int get_number(char *thing);
char *get_name(char *thing, int number);
int check_card(char *name, int *Acat, int *Acard, x *node);
int check_player (char *player, int *Aplayer, char *name_players[]);
void lock_in(int *Acat, int *Acard, int *Aplayer,int locked[][greatest_number][number_players]);
int add_node_deck(int number, char *card, int cat, x *node);
int add_node_tbc(int player, int location, int suspect, int weapon, TBC *tbc);
void deduce_locked (int locked[][greatest_number][number_players]);
void update_final (int locked[][greatest_number][number_players]);


int main(void)
{

    //Contents of the german walking dead version of Cluedo
    char *Orte[9] = {"Haupttor", "Wachturm", "Krankenstation", "Friedhof", "Waffenkammer", "Kirche", "Küche", "Wohnzimmer", "Vorratskammer"};
    char *Verdächtige[6] = {"Rick", "Daryl", "Glenn", "Michonne", "Carol", "Negan"};
    char *Waffen[6] = {"Revolver", "Armbrust", "Lucille", "Messer", "Handbeil", "Schrotflinte"};



    printf("\nCLUEDO\n");

    //Get name of person, using this program. This variable stores the active Player, which in the beginning is the user.
    char *thing = "player";
    number_players = get_number(thing);

    //get the names of the other players
    printf("\nPlease name all players clockwise, starting with yourself.\n");
    char *name_players[number_players];
    for (int i = 0; i < number_players; i++)
    {
        name_players[i] = get_name(thing, i + 1);
    }
    //add the user to the array of player names
    char *user = name_players[0];
    int A_asker = 0;


    //ask for version in order to eventually skip typing in every card in the game manualy
    do
    {
        version = get_name("", 99);
    }
    while (strcmp(version, "y") != 0 && strcmp(version, "n") != 0);


    //create first node to contain the first card of the game
    x deck;
    deck.card = NULL;
    deck.next = NULL;


    if (strcmp(version, "y") == 0)
    {
        number_locations = 9;
        number_suspects = 6;
        number_weapons = 6;

        add_node_deck(0, "Haupttor", 0, &deck);add_node_deck(1, "Wachturm", 0, &deck);add_node_deck(2, "Krankenstation", 0, &deck);add_node_deck(3, "Friedhof", 0, &deck);
        add_node_deck(4, "Waffenkammer", 0, &deck);add_node_deck(5, "Kirche", 0, &deck);add_node_deck(6, "Küche", 0, &deck);add_node_deck(7, "Wohnzimmer", 0, &deck); add_node_deck(8, "Vorratskammer", 0, &deck);

        add_node_deck(0, "Rick", 1, &deck);add_node_deck(1, "Daryl", 1, &deck);add_node_deck(2, "Glenn", 1, &deck);add_node_deck(3, "Michonne", 1, &deck);add_node_deck(4, "Carol", 1, &deck);add_node_deck(5, "Negan", 1, &deck);

        add_node_deck(0, "Revolver", 2, &deck);add_node_deck(1, "Armbrust", 2, &deck);add_node_deck(2, "Lucille", 2, &deck);add_node_deck(3, "Messer", 2, &deck);add_node_deck(4, "Handbeil", 2, &deck);add_node_deck(5, "Schrotflinte", 2, &deck);
    }

    else
    {
        printf("\nLocations\n");
        thing = "location";
        number_locations = get_number(thing);
        for (int i = 0; i < number_locations; i++)
        {
            add_node_deck(i, get_name(thing, i + 1), 0, &deck);
        }

        printf("\nSuspects\n");
        thing = "suspect";
        number_suspects = get_number(thing);
        for (int i = 0; i < number_suspects; i++)
        {
            add_node_deck(i, get_name(thing, i + 1), 1, &deck);
        }

        printf("\nWeapons\n");
        thing = "weapon";
        number_weapons = get_number(thing);
        for (int i = 0; i < number_weapons; i++)
        {
            add_node_deck(i, get_name(thing, i + 1), 2, &deck);
        }
    }


    //checks which category has the most cards
    greatest_number = number_locations;
    if (number_weapons > number_locations) {greatest_number = number_weapons;}
    if (number_suspects > number_locations) {greatest_number = number_suspects;}


    //creates a bool-table for each category (locations, suspects, weapons). Sets every cell to FALSE initialy.
    int locked[3][greatest_number][number_players];
    for (int h = 0; h < 3; h++)
    {
        for (int i = 0; i < greatest_number; i++)
        {
            for (int j = 0; j < number_players; j++)
            {
                if (h == 0 && i >= number_locations)
                {
                    locked[h][i][j] = 1;
                }
                else if (h == 1 && i >= number_suspects)
                {
                    locked[h][i][j] = 1;
                }
                else if (h == 2 && i >= number_weapons)
                {
                    locked[h][i][j] = 1;
                }
                else
                {
                    locked[h][i][j] = 0;
                }
            }
        }
    }





    //The location of the following two variables will be used by the check_card function. They will contain the category-number and card-number of a certain card.
    //Will be reused permanently to contain the currently relevant card.
    int Acat;
    int Acard;

    //Get number, name and category of starting cards, assigns them to the user
    printf("\nHow many cards do you already know (cards in your hand + possibly community cards)? \n");
    thing = "starting-card";
    int starting_cards = get_number(thing);
    char *card_buffer;

    for (int i = 0; i < starting_cards; i++)
    {
        card_buffer = get_name(thing, i + 1);

        if (check_card(card_buffer, &Acat, &Acard, &deck) == 1)
        {
            printf("\nThat card does not exist, try again.\n");
            i--;
        }
        else
        {
            lock_in(&Acat, &Acard, &A_asker, locked);
        }

        free(card_buffer);
    }

    //Document that the user doesnt have all of the other cards in the deck
    for (int m = 0; m < 3; m++)
    {
        for (int y = 0; y < greatest_number; y++)
        {
            if (locked[m][y][A_asker] != 2) {locked[m][y][A_asker] = 1;}
        }
    }




    printf("\n--Error Checking--\n\n");

    printf("Contents of name_players:\n");
    for (int i = 0; i < number_players; i++) {printf("%i %s\n",i, name_players[i]);}

    printf("\nContents of deck:\n");
    dRunner = &deck;

    while (1)
    {
        if (dRunner->card != NULL) {printf("%i %s %i\n", dRunner->number, dRunner->card, dRunner->cat);}
        if (dRunner->next != NULL) {dRunner = dRunner->next;}
        else {break;}
    }
    printf("\n--Error Checking--\n");

//----------------------------------------------- ROUND LOOP

    //Unelegante aber effektive Lösung damit der erste node immer bestehen bleibt. Das macht das free()n später einfacher
    TBC tbc;
    tbc.player = 111;
    tbc.location = 111;
    tbc.suspect = 111;
    tbc.weapon = 111;
    tbc.next = NULL;

    //Initializes a bunch of variables twice, once as a string and once as an int that represents the position of the string in an array | yvn stores either y or n (yes or no) | R_asker and R_answerer are both player names
    char *R_asker;
    char *R_location;
    char *R_weapon;
    char *R_suspect;
    char *R_answerer;
    char *yvn;
    int A_location;
    int A_suspect;
    int A_weapon;
    int A_answerer;
    char *final_location_name;
    char *final_suspect_name;
    char *final_weapon_name;
    char *shown;
    TBC *tRunner_buffer = NULL;

    for (int R = 1; ; R++)
    {

        printf("\n\nRUNDE %i\n\n", R);


        R_asker = get_name("", 22);
        while(check_player(R_asker, &A_asker, name_players))
        {
            free(R_asker);
            printf("Player doesn't exist, try again.\n");
            R_asker = get_name("", 22);
        }

        if (strcmp(R_asker, user) == 0)
        {    
            if (strcmp(version, "y") == 0)
            {
            printf("\nFrag nach den Dingen, zu denen noch am wenigsten Informationen vorliegen.\nAußer wenn in einer Kategorie schon die Lösung gefunden wurde, dann die eigene Karte nehmen, die die meisten schon kennen.\nOder noch besser: Frag nach Dingen die drei Leute nicht haben!\n");
            }
        }

        //get Location
        R_location = get_name("", 33);
        while(check_card(R_location, &Acat, &A_location, &deck) == 1 || Acat != 0)
        {
            free(R_location);
            printf("Location doesn't exist, try again.\n");
            R_location = get_name("", 33);
        }

        //get suspect
        R_suspect = get_name("", 44);
        while(check_card(R_suspect, &Acat, &A_suspect, &deck) == 1 || Acat != 1)
        {
            free(R_suspect);
            printf("Suspect doesn't exist, try again.\n");
            R_suspect = get_name("", 44);
        }

        //get Weapon
        R_weapon = get_name("", 55);
        while(check_card(R_weapon, &Acat, &A_weapon, &deck) == 1 || Acat != 2)
        {
            free(R_weapon);
            printf("Weapon doesn't exist, try again.\n");
            R_weapon = get_name("", 55);
        }


        //Did somebody show a card?
        yvn = get_name("", 66);
        while(strcmp(yvn, "y") != 0 && strcmp(yvn, "n") != 0)
        {
            free(yvn);
            printf("Please type in either ""n"" for no or ""y"" for yes.");
            yvn = get_name("", 66);
        }



        if (strcmp(yvn, "y") == 0)
        {

            //get Answerer
            R_answerer = get_name("", 77);
            while (check_player(R_answerer, &A_answerer, name_players))
            {
                free(R_answerer);
                printf("Player doesn't exist, try again.");
                R_answerer = get_name("", 77);
            }

            if (strcmp(R_asker, user) == 0)
            {
                shown = get_name("", 88);

                while(check_card(shown, &Acat, &Acard, &deck) == 1)
                {
                    free(shown);
                    printf("Card doesn't exist, try again.\n");
                    shown = get_name("", 88);
                }

                for (int u = 0; u < number_players; u++)
                {
                    if (u == A_answerer){locked[Acat][Acard][u] = 2;}
                    else {locked[Acat][Acard][u] = 1;}
                }
            }
            else
            {
                //The person who was able to show a card has one of the three cards - create a tbc node for that
                add_node_tbc(A_answerer, A_location, A_suspect, A_weapon, &tbc);
            }

            // Everybody who could not show a card doesnt have any of the 3 cards - lock that in
            for (int x = A_asker + 1; x != A_answerer; x++)
            {
                if (x == number_players) {x = 0;}
                if (x == A_answerer) {break;}
                locked[0][A_location][x] = 1;
                locked[1][A_suspect][x] = 1;
                locked[2][A_weapon][x] = 1;
            }


        }
        else // yvn = n
        {
            //Everyone exept for the asker definitely doesnt have any of the 3 cards
            for (int i = 0; i < number_players; i++)
            {
                if (i != A_asker)
                {
                locked[0][A_location][i] = 1;
                locked[1][A_suspect][i] = 1;
                locked[2][A_weapon][i] = 1;
                }
            }
        }


        if (strcmp(yvn, "y") == 0)
        {
        free(R_answerer);
        if (strcmp(R_asker, user) == 0){free(shown);}
        }

        free(R_asker);
        free(R_location);
        free(R_suspect);
        free(R_weapon);
        free(yvn);




        //in der locked liste logische schlussfolgerungen ziehen
        deduce_locked(locked);
        update_final(locked);

        //Alle TBCs durchgehen
        tRunner = tbc.next;
        tRunner_buffer = &tbc;
        if (tRunner != NULL)
        {
            for (int t = 0;;t++)
            {
                if (locked[0][tRunner->location][tRunner->player] == 2 || locked[1][tRunner->suspect][tRunner->player] == 2 || locked[2][tRunner->weapon][tRunner->player] == 2)
                {
                    tRunner->location = tRunner->suspect = tRunner->weapon = 99;
                }


                for (int i = 0; i < number_players; i++)
                {
                    if (i == tRunner->player)
                    {
                        //Checkt ob wir schon wissen, dass die Person eine der drei Karten nicht hat
                        if (locked[0][tRunner->location][i] == 1) {tRunner->location = 99;}
                        if (locked[1][tRunner->suspect][i] == 1) {tRunner->suspect = 99;}
                        if (locked[2][tRunner->weapon][i] == 1) {tRunner->weapon = 99;}
                    }
                    else
                    {
                        //Checkt ob wir wissen dass eine andere Person eine der drei Karten hat, dann kann die Person in question die Karte nicht haben
                        if (locked[0][tRunner->location][i] == 2) {tRunner->location = 99;}
                        if (locked[1][tRunner->suspect][i] == 2) {tRunner->suspect = 99;}
                        if (locked[2][tRunner->weapon][i] == 2) {tRunner->weapon = 99;}
                    }
                }


                if (tRunner->location + tRunner->suspect + tRunner->weapon >= 198 && tRunner->location + tRunner->suspect + tRunner->weapon < 297)
                {
                    if (tRunner->location != 99) {locked[0][tRunner->location][tRunner->player] = 2; tRunner->location = 99;}
                    if (tRunner->suspect != 99) {locked[1][tRunner->suspect][tRunner->player] = 2; tRunner->suspect = 99;}
                    if (tRunner->weapon != 99) {locked[2][tRunner->weapon][tRunner->player] = 2; tRunner->weapon = 99;}
                }



                if (tRunner->location + tRunner->suspect + tRunner->weapon == 297)
                {

                    if (tRunner->next == NULL)
                    {
                        tRunner_buffer->next = NULL;
                        free(tRunner);
                        break;
                    }
                    else
                    {
                        tRunner_buffer->next = tRunner->next;
                        free(tRunner);
                        tRunner = tRunner_buffer->next;
                    }

                }
                else
                {
                    if (tRunner->next == NULL) {break;}
                    else
                    {
                        tRunner_buffer = tRunner;
                        tRunner = tRunner->next;

                    }
                }


            }
        }

        //Check if the murderer is identified
        deduce_locked(locked);
        update_final(locked);


        printf("\n------------Background------------\n\n");

        for (int i = 0; i < number_locations; i++)
        {
            printf("%i - ", i);
            for (int j = 0; j < number_players; j++)
            {
                printf("%d ", locked[0][i][j]);
            }
            if (strcmp(version, "y") == 0)
            {
                printf("%s", Orte[i]); 
            }
            if (i == final_location) {printf("<--------------");}
            printf("\n");
        }
        printf("\n");


        for (int i = 0; i < number_suspects; i++)
        {
            printf("%i - ", i);
            for (int j = 0; j < number_players; j++)
            {
                printf("%d ", locked[1][i][j]);
            }
            if (strcmp(version, "y") == 0)
            {
                printf("%s", Verdächtige[i]); 
            }
            if (i == final_suspect) {printf("<--------------");}
            printf("\n");
        }
        printf("\n");

        for (int i = 0; i < number_weapons; i++)
        {
            printf("%i - ", i);
            for (int j = 0; j < number_players; j++)
            {
                printf("%d ", locked[2][i][j]);
            }
            if (strcmp(version, "y") == 0)
            {
                printf("%s", Waffen[i]);                
            }
            if (i == final_weapon) {printf("<--------------");}
            printf("\n");
        }
        printf("\n");




        tRunner = tbc.next;
        while (1)
        {
            if (tRunner!= NULL)
            {
                printf("%s -- %i, %i, %i\n",name_players[tRunner->player], tRunner->location, tRunner->suspect, tRunner->weapon);
                tRunner = tRunner->next;
            }
            else
            {
                break;
            }
        }


        printf("\n------------Background------------\n");




        if (final_location != 99 && final_suspect != 99 && final_weapon != 99)
        {
            for (dRunner = &deck; dRunner != NULL; dRunner = dRunner->next)
            {
                if (dRunner->cat == 0 && dRunner->number == final_location) {final_location_name = dRunner->card;}
                if (dRunner->cat == 1 && dRunner->number == final_suspect) {final_suspect_name = dRunner->card;}
                if (dRunner->cat == 2 && dRunner->number == final_weapon) {final_weapon_name = dRunner->card;}
            }
            printf("\n\n------SOLUTION------\n\nLocation: %s\nSuspect: %s\nWeapon: %s\n\n--------------------", final_location_name, final_suspect_name, final_weapon_name);
            break;

        }
    }

//-----------------------------------------------------------------------------------------------Free



    //Frees Players
    for (int i = 0; i < number_players; i++) {free(name_players[i]);}

    //Frees deck-node
    dRunner = &deck;
    x *dRunner_buffer;
    //free name in first node, that first node was manualy created and therefor does not have to be freed
    if (dRunner->card != NULL && strcmp(version, "y") != 0) {free(dRunner->card);}
    if (dRunner->next != NULL) {dRunner = dRunner->next;}
    //free all followin nodes and the names therein
    while (1)
    {
        if (dRunner->card != NULL && strcmp(version, "y") != 0) {free(dRunner->card);}
        if (dRunner->next != NULL)
        {
            dRunner_buffer = dRunner->next;
            free(dRunner);
            dRunner = dRunner_buffer;
        }
        else
        {
            free(dRunner);
            break;
        }
    }

    //Frees tbc-nodes
    tRunner = tbc.next;

    while (1)
    {
        if (tRunner->next != NULL)
        {
            tRunner_buffer = tRunner->next;
            free(tRunner);
            tRunner = tRunner_buffer;
        }
        else
        {
            free(tRunner);
            break;
        }
    }

    //free version-String
    free(version);




    return 0;
}







//------------------------------------------------------------------------------------------------ Functions









int get_number(char *thing)
{
    int number, status, temp;
    printf("Number of %ss: ", thing);

    //Die scanf-Funktion gibt die Anzahl der Dinge zurück die sie vom Keyboard ausliest, in meinem Fall: 1 Nummer.
    status = scanf("%i", &number);
    while(status!=1)
    {
        //In dieser While-Schleife wird solange die Funktion getchar() ausgeführt bis, dass Ergebnis false (v 0) ist. getchar() liest den zuletzt eingetippten Character.
		while((temp=getchar()) != EOF && temp != '\n');
		printf("Number of %ss: ", thing);
		status = scanf("%d", &number);
	}
    return number;
}


char *get_name(char *thing, int number)
{
    char *buffer;
    int name_length = 0;

    //Gets a string of max. lenght 30 as an Input
    char *name = malloc(sizeof(char) * 30);
    if (name == NULL)
    {
        printf("No memory available 1\n");
        return NULL;
    }

    switch (number)
    {
        case 99:
        printf("\nAre you playing the ""Walking Dead Version"" of Cluedo? (y/n)  ");
        break;

        case 22:
        printf("\nActive Player:  ");
        break;

        case 33:
        printf("\nLocation:  ");
        break;

        case 44:
        printf("Suspect:  ");
        break;

        case 55:
        printf("Weapon:  ");
        break;

        case 66:
        printf("\nDid somebody show a card? (y/n)  ");
        break;

        case 77:
        printf("\nPerson that showed the card:  ");
        break;

        case 88:
        printf("\nWhich card was showned?  ");
        break;

        default:
        printf("Name of %s %i:  ", thing, number);
    }

    scanf("%s", name);

    //Relocates that string to a location where it only takes up as much memory as it needs
    while (*(name + name_length) != '\0') {name_length++;}
    buffer = realloc(name, sizeof(char) * (name_length + 1));
    if (buffer == NULL)
    {
        printf("No memory available 2\n");
        return NULL;
    }
    name = buffer;

    return name;
}


//Takes card name as Input. If card exists return 0, else 1. If 0 change Acat to category number and Acard to number of card in the array.
int check_card(char *name, int *Acat, int *Acard, x *node)
{

    dRunner = node;

    while (1)
    {

        if (strcmp(name, dRunner->card) == 0)
        {
            *Acat = dRunner->cat;
            *Acard = dRunner->number;
            return 0;
        }

        if (dRunner->next != NULL)
        {
            dRunner = dRunner->next;
        }
        else
        {
            return 1;
        }

    }

}


//Takes player name as Input. If player exists return corresponding number in player array.
int check_player (char *player, int *Aplayer, char *name_players[])
{
    for (int i = 0; i < number_players; i++)
    {
        if (strcmp(player, name_players[i]) == 0)
        {
            *Aplayer = i;
            return 0;
        }
    }
    return 1;
}


//If one has deduced that one player definitely has a certain card on his hand, this function locks that information into the corresponding bool array
void lock_in(int *Acat, int *Acard, int *Aplayer,int locked[][greatest_number][number_players])
{
    for (int i = 0; i < number_players; i++)
    {
        (i == *Aplayer) ? (locked[*Acat][*Acard][i] = 2) : (locked[*Acat][*Acard][i] = 1);
    }
}


int add_node_deck(int number, char *card, int cat, x *node)
{

    dRunner = node;

    if (dRunner->card == NULL)
    {
        dRunner->number = number;
        dRunner->card = card;
        dRunner->cat = cat;
    }
    else
    {

        while (dRunner->next != NULL) {dRunner = dRunner->next;}

        dRunner->next = malloc(sizeof(x));
        if (dRunner->next == NULL)
        {
            printf("no mem avail\n");
            return 1;
        }
        dRunner = dRunner->next;
        dRunner->number = number;
        dRunner->card = card;
        dRunner->cat = cat;
        dRunner->next = NULL;
    }

    return 0;
}


int add_node_tbc(int player, int location, int suspect, int weapon, TBC *tbc)
{

    tRunner = tbc;

    while(tRunner->next != NULL) {tRunner = tRunner->next;}

    tRunner->next = malloc(sizeof(TBC));
    if (tRunner->next == NULL)
    {
        printf("no mem availy");
        return 1;
    }
    tRunner = tRunner->next;

    tRunner->player = player;
    tRunner->location = location;
    tRunner->suspect = suspect;
    tRunner->weapon = weapon;
    tRunner->next = NULL;

    return 0;
}


//adds information to the locked array, based on logical deductions
void deduce_locked (int locked[][greatest_number][number_players])
{
    int zeile = 0;
    int zweier = 0;
    int fehlend = 0;
    for (int i = 0; i < number_locations; i++)
    {
        for (int j = 0; j < number_players; j++)
        {
            if (locked[0][i][j] == 2) {zweier++; zeile = 1;}
        }
        if (zeile != 1) {fehlend = i;}
        zeile = 0;
    }
    if (zweier == number_locations - 1)
    {
        for (int x = 0; x < number_players; x++)
        {
            locked[0][fehlend][x] = 1;
        }
    }

    zeile = 0;
    zweier = 0;
    fehlend = 0;
    for (int i = 0; i < number_suspects; i++)
    {
        for (int j = 0; j < number_players; j++)
        {
            if (locked[1][i][j] == 2) {zweier++; zeile = 1;}
        }
        if (zeile != 1) {fehlend = i;}
        zeile = 0;
    }
    if (zweier == number_suspects - 1)
    {
        for (int x = 0; x < number_players; x++)
        {
            locked[1][fehlend][x] = 1;
        }
    }

    zeile = 0;
    zweier = 0;
    fehlend = 0;
    for (int i = 0; i < number_weapons; i++)
    {
        for (int j = 0; j < number_players; j++)
        {
            if (locked[2][i][j] == 2) {zweier++; zeile = 1;}
        }
        if (zeile != 1) {fehlend = i;}
        zeile = 0;
    }
    if (zweier == number_weapons - 1)
    {
        for (int x = 0; x < number_players; x++)
        {
            locked[2][fehlend][x] = 1;
        }
    }


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < greatest_number; j++)
        {
            for (int k = 0; k < number_players; k++)
            {
                if (locked[i][j][k] == 2)
                {
                    for (int l = 0; l < number_players; l++)
                    {
                        if (l != k)
                        {
                            locked[i][j][l] = 1;
                        }
                    }
                }
            }
        }
    }


    int cpp = (number_locations + number_suspects + number_weapons) / number_players;
    int A_counter;
    int B_counter;
    int C_counter;

    for (int i = 0; i < number_players; i++)
    {
        A_counter = 0;
        B_counter = 0;
        C_counter = 0;

        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < greatest_number; k++)
            {
                switch (locked[j][k][i])
                {
                    case (0):
                    A_counter ++;
                    break;

                    case (1):
                    B_counter ++;
                    break;

                    case (2):
                    C_counter ++;
                }
            }
        }

        //If one knows all of the cards of a certain player, one also knows that he doesnt have any of the other cards
        if (C_counter == cpp)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < greatest_number; k++)
                {
                    if (locked[j][k][i] != 2) {locked[j][k][i] = 1;}
                }
            }
        }

        //if one knows that much about what a person doesnt have, at some point one can deduce what he does have
        if ((A_counter + B_counter + C_counter) - B_counter == cpp)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < greatest_number; k++)
                {
                    if (locked[j][k][i] == 0) {locked[j][k][i] = 2;}
                }
            }
        }



    }



    //checken wieviele nodes es in tbc gibt die sich nicht überschneiden (es gibt mehrere Kombinationsmglichkeiten - die mit der höchsten Anzahl finden)
    //Jeder überschneidungsfreier node beinhaltet dann safe eine karte die die hat
    //wenn: die Anzahl überschneidungsfreier nodes == Anzahl Handkarten - Bekannte Handkarten der Person
    //dann: Weiß man dass die Person keine der Karten hat, die nicht in einem der überschneidungsfreien nodes sind.

}


//checks if we have a solution, if yes return 1 and print it out;
void update_final (int locked[][greatest_number][number_players])
{
    int counter = 0;

    if (final_location == 99)
    {
        for (int i = 0; i < number_locations; i++)
        {
            for (int j = 0; j < number_players; j++)
            {
                counter += locked[0][i][j];
            }
            if (counter == number_players)
            {
                final_location = i;
                break;
            }
            counter = 0;
        }
    }


    if (final_suspect == 99)
    {
        for (int i = 0; i < number_suspects; i++)
        {
            for (int j = 0; j < number_players; j++)
            {
                counter += locked[1][i][j];
            }
            if (counter == number_players)
            {
                final_suspect = i;
                break;
            }
            counter = 0;
        }
    }


    if (final_weapon == 99)
    {
        for (int i = 0; i < number_weapons; i++)
        {
            for (int j = 0; j < number_players; j++)
            {
                counter += locked[2][i][j];
            }
            if (counter == number_players)
            {
                final_weapon = i;
                break;
            }
            counter = 0;
        }
    }
}
