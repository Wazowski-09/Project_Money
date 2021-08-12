#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 1000

typedef struct
{ // หนังสือ
    char name[100];
    char author[100];
    char isbn[20];
    int amount;
} Book;

typedef struct
{ // นิสิต
    char name[10];
    char id[10];
    char faculty[20];
} Student;
typedef struct
{ // user
    char id[10];
} ID;

typedef struct
{ //password
    char pwd[10];
} PWD;
typedef struct
{ // รายละเอียดหนังสือ
    char detail[1000];
} Detail;

typedef struct
{ // ประวัติการยืม
    char id[10];
    char isbn[20];
    int day; // ยืมมากี่วัน
} Record;

void Borrow();
void Return();
void CheckBook();
void CheckStudent();
void save();
int GetISBN(char *);
void displayLogin();
void loginorcreate();
void login();
void create();
void allbook();

char userName[20]; /* The user name */
char userPWD[20];  /* password */
char Numb[3];
int i, sum;
int answeruser;
Book books[MAX_SIZE];
Student students[MAX_SIZE];
Record records[MAX_SIZE];
Detail detail[MAX_SIZE];
ID id[MAX_SIZE];
PWD pwd[MAX_SIZE];
int ind = 0;
int sizebook = 0;
int c = 0;

int main()
{
    displayLogin();
    //printf("\n1\n");
    FILE *fbook, *fstudent, *frecord, *fdetail;
    char line[1000], *token;
    //printf("\n2\n");
    fbook = fopen("book.txt", "r+");
    fstudent = fopen("student.txt", "r+");
    frecord = fopen("record.txt", "r+");
    fdetail = fopen("detail.txt", "r+");
    if ((fbook == NULL) && (fstudent == NULL) && (frecord == NULL) && (fdetail == NULL))
    { // check files
        printf("Can't open data\n");
        return 0;
    }
    //printf("\n3\n");
    fgets(line, sizeof(line), fbook);
    while (fgets(line, sizeof(line), fbook))
    {
        token = strtok(line, ",");
        strcpy(books[ind].name, token);
        token = strtok(NULL, ",");
        strcpy(books[ind].author, token);
        token = strtok(NULL, ",");
        strcpy(books[ind].isbn, token);
        token = strtok(NULL, ",");
        books[ind].amount = atoi(token);
        ind++;
        sizebook++;
    }
    fclose(fbook);
    //printf("\n4\n");
    ind = 0;
    fgets(line, sizeof(line), fstudent);
    while (fgets(line, sizeof(line), fstudent))
    {
        token = strtok(line, ",");
        strcpy(students[ind].name, token);
        token = strtok(NULL, ",");
        strcpy(students[ind].id, token);
        token = strtok(NULL, ",");
        strcpy(students[ind].faculty, token);
        ind++;
    }
    fclose(fstudent);

    ind = 0;
    fgets(line, sizeof(line), frecord);
    while (fgets(line, sizeof(line), frecord))
    {
        token = strtok(line, ",");
        strcpy(records[ind].id, token);
        token = strtok(NULL, ",");
        strcpy(records[ind].isbn, token);
        token = strtok(NULL, ",");
        records[ind].day = atoi(token);
        ind++;
    }
    fclose(frecord);

    ind = 0;
    fgets(line, sizeof(line), fdetail);
    while (fgets(line, sizeof(line), fdetail))
    {
        token = strtok(line, "/");
        strcpy(detail[ind].detail, token);
        ind++;
    }
    fclose(fdetail);

    loginorcreate();
    while (c == 1)
    {
        int n;
        //system("clear");
        printf("\n\n1 : Borrow\n2 : Return\n3 : Check Book\n4 : Check Student\n5 : View all book\n0 : Quit\nEnter a number : ");
        scanf("%d", &n);
        if (n == 0)
        {
            c = 0;
            break;
        }
        else if (n == 1)
        {
            Borrow();
        }
        else if (n == 2)
        {
            Return();
        }
        else if (n == 3)
        {
            CheckBook();
        }
        else if (n == 4)
        {
            CheckStudent();
        }
        else if (n == 5)
        {
            allbook();
        }
        else
        {
            printf("Enter the correct number.\n-------------------\n");
            continue;
        }
    }

    save();

    return 0;
}

void Borrow()
{
    char isbn[20], id[10];
    printf("Enter Student ID : ");
    scanf("\n%[^\n]", id);
    printf("Enter Book ISBN : ");
    scanf("\n%[^\n]", isbn);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (strcmp(isbn, books[i].isbn) == 0)
        {
            if (books[i].amount <= 0)
            {
                printf("There are no books left\n-------------------\n");
                return;
            }
            else
            {
                printf("Borrowed [%s]\n-------------------\n", books[i].name);
                strcpy(records[ind].id, id);
                strcpy(records[ind].isbn, isbn);
                records[ind].day = 7;
                ind++;
                books[i].amount--;
                return;
            }
        }
    }
    printf("Can't find book\n-------------------\n");
}

void Return()
{
    char isbn[20], id[10];
    printf("Enter Student ID : ");
    scanf("\n%[^\n]", id);
    printf("Enter Book ISBN : ");
    scanf("\n%[^\n]", isbn);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if ((strcmp(id, records[i].id) == 0) || (strcmp(isbn, records[i].isbn) == 0))
        {
            int j = GetISBN(isbn);
            if (records[i].day <= 0)
            {
                printf("Returned Late [%s]\n-------------------\n", books[j].name);
            }
            else
            {
                printf("Returned [%s]\n-------------------\n", books[j].name);
            }
            strcpy(records[i].id, "\0");
            strcpy(records[i].isbn, "\0");
            books[j].amount++;
            return;
        }
    }
    printf("No records found\n-------------------\n");
}

int GetISBN(char *isbn)
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (strcmp(isbn, books[i].isbn) == 0)
        {
            return i;
        }
    }
    return 0;
}

void CheckBook()
{
    char isbn[20];
    int count = 0;
    printf("Enter Book ISBN : ");
    scanf("\n%[^\n]", isbn);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (strcmp(records[i].isbn, isbn) == 0)
        {
            printf("ID : %s\nISBN : %s\nDays left : %d\n-------------------\n", records[i].id, records[i].isbn, records[i].day);
            count++;
        }
    }
    if (!count)
    {
        printf("Can't find record\n-------------------\n");
    }
}

void CheckStudent()
{
    char id[10];
    int count = 0;
    printf("Enter ID : ");
    scanf("\n%[^\n]", id);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (strcmp(records[i].id, id) == 0)
        {
            printf("ID : %s\nISBN : %s\nDays left : %d\n-------------------\n", records[i].id, records[i].isbn, records[i].day);
            count++;
        }
    }
    if (!count)
    {
        printf("Can't find record\n-------------------\n");
    }
}

void save()
{
    char line[1000];
    int i = 0;
    FILE *f = fopen("book.txt", "w+");
    fprintf(f, "name,author,isbn,amount\n");
    while (i < MAX_SIZE)
    {
        if (strlen(books[i].isbn) == 0)
        {
            i++;
            continue;
        }
        fprintf(f, "%s,%s,%s,%d\n", books[i].name, books[i].author, books[i].isbn, books[i].amount);
        i++;
    }
    fclose(f);

    i = 0;
    f = fopen("student.txt", "w+");
    fprintf(f, "name,id,faculty\n");
    while (i < MAX_SIZE)
    {
        if (strlen(students[i].id) == 0)
        {
            i++;
            continue;
        }
        fprintf(f, "%s,%s,%s", students[i].name, students[i].id, students[i].faculty);
        i++;
    }
    fclose(f);

    i = 0;
    f = fopen("record.txt", "w+");
    fprintf(f, "id,isbn,day\n");
    while (i < MAX_SIZE)
    {
        if (strlen(records[i].id) == 0)
        {
            i++;
            continue;
        }
        fprintf(f, "%s,%s,%d\n", records[i].id, records[i].isbn, records[i].day);
        i++;
    }
    fclose(f);
}
void displayLogin()
{
    // char choiceloginorcreate;
    printf("**********************************\n");
    printf("   *    *\n");
    printf("   *    *\n");
    printf("   *    *\n");
    printf("   *  Welcome to the book information management system  *\n");
    printf("   *    *\n");
    printf("   *    *\n");
    printf("   *    *\n");
    printf("**********************************\n");
    // printf("1: continue\n");
    // printf("2: exit\n");
    // printf("Enter your choice: ");
    //scanf("%c", &choiceloginorcreate);
    // choiceloginorcreate = getchar();
    // switch (choiceloginorcreate)
    // {
    // case '1':
    //     loginorcreate();
    //     break;
    // case '2':
    //     printf("Bye!!\n");
    //     exit(0);
    //     //break;
    // }
}
void loginorcreate()
{

    //system("clear"); //macos
    //system("cls"); //window
    int a;
    printf("Login or Create\n");
    printf("1: Login\n");
    printf("2: Create\n");
    printf("3: Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &a);
    switch (a)
    {
    case 1:
        login();
        break;
    case 2:
        create();
        break;
    case 3:
        printf("Bye!!\n");
        exit(0);
        //break;
    }
}
int checkuser(int size, ID *id, PWD *pwd, char username[], char password[])
{
    //printf("%d\n", size);
    int checkuser;
    int checkpwd;
    int answer;
    char pwdanswer;
    char useranswer;
    for (int j = 0; j < size; j++)
    {
        ID *d = &id[j];
        PWD *p = &pwd[j];

        // printf("d=%s\n", d);
        // printf("p=%s\n\n", p);

        // printf("user=%s\n", username);
        // printf("pass=%s\n\n", password);

        // checkuser = strcmp(username, d);
        // checkpwd = strcmp(password, p);

        checkuser = strcmp(username, id[j].id);
        checkpwd = strcmp(password, pwd[j].pwd);

        // printf("%d\n", checkuser);
        // printf("%d\n\n", checkpwd);

        if (checkuser == 0 && checkpwd == 0)
        {
            answer = 0;
            break;
        }
        else
        {
            answer = -1;
        }
    }
    //printf("%d", answer);
    return answer;
}
int read_user(ID *id, PWD *pwd)
{
    FILE *fid = fopen("id.txt", "r");
    FILE *fpwd = fopen("password.txt", "r");
    if (fid == NULL && fpwd == NULL)
    {
        printf("can,t open");
        exit(0);
        return 0;
    }
    int j = 0;
    while (fscanf(fid, "%s",
                  id[j].id) == 1 &&
           fscanf(fpwd, "%s",
                  pwd[j].pwd) == 1)
    {
        ++j;
    }
    fclose(fid);
    fclose(fpwd);
    return j;
}
void create()
{
    FILE *fpuser, *fppwd;
    char user[10];
    char password[10];
    //char password1[10];
    fpuser = fopen("id.txt", "a");
    fppwd = fopen("password.txt", "a");
    if (fpuser == NULL && fppwd == NULL)
    {
        printf("can not open file");
        exit(0);
    }
    printf("User : ");
    scanf("%s", user);
    printf("Password : ");
    scanf("%s", password);
    // printf("Confirm password : ");
    // scanf("%s", password1);
    // if (password == password1)
    // {
    //     fprintf(fpuser, "\n%-10s", user);
    //     fprintf(fppwd, "\n%-10s", password);
    // }else{
    //     printf("please try again");
    //     exit(0);
    // }
    fprintf(fpuser, "\n%-10s", user);
    fprintf(fppwd, "\n%-10s", password);
    c = 1;
    printf("Create Complete\n");
    printf("**********************************\n");

    fclose(fpuser);
    fclose(fppwd);
    //menu();
}
void login()
{

    //system("color 0B");
    int size = read_user(id, pwd);
    for (i = 1; i < 4; i++)
    {
        /* The username and password are both abcde;*/
        //printf("    ( The username and password are both abcde)\n\n");
        printf("\nPlease enter your user name\t:");
        scanf("%s", userName);
        //printf("user=%s\n", userName);

        printf("\nPlease enter your password\t:");
        scanf("%s", userPWD);
        //printf("pass=%s", userPWD);
        // int checkname = strcmp(userName, "abcde");
        // int checkpwd = strcmp(userPWD, "abcde");
        // printf("%d \n", checkname);
        // printf("%d \n", checkpwd);
        //readfile();
        //answeruser = checkuser(userName,userPWD);

        int answeruser = checkuser(size, id, pwd, userName, userPWD);
        if (answeruser == 0) /* Verify the username and password */
        {
            printf("\n   * The user name and password are correct, and the main menu is displayed *");
            //menu();
            c = 1;
            break;
        }
        else
        {
            if (i < 3)
            {
                printf("User name or password error, prompt user to enter again ");
                printf("The user name or password is wrong. Please enter it again !");
            }
            else
            {
                printf("continuous 3 Enter the wrong user name or password the second time and exit the system. ");
                printf("You have been 3 If the user name or password is incorrectly entered this time, the system will exit !");
                exit(1);
            }
        }
    }
}

void Book_Detail()
{
    //int sizedetail = read_descbook(detail);
    int n;
    int checknumb;
    char x[1];
    sscanf(Numb, "%d", &n); // Using sscanf
                            //printf("\nThe value of x : %d", n);
                            // for (int i = 0; i < sizedetail; ++i)
                            // {
    system("clear");
    Detail *d = &detail[n - 1];
    printf("%d) %s\n\n", n,
           d->detail);
    //}

    // printf("Would you like to rent this book? \n Press \"1\" for rent \n Press \"2\" for Menu\n: ");
    // scanf("%s", &x);
    // checknumb = strcmp(x, "1");
    // //printf("%s", x);
    // //printf("%d", checknumb);
    // if (checknumb == 0)
    // {
    //     //rentbook();
    // }
    // else
    // {
    //     //menu();
    //     exit(0);
    // }
}
void allbook()
{

    //int sizebook = read_book(books);
    char checknumb;
    for (int i = 0; i < sizebook; ++i)
    {
        Book *b = &books[i];
        printf("%2d) name : %-30s author : %-30s isbn : %-10s amount : %-2d\n\n", i + 1,
               b->name,
               b->author,
               b->isbn,
               b->amount);
    }
    printf("Which book do you interest (1-20) Press \"0\" for Menu:  ");
    scanf("%s", Numb);
    checknumb = strcmp(Numb, "0");
    //printf("%s", Numb);
    //printf("%d", checknumb);
    if (checknumb == 0)
    {
        //menu();
        // printf("%s\n",Numb);
        // exit(0);
    }
    else
    {
        //rentbook();
        Book_Detail();
    }
}