#include<stdio.h>
#include<windows.h>
#include<string.h>
//version 1.0

int pil, k;//variabel pil untuk semua user input, variable k untuk stasiun

struct member{//struct untuk status kartu
    char id[7];
    float bal;
};

struct memberx{//struct untuk status kartu di buyc
    int id;
    float bal;
};

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

struct SYSTEMTIME{//struct untuk mendata waktu GMT, hanya digunakan untuk hour dan dayofweek
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

struct station{//deklarasi struct stasiun
    char lane1[3];
    char lane2[3];
    char lane3[3];
    int code1;
    int code2;
    int code3;
    float loc;
    char name[30];
};

int price( struct station target, struct station current, int c){
    /*
    function untuk menghitung harga yang harus dibayar
    */
    //deklarasi variabel
    int x = 1, i, stat, j = 0, y = 0;//deklarasi variabel lokal
    pil = 9;//mengubah isi variabel global
    float distance, price;//deklarasi jarak dan harga
    char id[7];//deklarasi untuk input id
    struct member Member[1000];//deklarasi array struct member
    struct station all[145];//deklarasi array struct stasiun
    FILE *fs = fopen("station.txt", "r");//membuka daftar stasiun
    FILE *fn = fopen("card.txt", "r+");//membuka daftar card
    while(!feof(fn)){//membaca card
        fscanf(fn,"%[^ ] %f\n", &Member[j].id, &Member[j].bal);
        j++;
    }
    while(!feof(fs)){//membaca daftar stasiun
        fscanf(fs,"%[^|]|%d#%[^|]|%d#%[^|]|%d#%[^#]#%f\n", &all[y].lane1,&all[y].code1,&all[y].lane2,&all[y].code2,&all[y].lane3,&all[y].code3,&all[y].name,&all[y].loc);
        if(strcmp(target.lane1,all[y].lane1) == 0 && strcmp(target.name,all[y].name) == 0){
            k = y;
        }
        y++;
    }
    fclose(fs);
    if(c == 1){//jika menggunakan sistem kartu, akan masuk ke fungsi ini
        while(1){
            printf("Type -1 to exit\nID  :");scanf("%s", &id);fflush(stdin);//input ID kartu
            stat = 0;
            rewind(fn);//mengembalikan pointer file ke awal
            i = 0;
            if(strcmp(id, "-1") == 0){
                    pil = '0';
                    system("cls");
                    return;
            }
            while(i <= j){
                if(strcmp(Member[i].id, id) == 0){//jika ID benar dan ada di list
                    stat = 1;
                    break;
                }
                else if(!feof(fn)){//jika ID masih salah
                    i++;
                    continue;
                }
                else if(feof(fn)){//jika ID tidak ada di list atau memang salah input
                    system("cls");
                    stat = 0;i = 0;
                    break;
                }
            }
            if(stat == 1){//jika ID benar, loop akan break
                system("cls");
                break;
            }
            else if(stat == 0){//jika ID salah, loop lanjut
                system("cls");
                printf("ID is incorrect\n");
                continue;
            }
        }
    }
    if(c == 1){
        if(Member[i].bal < 1.50){//jika balance kartu dibawah 1.5 dollar
            printf("Your balance is not enough\n");
            printf("Press any key to go back . . .");
            getch();
            system("cls");
            pil = '0';
            return;
        }
    }
    struct SYSTEMTIME str;//deklarasi waktu
    GetSystemTime(&str);//mengambil waktu
    str.wHour += 8;//menyesuaikan waktu ke singapur
    y = 0;
    if(strcmp(target.lane1,current.lane1) == 0){//untuk menghitung jarak total
        if(target.loc < current.loc){
            distance = current.loc - target.loc;
        }
        else if(target.loc > current.loc){
            distance = target.loc - current.loc;
        }
    }
    else if(strcmp(target.lane1, current.lane1) != 0){//jika stasiun tujuan atau awal berada di 2 jalur berbeda
        while(y < 145){
            if(strcmp(target.lane1,all[y].lane2) == 1 || strcmp(target.lane1,all[y].lane2) == 1 ){
                target.loc = all[y].loc;
                break;
            }
            y++;
        }
        if(target.loc < current.loc){
            distance = current.loc - target.loc;
        }
        else if(target.loc > current.loc){
            distance = target.loc - current.loc;
        }
    }
    //printf("%f", distance);
    if(distance < 0) distance *= -1;//kalau distance negatif, maka akan dimutlakan
    if(c != 1){//jika tidak menggunakan card, perhitungan harga juga beda
        distance = distance - 3.2;
        price = 140;
        x++;//x digunakan menjadi penanda berapa kali jarak yang sudah dikurangi masih diatas 0
        if(distance > 0){
            while(distance > 0){
                if(x == 2 || x == 5 || x == 8 || x == 10 ){//penghitungan harga
                    price += 20;
                }
                else if(x == 14 || x == 18 || x == 22){
                    price +=10;
                }
                distance -= 0.9;
                x++;
            }
        }
    }
    else if(c == 1){//perhitungan harga jika menggunakan card
        distance -= 3.2;
        price = 77;
        x++;
        while(distance > 0){
            distance -= 0.9;
            x++;
            if(x <= 4) price += 10;
            else if(x > 4 && x < 7) price += 9;
            else if(x == 7) price += 6;
            else if(x > 7 && x <= 17) price += 4;
            else if(x > 17 && x <= 21) price += 3;
            else if(x > 21 && x <= 24) price += 2;
            else price += 1;
        }
    }
    if((str.wDayOfWeek == 5 || str.wDayOfWeek == 6) && (str.wHour <= 7 && str.wHour >= 5) && (str.wMinute <= 45) && c == 1){
        //kalau misalkan user menggunakan mrt di hari sabtu atau minggu sebelum jam 7.45 pagi
        //maka ada diskon 50 cent
        price -= 50;
    }
    price /= 100;//harga diubah ke dolar
    if(c == 0){//kalau tidak menggunakan kartu, langsung menampilkan harga total
        printf("Please insert $%.2f", price);
    }
    else if(c != 0){//jika menggunakan kartu
        Member[i].bal -= price;
        printf("Your card has been charge $%.2f\n", price);//menampilkan harga total
        printf("Your current balance is %.2f\n", Member[i].bal);//menampilkan balance card yang lama
        printf("Press any key to continue . . .");
        i = 0;
        rewind(fn);
        while(i < j){
            fprintf(fn,"%s %.2f\n", Member[i].id, Member[i].bal);//update text file
            i++;
        }
    fclose(fn);
    }
    getch();
    system("cls");
    if(c != 1){
        printf("Dispensing single use Ticket, Thank you for using this machine\nPress any key to continue . . .");
        getch();
    }
    system("cls");
    pil = '0';
    return;
}

int EW(struct station temp[145], struct station current, int stat, int c){//function untuk menampilkan/mendata stasiun berdasarkan lane tempat user berada
    if(pil == '0') return;
    int  i = 1,j = 0, k = 1;
    struct station EW[50];//array struct untuk isi dari stasiun dengan yang ada di jalur EW
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"EW") == 0){//jika stasiun ada di jalur EW, maka akan dipindahkan
            EW[k] = temp[j];
            k++;
        }
        j++;
    }
    while(1){
        if(pil == '0' || pil == 0) break;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);//menampilkan stasiun user
        printf("=======================================\n");
        while(i < k){//print stasiun
            printf("[%d]\t%s%2d %s\n", i, EW[i].lane1, EW[i].code1, EW[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);
        if(current.code1 == target.code1 || pil >= 34){//kalau dipilih stasiun yang sama, menu akan di restart
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = EW[i];
        if(pil == 0 || pil == '0'){//jika user memilih untuk keluar
            pil = 9;
            break;
        }
        else if(pil < 34 && pil > 0) price(target, current, c);//jika input valid, akan menhitung jarak dan harga
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int DT(struct station temp[145], struct station current, int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station DT[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"DT") == 0){
            DT[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
            if(pil == '0' || pil == 0) break;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, DT[i].lane1, DT[i].code1, DT[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);
        if(current.code1 == target.code1 || pil >= 36){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = DT[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < 36 && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int NS(struct station temp[145], struct station current, int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station NS[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"NS") == 0){
            NS[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
            if(pil == '0' || pil == 0) break;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, NS[i].lane1, NS[i].code1, NS[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);
        if(current.code1 == target.code1 || pil >= 29){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = NS[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < 29 && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int NE(struct station temp[145], struct station current,int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station NE[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"NE") == 0){
            NE[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
        if(pil == '0' || pil == 0) break;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, NE[i].lane1, NE[i].code1, NE[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);
        if(current.code1 == target.code1 || pil >= 18){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = NE[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < 18 && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int CC(struct station temp[145], struct station current, int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station CC[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"CC") == 0){
            CC[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
        if(pil == '0' || pil == 0) return;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, CC[i].lane1, CC[i].code1, CC[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);
        if(current.code1 == target.code1 || pil >= 30){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = CC[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < 30 && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int CG(struct station temp[145], struct station current, int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station CG[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"CG") == 0){
            CG[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
            if(pil == '0' || pil == 0) return;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, CG[i].lane1, CG[i].code1, CG[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);

        if((current.code1 == target.code1 && current.lane1 == target.lane1) || pil >= k){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = CG[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < k && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

int CE(struct station temp[145], struct station current, int stat, int c){//persis seperti function EW
    if(pil == '0' || pil == 0) return;
    int  i = 1,j = 0, k = 1;
    struct station CE[50];
    struct station target;
    while(j <= 145){
        if(strcmp(temp[j].lane1,"CE") == 0){
            CE[k] = temp[j];
            k++;
        }
        j++;
    }

    while(1){
            if(pil == '0' || pil == 0) return;
        printf("You are currently at station %s|%d %s\n", current.lane1, current.code1, current.name);
        printf("=======================================\n");
        while(i < k){
            printf("[%d]\t%s%2d %s\n", i, CE[i].lane1, CE[i].code1, CE[i].name);
            i++;
        }
        if(stat == 1){
            printf("[0] Return to Lane Choosing Menu\n");
        }
        printf("Option :");scanf("%d", &pil);fflush(stdin);

        if(current.code1 == target.code1 || pil >= k){
                system("cls");
                i = 1;
                continue;
        }
        i = pil;
        system("cls");
        target = CE[i];
        if(pil == 0 || pil == '0'){
            pil = 9;
            break;
        }
        else if(pil < k && pil > 0) price(target, current, c);
        else{
            i = 1;
            continue;
        }
    }
    return;
}

void buyc(){
    /*
    fungsi buyc untuk membuat card baru, karena tidak dapat membuat card reader
    dan card printer yang dapat menggunakan teknologi NFC, maka digantikan dengan
    ID
    */
    pil = 9;
    int  j = 0, i = 0, x, y, stat = 3, type,hias;
    long int id;
    float bal;
    struct memberx Member[1000];
    printf("%c",201);
    for(hias=0;hias<35;hias++){
            printf("%c",205);
    }
    printf("%c",187);
    printf("\n%cPlease enter $10\t\t    %c\n",186,186);//simulasi jumlah minimum
    printf("%c",200);
    for(hias=0;hias<35;hias++){
        printf("%c",205);
    }
    printf("%c",188);
    getch();
    system("cls");
    FILE *fn = fopen("card.txt", "a+");//untuk menambahkan entry baru
    while(!feof(fn)){
        fscanf(fn,"%[^ ] %f\n", &Member[j].id, &Member[j].bal);
        j++;
    }
    while(1){
        x = 0;
        y = 0;
        while(1){//generate id dengan 6 digit
            srand(time(NULL));
            id =(rand() % (999999 + 1 - 100000) + 100000) + (rand() % 10);
            printf("%d", id);
            if(id > 100000 && id < 1000000){//double check untuk memastikan digit id
                break;
            }
            else{
                continue;
            }
        }
        while(y < j){
            if(id == Member[y].id){//kalau id sudah ada, variabel stat(status) berubah jadi 1
                y++;
                stat = 1;
                continue;
            }
            else{//kalau id tidak terdaftar variabel stat berubah jadi 0
                stat = 0;
                break;
            }
        }
        if(stat == 1) continue;//kalau variabel stat 1, maka id baru akan di generate
        else{
            break;
        }
    }
    system("cls");
    bal = 10;
    type = 1;
    fprintf(fn,"%d %.2f\n", id, bal);
    printf("%c",201);
    for(hias=0;hias<35;hias++){
            printf("%c",205);
    }
    printf("%c",187);
    printf("\n%cYour new card ID are %d        %c\n",186,id,186);//menampilkan ID yang baru
    printf("%c",200);
    for(hias=0;hias<35;hias++){
	printf("%c",205);
    }
    printf("%c",188);
    printf("\nPress any key to continue . . .");
    fclose(fn);
    getch();
    system("cls");
    return;
}

void refc(){
    /*
    Function ini digunakan untuk mengisi ulang Card yang ada, jika card tidak ada di daftar maka akan ditolak
    */
    pil = 9;
    int i = 0, stat, ref, j = 0, ch;
    char id[7];
    struct member Member[1000];
    FILE *fn = fopen("card.txt", "r+");
    while(!feof(fn)){
        fscanf(fn,"%[^ ] %f\n", &Member[j].id, &Member[j].bal);
        j++;
    }
    while(1){
        printf("Type -1 to exit\nID  :");scanf("%s", &id);fflush(stdin);
        stat = 0;
        rewind(fn);
        i = 0;
        while(i <= j){//untuk mengecek input
            if(strcmp(id, "-1") == 0){//keluar dari menu
                system("cls");
                return;
            }
            else if(strcmp(Member[i].id, id) == 0){//jika id benar
                stat = 1;
                break;
            }
            else if(!feof(fn)){
                i++;
                continue;
            }
            else if(feof(fn)){//jika id salah
                system("cls");
                stat = 0;i = 0;
                break;
            }
        }
        if(stat == 1){//mengecek apakah id yang dimasukkan benar atau salah
            system("cls");
            break;
        }
        else if(stat == 0){
            system("cls");
            printf("ID is incorrect or currently unused\n");
            continue;
        }
    }
    printf("Your Current Balance is %.2f\n", Member[i].bal);//menampilkan sisa uang di dalam card
    printf("Refill Amount:\n[1]$5\n[2]$10\n[3]$20\n[4]$50\n[5]$100");//pilihan untuk refill
    ref = getch();
    system("cls");
    printf("Please insert money");
    getch();
    system("cls");
    //funsi untuk menambahkan balance
    if(ref == '1'){
        Member[i].bal += 5;
    }
    else if(ref == '2'){
        Member[i].bal += 10;
    }
    else if(ref == '3'){
        Member[i].bal += 20;
    }
    else if(ref == '4'){
        Member[i].bal += 50;
    }
    else if(ref == '5'){
        Member[i].bal += 100;
    }
    printf("Card Refilled\nYour Current Balance is %.2f", Member[i].bal);//menampilkan balance yang baru
    i = 0;
    rewind(fn);
    while(i < j){//memasukkan data yang baru kedalam file text
        fprintf(fn,"%s %.2f\n", Member[i].id, Member[i].bal);
        i++;
    }
    fclose(fn);
    getch();
    system("cls");
    return;
}

void map(){
    int hias;
    /*
    function untuk membuka map MRT
    */
    pil = 9;
    printf("\nOpening Map... Please Wait");
    Sleep(500);
    ShellExecute(NULL, "open", "file:///C:/Users/giratina1231v2.0/Documents/Kuliah/Test%20code/Final%20sem2/MRTmap.jpg", NULL, NULL, SW_SHOWNORMAL);
    //ShellExecute(NULL, "open", "https://www.lta.gov.sg/content/dam/ltaweb/corp/PublicTransport/files/00_System%20Map%20%20with%20NELe-new%20version-Nov17.jpg", NULL, NULL, SW_SHOWNORMAL);
    system("cls");
    printf("%c",201);
    for(hias=0;hias<35;hias++){
            printf("%c",205);
    }
    printf("%c\n",187);
    printf("%c\t\tMap\t\t    %c\n",186,186);
    printf("%c",204);
    for(hias=0;hias<35;hias++){
            printf("%c",205);
    }
    printf("%c",185);
    printf("\n%c[1] Buy Card\t\t\t    %c",186,186);
    printf("\n%c[2] Buy Ticket\t\t\t    %c",186,186);
    printf("\n%c[0] Return to main menu\t    %c",186,186);
    printf("\n%c",200);
    for(hias=0;hias<35;hias++){
            printf("%c",205);
    }
    printf("%c",188);
    pil = getch();
    if(pil == '1'){
        pil = '2';
    }
    else if(pil == '2'){
        pil = '3';
    }
    system("cls");
    return;
}

void card(){
    int hias;
    /*
    function card menampilkan pilihan yang dapat dipilih berhubungan dengan card
    */
    pil = 9;
    while(1){
        printf("%c",201);
        for(hias=0;hias<35;hias++){
                printf("%c",205);
        }
        printf("%c\n",187);
        printf("%c Buy or Refill Card\t\t    %c\n",186,186);
        printf("%c",204);
        for(hias=0;hias<35;hias++){
            printf("%c",205);
        }
        printf("%c",185);
        printf("\n%c[1] Buy Card\t\t\t    %c",186,186);
        printf("\n%c[2] Refill Card\t\t    %c",186,186);
        printf("\n%c[0] Return to Main Menu\t    %c",186,186);
        printf("\n%c",200);
        for(hias=0;hias<35;hias++){
            printf("%c",205);
        }
        printf("%c",188);
        pil = getch();
        system("cls");
        if(pil == '1'){
            buyc();
            pil = '2';
            break;
        }
        else if(pil == '2'){
            refc();
            pil = '2';
            break;
        }
        else if(pil == '0'){
            break;
        }
        else continue;
    }
    return;
}

int ticket(int c){
    /*allall
    function untuk membeli single use ticket, mirip ddengan function yang digunakan untuk simulasi kartu
    */
    if(pil == '0' || pil == 0) return;
    int i = 0, j, stat, ce = 0, cc = 0, ew = 0, ns = 0, ne = 0, cg = 0, dt = 0;//variable setelah stat digunakan sebagai error handling
    pil = 9;
    struct station all[200];
    struct station current;
    FILE *fn = fopen("station.txt","r");
    while(!feof(fn)){//membaca isi file yang ada data stasiunnya
        fscanf(fn,"%[^|]|%d#%[^|]|%d#%[^|]|%d#%[^#]#%f\n", &all[i].lane1,&all[i].code1,&all[i].lane2,&all[i].code2,&all[i].lane3,&all[i].code3,&all[i].name,&all[i].loc);
        i++;
    }
    fclose(fn);
    current = all[k];
    while(1){//tampilan menu
        if(pil == '0' || pil == 0) break;
    printf("You are currently at station %s|%d %s\n", all[k].lane1, all[k].code1, all[k].name);//print lokasi stasiun sekarang
    printf("=======================================\n");
    printf("Choose Alighting Station :\n");
    if(strcmp(current.lane2, "x") != 0){//jika stasiun ada di lebih dari 1 jalur, bagian ini akan jalan untuk print pilihan jalur
        stat = 1;
        if(strcmp(current.lane1,"EW") == 0 || strcmp(current.lane2,"EW") == 0 || strcmp(current.lane3,"EW") == 0){
            printf("[1] East West Line\n");
            ew = 1;
        }
        if(strcmp(current.lane1,"NS") == 0 || strcmp(current.lane2,"NS") == 0 || strcmp(current.lane3,"NS") == 0){
            printf("[2] North South Line\n");
            ns = 1;
        }
        if(strcmp(current.lane1,"NE") == 0 || strcmp(current.lane2,"NE") == 0 || strcmp(current.lane3,"NE") == 0){
            printf("[3] North East Line\n");
            ne = 1;
        }
        if(strcmp(current.lane1,"CC") == 0 || strcmp(current.lane2,"CC") == 0 || strcmp(current.lane3,"CC") == 0){
            printf("[4] Circle Line\n");
            cc = 1;
        }
        if(strcmp(current.lane1,"DT") == 0 || strcmp(current.lane2,"DT") == 0 || strcmp(current.lane3,"DT") == 0){
            printf("[5] Downtown Line\n");
            dt = 1;
        }
        if(strcmp(current.lane1,"CG") == 0 || strcmp(current.lane2,"CG") == 0 || strcmp(current.lane3,"CG") == 0){
            printf("[6] Changi Line\n");
            cg = 1;
        }
        if(strcmp(current.lane1,"CE") == 0 || strcmp(current.lane2,"CE") == 0 || strcmp(current.lane3,"CE") == 0){
            printf("[7] CE Line\n");
            ce = 1;
        }
        printf("[0]Return to Menu");
    pil = getch();
    }
    system("cls");//setelah input, user akan dimasukkan ke function tertentu untuk jalur tertentu
    if(pil == '1' && ew == 1) EW(all, current, stat, c);
    else if(pil == '2' && ns == 1) NS(all, current, stat, c);
    else if(pil == '3' && ne == 1) NE(all, current, stat, c);
    else if(pil == '4' && cc == 1) CC(all, current, stat, c);
    else if(pil == '5' && dt == 1) DT(all, current, stat, c);
    else if(pil == '6' && cg == 1) CG(all, current, stat, c);
    else if(pil == '7' && ce == 1) CE(all, current, stat, c);
    //else if dibawah ini jalan jika stasiun hanya memiliki satu jalur
    else if(strcmp(current.lane1, "EW") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) EW(all, current, stat, c);
    else if(strcmp(current.lane1, "NS") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) NS(all, current, stat, c);
    else if(strcmp(current.lane1, "NE") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) NE(all, current, stat, c);
    else if(strcmp(current.lane1, "CC") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) CC(all, current, stat, c);
    else if(strcmp(current.lane1, "DT") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) DT(all, current, stat, c);
    else if(strcmp(current.lane1, "CG") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) CG(all, current, stat, c);
    else if(strcmp(current.lane1, "CG") == 0 && strcmp(current.lane2,"x") == 0  && strcmp(current.lane3,"x") == 0) CE(all, current, stat, c);
    else if(pil == '0') break;
    else if(pil != 0) continue;
    }
    return;
}

void LA(struct station all[145]){
    /*
    function LA digunakan untuk mengurutkan stasiun berdasarkan jalur
    LA berarti Line Ascending, diurutkan berdasarkan alphabet jalur
    */
    int i, j, s;
    struct station temp;
    for(i = 0;i < 145 * 145;i++){//bubble sort
        s = 1;
        for(j = 0;j <= 140;j++){
            if(strcmp(all[j].lane1,all[j + 1].lane1) > 0){
                temp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = temp;
                s = 0;
            }
        }
        if(s == 1) break;
    }
    i = 1;
    while(i <= 140){//bubble sort
        printf("%s%d %s\n", all[i].lane1, all[i].code1, all[i].name);
        i++;
    }
    return;
}
void LD(struct station all[145]){
    /*
    function LD digunakan untuk mengurutkan stasiun berdasarkan jalur
    LD berarti Line Descending, diurutkan berdasarkan alphabet jalur
    */
    int i = 0, j, s;
    struct station temp;
    for(;i < 145 * 145;i++){//bubble sort
        s = 1;
        for(j = 0;j < 145;j++){
            if(strcmp(all[j].lane1,all[j + 1].lane1) < 0){
                temp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = temp;
                s = 0;
            }
        }
        if(s == 1) break;
    }
    i = 0;
    while(i < 141){//print hasil sort
        printf("%s%d %s\n", all[i].lane1, all[i].code1, all[i].name);
        i++;
    }
    return;
}
void SA(struct station all[145]){
    /*
    function SA ada untuk mengurutkan stasiun berdasarkan nama
    SA akan mengurutkan nama stasiun dari A ke Z
    */
    int i = 0, j, s;
    struct station temp;
    for(;i < 10000;i++){//Bubble Sort
        s = 1;
        for(j = 0;j < 145;j++){
            if(strcmp(all[j].name,all[j + 1].name) > 0){
                temp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = temp;
                s = 0;
            }
        }
        if(s == 1) break;
    }
    i = 5;
    while(i <= 145){ //print hasil sort
        printf("%s%d %s\n", all[i].lane1, all[i].code1, all[i].name);
        i++;
    }
    return;
}
void SD(struct station all[145]){
    /*
    function SD merupakan function untuk mengurutkan stasiun berdasarkan nama
    kebalikan dari SA, SD mengurutkan descending dari Z ke A
    */
    int i = 0, j, s;
    struct station temp;
    for(;i < 10000;i++){//Bubble Sort
        s = 1;
        for(j = 0;j < 145;j++){
            if(strcmp(all[j].name,all[j + 1].name) < 0){
                temp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = temp;
                s = 0;
            }
        }
        if(s == 1) break;
    }
    i = 0;
    while(i < 141){ //print hasil sort
        printf("%s%d %s\n", all[i].lane1, all[i].code1, all[i].name);
        i++;
    }
    return;
}

void sort(){
    /*
    function sort untuk menampilkan semua stasiun dan mengurutkannya berdasarkan kriteria yang ada
    */
    pil = 9;
    int i = 0, j, x, y;
    struct station all[200];
    FILE *fn = fopen("station.txt", "r");
    while(!feof(fn)){//untuk membaca file yang menyimpan data stasiun dan menampilkan keseluruhan stasiun
        fscanf(fn,"%[^|]|%d#%[^|]|%d#%[^|]|%d#%[^#]#%f\n", &all[i].lane1,&all[i].code1,&all[i].lane2,&all[i].code2,&all[i].lane3,&all[i].code3,&all[i].name,&all[i].loc);
        printf("%s%d %s\n", all[i].lane1, all[i].code1, all[i].name);
        i++;
    }
    fclose(fn);
    while(1){//tampilan menu di function sort
        printf("===============================\n");
        printf("Sort by:\n");
        printf("[1] Line: Ascending\n");
        printf("[2] Line: Descending\n");
        printf("[3] Station Name: A-Z\n");
        printf("[4] Station Name: Z-A\n");
        printf("[0] Back to main menu");
        while(1){//error handling jika user memasukkan input yang tidak ada pilihannya
            pil = getch();
            if(pil < '0' || pil > '4') continue;
            else break;
        }
        system("cls");
        //jika input benar, akan memanggil function yang mengurutkan stasiun
        if(pil == '1') LA(all);
        else if(pil == '2') LD(all);
        else if(pil == '3') SA(all);
        else if(pil == '4') SD(all);
        else if(pil == '0') break;
        else continue;
    }
    return;
}

void cross(){
    /*
    merupaka ekstensi dari function main, dibuat function sendiri agar lebih mudah diatur
    */
    int c;
    if(pil == '0') return;
    while(1){
        if(pil == '1'){
            map();
        }
        else if(pil == '2'){
            card();
        }
        else if(pil == '3'){
            c = 0;
            ticket(c);
        }
        else if(pil == '4'){
            c = 1;
            ticket(c);
        }
        else if(pil == '5'){
            sort();
        }
        else if(pil == '0'){
            pil = 9;
            return;
        }
        else continue;
    }
}

int main(){//function ini berguna untuk menampilkan menu utama dan mengarahkan user ke menu lainnya
    srand(time(NULL));//anchor untuk stasiun
    fflush(stdin);
    k = rand() % 141;//secara random memilih stasiun
    hidecursor();
    int hias;
    while(1){
        //tampilan menu utama
        printf("%c",201);
        for(hias=0;hias<35;hias++){
            printf("%c",205);
        }
        printf("%c",187);
        printf("\n%c  Welcome to Singapore MRT Locket  %c\n",186,186);
        printf("%c",204);
        for(hias=0;hias<35;hias++){
            printf("%c",205);
        }
        printf("%c",185);
        printf("\n%c[1] Show Map                       %c",186,186);
        printf("\n%c[2] Buy and Refill Card            %c",186,186);
        printf("\n%c[3] Buy Single Use Ticket          %c",186,186);
        printf("\n%c[4] Card's Fare Simulation         %c",186,186);
        printf("\n%c[5] Show all station               %c",186,186);
        printf("\n%c[0] Exit                           %c\n",186,186);
        printf("%c",200);
        for(hias=0;hias<35;hias++){
            printf("%c",205);
        }
        printf("%c",188);
        pil = getch();
        fflush(stdin);
        system("cls");
        if(pil < '0' || pil > '5') continue;//jika input ada di dalam range yang  diterima, akan memanggil function cross
        cross();
        if(pil == '0') break;
    }
    system("cls");
    printf("Thank You for using this machine\n");
    getch();
    return 0;
}
