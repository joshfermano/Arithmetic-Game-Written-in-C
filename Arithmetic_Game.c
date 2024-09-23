/* 
    Josh Khovick Fermano
    2022
 */
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


#define MAX_PLAYERS 100
#define MAX_NAME_LEN 50
#define MAX_PASS_LEN 50
#define PLAYER_FILE "playerusernameandpass.txt"
#define PLAYER_SC_FILE "playerscores.txt"

typedef struct {
    char username[MAX_NAME_LEN];
    int addscore;
    int subscore;
    int mulscore;
    int divscore;
    int totalscore;
    float avescore;
} PScore;

typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
} player;

//Register and Load Players
void Register(player players[], PScore pScores[], int *numPl);

//Loading Bar and Features
void loading();
void loadingbar(int total, int progress);

//Save and Log In Players
void Saveusers(player players[], int numPl);
int LoginUser(player players[], int numPl, PScore pScores[]);
void Saveplayerscores(PScore pScores[], int numPl);
void Load_user(player players[], PScore pScores[], int *numPl);
void Addition(PScore *pScores);

//Main Functions (UI)
int Menu(PScore pScores[], int currPl);
int LeadMenu(PScore pScores[], int currPl);
int PlayerFinder(PScore pScores[], int numPl, char username[]);


//Arithmetic Functions and Leader board
int leadmenu(PScore pScores[], int currentPl);
void Addgame(PScore *pScores);
void Subgame(PScore *pScores);
void Mulgame(PScore *pScores);
void Divgame(PScore *pScores);
void Addlead(PScore *pScores);
void Sublead(PScore *pScores);
void Mullead(PScore *pScores);
void Divlead(PScore *pScores);
void Ranklead(PScore *pScores);

int currentPl = -1;

void Register(player players[], PScore pScores[], int *numPl) {
    if (*numPl >= MAX_PLAYERS) {
        printf("Maximum number of players has reached.\n");
        return;
    }
    player newPlayer;
    printf("Enter Username: ");
    scanf("%s", newPlayer.username);
    printf("Enter your password: ");
    scanf("%s", newPlayer.password);
    for (int i = 0; i < *numPl; i++) {
        if (strcmp(players[i].username, newPlayer.username) == 0) {
            printf("Username is already used.\n");
            system("pause");
            return;
        }
    }
    players[*numPl] = newPlayer;
    PScore newScore = {0};
    strcpy(newScore.username, newPlayer.username);
    pScores[*numPl] = newScore;
    *numPl += 1;
    FILE *fp = fopen(PLAYER_FILE, "a");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }

    fprintf(fp, "%s 0 0 0 0\n", newPlayer.username);
    fclose(fp);

    fp = fopen(PLAYER_FILE, "a");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    fprintf(fp, "%s %s\n", newPlayer.username, newPlayer.password);
    fclose(fp);
    printf("-----------------------------------------------------------\n");
    printf("Saving in Progress.\nThe Username and Password was succesfully created.\n");
    printf("Loading...Please Wait...\n");
    int total = 100;
    for (int i = 0; i <= total; i++) {
        loadingbar(total, i);
        usleep(10000);
    }
    return;
}

void Loadplayerscores(PScore pScores[], int numPl) {
    FILE *fp = fopen(PLAYER_SC_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    int ct = 0;
    while (!feof(fp) && ct < numPl) {
        char li[100];
        fgets(li, sizeof(li), fp);
        if (sscanf(li, "%s %d %d %d %d", pScores[ct].username, &pScores[ct].addscore, &pScores[ct].subscore, &pScores[ct].mulscore, &pScores[ct].divscore) == 5) {
            pScores[ct].totalscore = pScores[ct].addscore + pScores[ct].subscore + pScores[ct].mulscore + pScores[ct].divscore;
            pScores[ct].avescore = pScores[ct].totalscore / 4.0;
            ct++;
        }
    }
    fclose(fp);
    for (int i = 0; i < numPl; i++) {
        for (int j = i + 1; j < numPl; j++) {
            if (pScores[i].avescore < pScores[j].avescore) {
                PScore temp = pScores[i];
                pScores[i] = pScores[j];
                pScores[j] = temp;
            }
        }
    }
}

int PlayerFinder(PScore pScores[], int numPl, char username[]) {
    for (int i = 0; i < numPl; i++) {
        if (strcmp(pScores[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

int LoginUser(player players[], int numPl, PScore pScores[]) {
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    int index = PlayerFinder(pScores, numPl, username);
    if (index == -1) {
        printf("Invalid Username or Password.\n");
        system("pause");
        return -1;
    }
    if (strcmp(players[index].password, password) != 0) {
        printf("Invalid Username or password.\n");
        system("pause");
        return -1;
    }
  printf("-------------------------------------\n");
    printf("Log In Successful.\n");
    printf("Loading...Please Wait for the Game.\n");
    int total = 100;
    for (int i = 0; i <= total; i++) {
    loadingbar(total, i);
    usleep(10000);
    }
    currentPl = index;
    return index;
}

void Saveusers(player players[], int numPl) {
     FILE *fp = fopen(PLAYER_FILE, "w");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        fprintf(fp, "%s %s\n", players[i].username, players[i].password);
    }
    fclose(fp);
}

void Load_user(player players[], PScore pScores[], int *numPl) {
    FILE *fp = fopen(PLAYER_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    *numPl = 0;
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    while (fscanf(fp, "%s %s\n", username, password) == 2) {
        player newPlayer;
        strcpy(newPlayer.username, username);
        strcpy(newPlayer.password, password);
        players[*numPl] = newPlayer;
        strcpy(pScores[*numPl].username, username);
        *numPl += 1;
    }
    fclose(fp);
}

void Saveplscorelead(PScore pScore) {
    FILE *fp = fopen(PLAYER_SC_FILE, "a");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    fprintf(fp, "%s %d %d %d %d\n", pScore.username, pScore.addscore, pScore.subscore, pScore.mulscore, pScore.divscore);
    fclose(fp);
}

void Saveplayerscores(PScore pScores[], int numPl) {
     FILE *fp = fopen(PLAYER_SC_FILE, "w");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        fprintf(fp, "%s %d %d %d %d\n", pScores[i].username, pScores[i].addscore, pScores[i].subscore, pScores[i].mulscore, pScores[i].divscore);
    }
    fclose(fp);
}

void loading()
{
    char a = 177, b = 219;
    printf("\n\n\n\n");
    printf("\n\n\n\n\t\t\t\t\t""Loading...\n\n");
    printf("\t\t\t\t\t");
    for (int i = 0; i < 26; i++)
        printf("%c", a);
    printf("\r");
    printf("\t\t\t\t\t");
    for (int i = 0; i < 26; i++) {
        printf("%c", b);
        Sleep(100);
    }
}

void loadingbar(int total, int progress) {
    int barWidth = 40;
    int percent = (progress * 100) / total;
    int numBars = (progress * barWidth) / total;
    printf("[");
    for (int i = 0; i < barWidth; i++) {
        if (i < numBars) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\r", percent);
    fflush(stdout);
}

void Addgame(PScore *pScore) {
     int numcor = 0, pl_ans = 0, ans = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 101;
        b = rand() % 101;
        ans = a + b;
        printf("%d + %d = ", a, b);
        scanf("%d", &pl_ans);
        if (pl_ans != ans) {
             printf("\nYour Answer is Incorrect.\nThe correct answer is %d.\n", ans);
            printf("===========================================\n");
            break;
        }
        numcor++;
    }
    if (numcor > pScore->addscore) {
        pScore->addscore = numcor;
        printf("CONGRATULATIONS! New High Score: %d\n", pScore->addscore);
        Saveplscorelead(*pScore);
        system("pause");
    } else {
        printf("Player Score: %d\n", numcor);
        system("pause");
    }
}

void Subgame(PScore *pScore){
    int numcor = 0, pl_ans = 0, ans = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 101;
        b = rand() % 101;
        if (b > a) {
            int temp = a;
            a = b;
            b = temp;
        }
        ans = a - b;
        printf("%d - %d = ", a, b);
        scanf("%d", &pl_ans);
        if (pl_ans != ans) {
            printf("\nYour Answer is Incorrect.\nThe correct answer is %d.\n", ans);
            printf("===========================================\n");
            break;
        }
        numcor++;
    }
    if (numcor > pScore->subscore) {
        pScore->subscore = numcor;
        printf("CONGRATULATIONS! New High Score: %d\n", pScore->subscore);
        Saveplscorelead(*pScore);
        system("pause");
    } else {
        printf("Player Score: %d\n", numcor);
        system("pause");
    }
}
void Mulgame(PScore *pScore) {
    int numcor = 0, pl_ans = 0, ans = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 13;
        b = rand() % 13;
        ans = a * b;
        printf("%d x %d = ", a, b);
        scanf("%d", &pl_ans);
        if (pl_ans != ans) {
            printf("\nYour Answer is Incorrect.\nThe correct answer is %d.\n", ans);
            printf("===========================================\n");
            break;
        }
        numcor++;
    }
    if (numcor > pScore->mulscore) {
        pScore->mulscore = numcor;
        printf("CONGRATULATIONS! New High Score: %d\n", pScore->mulscore);
        Saveplscorelead(*pScore);
        system("pause");
    } else {
        printf("Player Score: %d\n", numcor);
        system("pause");
    }
}
void Divgame(PScore *pScore) {
    int numcor = 0, pl_ans = 0, ans = 0;
        int a = 0, b = 0, c = 0;
        srand(time(0));

        while (1) {
            a = rand() % 13;
            b = rand() % 13;
            c = a * b;
            printf("%d / %d = ", c, a);
            scanf("%d", &pl_ans);
        if (pl_ans != b) {
        printf("\nYour Answer is Incorrect.\nThe correct answer is %d.\n", b);
        printf("===========================================\n");
            break;
        }
        numcor++;
    }
    if (numcor > pScore->divscore) {
        pScore->divscore = numcor;
        printf("CONGRATULATIONS! New High Score: %d\n", pScore->divscore);
        Saveplscorelead(*pScore);
        system("pause");
    } else {
        printf("Player Score: %d\n", numcor);
        system("pause");
    }
}

void Addlead(PScore pScores[]) {
    int numPl = 0;
    char li[100];
    PScore tempScores[MAX_PLAYERS];
    FILE *fp = fopen(PLAYER_SC_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    while (fgets(li, sizeof(li), fp)) {
        if (sscanf(li, "%s %d %d %d %d", tempScores[numPl].username, &tempScores[numPl].addscore, &tempScores[numPl].subscore, &tempScores[numPl].mulscore, &tempScores[numPl].divscore) == 5) {
            tempScores[numPl].totalscore = tempScores[numPl].addscore + tempScores[numPl].subscore + tempScores[numPl].mulscore + tempScores[numPl].divscore;
            tempScores[numPl].avescore = tempScores[numPl].totalscore / 4.0;
            numPl++;
        }
    }
    fclose(fp);

    if (numPl == 0) {
        printf("No Players Found.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        for (int j = i + 1; j < numPl; j++) {
            if (tempScores[i].addscore < tempScores[j].addscore) {
                PScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    printf("Top 10 Rankings by Addition:\n");
    for (int i = 0; i < numPl && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].username, tempScores[i].addscore);
    }
    system("pause");
}

void Sublead(PScore pScores[]) {
    int numPl = 0;
    char li[100];
    PScore tempScores[MAX_PLAYERS];
    FILE *fp = fopen(PLAYER_SC_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    while (fgets(li, sizeof(li), fp)) {
        if (sscanf(li, "%s %d %d %d %d", tempScores[numPl].username, &tempScores[numPl].addscore, &tempScores[numPl].subscore, &tempScores[numPl].mulscore, &tempScores[numPl].divscore) == 5) {
            tempScores[numPl].totalscore = tempScores[numPl].addscore + tempScores[numPl].subscore + tempScores[numPl].mulscore + tempScores[numPl].divscore;
            tempScores[numPl].avescore = tempScores[numPl].totalscore / 4.0;
            numPl++;
        }
    }
    fclose(fp);

    if (numPl == 0) {
        printf("No Players Found.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        for (int j = i + 1; j < numPl; j++) {
            if (tempScores[i].subscore < tempScores[j].subscore) {
                PScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    printf("Top 10 Rankings by Subtraction:\n");
    for (int i = 0; i < numPl && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].username, tempScores[i].subscore);
    }
    system("pause");
}
void Mullead(PScore pScores[]) {
    int numPl = 0;
    char li[100];
    PScore tempScores[MAX_PLAYERS];
    FILE *fp = fopen(PLAYER_SC_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    while (fgets(li, sizeof(li), fp)) {
        if (sscanf(li, "%s %d %d %d %d", tempScores[numPl].username, &tempScores[numPl].addscore, &tempScores[numPl].subscore, &tempScores[numPl].mulscore, &tempScores[numPl].divscore) == 5) {
            tempScores[numPl].totalscore = tempScores[numPl].addscore + tempScores[numPl].subscore + tempScores[numPl].mulscore + tempScores[numPl].divscore;
            tempScores[numPl].avescore = tempScores[numPl].totalscore / 4.0;
            numPl++;
        }
    }
    fclose(fp);

    if (numPl == 0) {
        printf("No Players found.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        for (int j = i + 1; j < numPl; j++) {
            if (tempScores[i].mulscore < tempScores[j].mulscore) {
                PScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    printf("Top 10 Rankings by Multiplication: \n");
    for (int i = 0; i < numPl && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].username, tempScores[i].mulscore);
    }
    system("pause");
}

void Divlead(PScore *pScores) {
    int numPl = 0;
    char li[100];
    PScore tempScores[MAX_PLAYERS];
    FILE *fp = fopen(PLAYER_SC_FILE, "r");
    if (fp == NULL) {
        printf("There is an Error While Opening the File.\n");
        return;
    }
    while (fgets(li, sizeof(li), fp)) {
        if (sscanf(li, "%s %d %d %d %d", tempScores[numPl].username, &tempScores[numPl].addscore, &tempScores[numPl].subscore, &tempScores[numPl].mulscore, &tempScores[numPl].divscore) == 5) {
            tempScores[numPl].totalscore = tempScores[numPl].addscore + tempScores[numPl].subscore + tempScores[numPl].mulscore + tempScores[numPl].divscore;
            tempScores[numPl].avescore = tempScores[numPl].totalscore / 4.0;
            numPl++;
        }
    }
    fclose(fp);

    if (li == 0) {
        printf("No Players found.\n");
        return;
    }
    for (int i = 0; i < numPl; i++) {
        for (int j = i + 1; j < numPl; j++) {
            if (tempScores[i].divscore < tempScores[j].divscore) {
                PScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    printf("Top 10 Rankings by Division:\n");
    for (int i = 0; i < numPl && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].username, tempScores[i].divscore);
    }
    system("pause");
}

void Ranklead(PScore pScores[]) {
    printf("\n====================================\n");
    printf("\n\t--- R A N K I N G S ---\n");
    printf("\n====================================\n");
    printf("Rank\tName\tAverage Score\n");
    for (int i = 0; i < 10 && i < MAX_PLAYERS; i++) {
        printf("%d\t%s\t%.2f\n", i+1, pScores[i].username, pScores[i].avescore);
    }
     printf("---------------------------------\n");
    system("pause");
}

int main() {
    player players[MAX_PLAYERS];
    PScore pScores[MAX_PLAYERS];
    int numPl = 0;

    Load_user(players, pScores, &numPl);
    Loadplayerscores(pScores, numPl);

    while (1) {
        int opt = -1;
        if (currentPl == -1) {
           system("cls");
            printf("\n\t\t==============================\n");
            printf("\n\t\t|      G A M E  M E N U      |\n");
            printf("\n\t\t==============================\n\n");
            printf("***************************************************************\n");
            printf("*                      H O W  T O  P L A Y                    *\n");
            printf("***************************************************************\n");
            printf("*                                                             *\n");
            printf("*   Step 1: Register an Account                               *\n");
            printf("*   Step 2: Log In with Your Account                          *\n");
            printf("*   Step 3: Get Ready to Play and Have Fun!                   *\n");
            printf("*                                                             *\n");
            printf("***************************************************************\n\n");
            printf("You can choose to Log in or register:\n");
            printf("[1] Log In\n");
            printf("[2] Register\n");
            printf("[3] Save & Quit\n");
            printf("=========================================\n");
            printf("Please Enter From the Choices Above: ");
            scanf("%d", &opt);
            printf("=================================\n");
            switch (opt) {
                case 1:
                    system("cls");
                    currentPl = LoginUser(players, numPl, pScores);
                    break;
                case 2:
                    system("cls");
                    Register(players, pScores, &numPl);
                    break;
                case 3:
                    Saveusers(players, numPl);
                    printf("Progress saved. Thankyou for playing Josh and Dennis Arithmetic Game!\n");
                    printf("Saving in Progress, Please Wait...\n");
				    loading();
                    return 0;
                default:
                    system("cls");
                    printf("Invalid Input.\n");
                    system("pause");
                    break;
            }
        } else {
            currentPl = Menu(pScores, currentPl);
        }
    }
}

int Menu(PScore pScores[], int currentPl) {
    int avescore = (pScores[currentPl].addscore + pScores[currentPl].subscore + pScores[currentPl].mulscore + pScores[currentPl].divscore) / 4;
    int menu;

    system("cls");
    printf("Welcome, %s! Let the games begin! \n", pScores[currentPl].username);
    printf("\n=================================\n");
    printf("\n|    P L A Y  T H E  G A M E    |\n ");
    printf("\n=================================\n\n");
    printf("Your Total Average Score: %d\n\n", avescore);
    printf("Choose Arithmetic Game Mode:\n");
    printf("[1] Addition Game Mode         =>> Your Current Score: (%d)\n", pScores[currentPl].addscore);
    printf("[2] Subtraction Game Mode      =>> Your Current Score: (%d)\n", pScores[currentPl].subscore);
    printf("[3] Multiplication Game Mode   =>> Your Current Score: (%d)\n", pScores[currentPl].mulscore);
    printf("[4] Division Game Mode         =>> Your Current Score: (%d)\n", pScores[currentPl].divscore);
    printf("[5] Leaderboards\n");
    printf("[6] Log Out\n");
    printf("=================================\n");
    printf("Input [1-6]: ");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
            system("cls");
            Addgame(&pScores[currentPl]);
            break;
        case 2:
            system("cls");
            Subgame(&pScores[currentPl]);
            break;
        case 3:
            system("cls");
            Mulgame(&pScores[currentPl]);
            break;
        case 4:
            system("cls");
            Divgame(&pScores[currentPl]);
            break;
        case 5:
            system("cls");
            leadmenu(pScores, currentPl);
            break;
        case 6:
            Saveplayerscores(pScores, MAX_PLAYERS);
            printf("Saving Progress...\nGoing Back to Menu\n");
            printf("This Program Will Exit, Please Wait...\n");
            loading();
            currentPl = -1;
            break;
        default:
            printf("Invalid choice.\n");
            system("pause");
            break;
    }
    return currentPl;
}

int leadmenu(PScore pScores[], int currentPl) {
    int lead;
    system("cls");
    printf("Choose a Rankings:\n");
    printf("[1] Addition Rankings\n");
    printf("[2] Subtraction Rankings\n");
    printf("[3] Multiplication Rankings\n");
    printf("[4] Division Rankings\n");
    printf("[5] Top Rankings\n");
    printf("[6] Back to Menu\n");
    printf("Input [1-6]: ");
    scanf("%d", &lead);
    switch(lead) {
        case 1:
            system("cls");
            Addlead(&pScores[currentPl]);
            break;
        case 2:
            system("cls");
            Sublead(&pScores[currentPl]);
            break;
        case 3:
            system("cls");
            Mullead(&pScores[currentPl]);
            break;
        case 4:
            system("cls");
            Divlead(&pScores[currentPl]);
            break;
        case 5:
            system("cls");
            Ranklead(pScores);
            break;
        case 6:
            system("cls");
            printf("\nSaving Progress...Going Back to Menu.\n");
            printf("\nThis Program Will Exit, Please Wait...\n");
            loading();
            break;
        default:
            printf("Invalid Input.\n");
            break;
    }
    return lead;
}
