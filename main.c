#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#define MAX_SIZE 10
#define DEFAULT_BUFLEN 512
void logposielanie(FILE* file, char* posielatel, char* sprava){
    fprintf(file, "%s: %s\n", posielatel, sprava);
}
int prvocislo(int cislo){
    if (cislo <= 1) {
        printf("cislo nie je prvocislo");
        return 0;
    }
    for (int i = 2; i * i <= cislo; i++){
        if (cislo % i == 0)
            return 0;
    }
    return 1;
}
char* vytahPrvocisla(char* encr_message){
    int length = strlen(encr_message);//длинна зашифрованого сообшения
    char* extr_message = (char*)malloc(length);//выделяет память лоя строки будет содержа
    if (extr_message == NULL) {//Проверка на успешное выделение памяти. Если память не была выделена, функция выводит сообщение об ошибке и возвращает NULL.
        printf("Chyba alokace pameti");
        return NULL;
    }
int count = 0;//Инициализация переменной count, которая будет использоваться для отслеживания количества символов в строке extr_message.
    for (int i = 2; i < length; i++){
        if (prvocislo(i)){
            extr_message[count] = encr_message[i - 1];// из зашифрованного сообщения с индексом i - 1 (так как индексация начинается с 0) добавляется в строку extr_message на позицию count
            count+=1;//Увеличение счетчика count для следующего символа в строке
        }
    }
    extr_message[count] = '\0';//завершить строку
    return extr_message;
}
char* desifrovka(char* sifrovana_sprava){
    int lengthM = 148;
    char* desifrovana_sprava = (char*)malloc(lengthM +1);
    for (int i = 0; i < lengthM; i++)
    {
        char sifrovany_znak = sifrovana_sprava[i];
        char desifrovany_znak = sifrovany_znak^ 55;
        desifrovana_sprava[i] = desifrovany_znak;
    }
    desifrovana_sprava[lengthM] = '\0';
    return desifrovana_sprava;
}

int compute_remainder(int student_id) {
    char student_id_str[10];
    sprintf(student_id_str, "%d", student_id);
    int sum_first_five_digits = 0;
    for (int i = 0; i < 5; i++) {
        sum_first_five_digits += student_id_str[i] - '0';
    }
    int fifth_digit = student_id_str[4] - '0';
    if (fifth_digit == 0) {
        fifth_digit = 9;
    }
    int remainder = sum_first_five_digits % fifth_digit;
    return remainder;
}
void right(HANDLE hConsole, char* pisanie_spravy){
    COORD cursorPos;//хранит текущую позицию курсора
    CONSOLE_SCREEN_BUFFER_INFO c_info;//хранения информации о буфере экрана консоли
    GetConsoleScreenBufferInfo(hConsole, &c_info);//Вызов функции
    int c_width = c_info.dwSize.X;//Получение ширины консоли из структуры
    int textLen = strlen(pisanie_spravy);//Определение длины строки
    cursorPos.Y = c_info.dwCursorPosition.Y;//становка вертикальной позиции курсора на текущую строку консоли.
    cursorPos.X = c_width ;
    SetConsoleCursorPosition(hConsole, cursorPos);
    for (int i=0; i< textLen; i++){
        if (cursorPos.X == c_width )
        {
            cursorPos.X = c_width / 3;
            cursorPos.Y+=2;
            SetConsoleCursorPosition(hConsole, cursorPos);
        }
        printf("%c", pisanie_spravy[i]);
        cursorPos.X+=1;//Увеличение  позиции курсора на 1 символ для перехода к следующему символу в строке
    }
    cursorPos.Y+=1;
    cursorPos.X = 0;//начало строки
    SetConsoleCursorPosition(hConsole, cursorPos);
}


int main (){
        FILE* file = fopen("correspondence.txt", "w");
        HANDLE hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        char*my_id = "126890";

        char* answer1[MAX_SIZE] = {"Hi",my_id, "WHat"};
        char* answer2[MAX_SIZE] = {"333222334","123"};
        char* answer3[MAX_SIZE] = {"27","86","M.E.","PRIMENUMBER"};
        char* answer4[MAX_SIZE] = {"Trinity","DQPSK","16"};
        int index1 = 0;
        int index2 = 0;
        int index3 = 0;
        int index4 = 0;

        WSADATA wsaData;    //struktura WSADATA pre pracu s Winsock
        int iResult;

        char sendbuf[DEFAULT_BUFLEN];
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);     //zakladna inicializacia
        if (iResult != 0)     //kontrola, ci nestala chyba
        {
            printf("WSAStartup failed : %d\n", iResult);
            return 1;

        }

        struct addrinfo *result = NULL, *ptr = NULL;     //struktura pre pracu s adresami
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;     //pracujeme s protokolom TCP/IP

        // Resolve the server address and port
        iResult = getaddrinfo("147.175.115.34", "777", &hints, &result);
        if (iResult != 0)     //kontrola, ci nenastala chyba
        {
            printf("getaddrinfo failed : %d\n", iResult);
            WSACleanup();
            return 1;

        }
        else
            printf("getaddrinfo didn’t fail…\n");
        SOCKET ConnectSocket = INVALID_SOCKET;

        // Attempt to connect to the first address returned by
        // the call to getaddrinfo
        ptr = result;

        // Create a SOCKET for connecting to server => pokus o vytvorenie socketu
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET)     //kontrola, ci nenastala chyba
        {
            printf("Error at socket() : %d\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }
        else
            printf("Error at socket DIDN’T occur…\n");

        // Connect to server. => pokus o pripojenie sa na server

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)     //kontrola, ci nenastala chyba
            printf("Not connected to server…\n");
        else
            printf("Connected to server!\n");

        if (iResult == SOCKET_ERROR)    //osetrenie chyboveho stavu
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            WSACleanup();
            return 1;
        }

        Sleep(250);
        while (index1 < MAX_SIZE) {
            // Send data
            sprintf(sendbuf, "%s", answer1[index1]);
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            printf("Sent: %s\n", sendbuf);
            logposielanie(file,"My message",sendbuf);

            // Receive until the peer closes the connection
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                recvbuf[iResult] = '\0';
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("Received: ");
                right(hConsole, recvbuf);
                logposielanie(file,"Your message",recvbuf);
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed: %d\n", WSAGetLastError());

            // Change the value of ansver
            index1 = (index1 + 1) % MAX_SIZE;
        }

        sprintf(sendbuf,"%d",844848);
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
             if (iResult == SOCKET_ERROR){
             printf("send failed : %d\n", WSAGetLastError());
             closesocket(ConnectSocket);
             WSACleanup();
             return 1;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("code: %s\n", sendbuf);
        logposielanie(file,"My message",sendbuf);
         //posielanie
        const char *my_name = "Mariia";
        for (int i = 0; my_name[i] != '\0'; ++i) {
            char sendbuf[2] = { my_name[i], '\0' };//сделаем массив на два елемента где один будет буква а второй конечный елемент
            iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            printf("%c\n", my_name[i]);
            Sleep(125);
            logposielanie(file,"My message",sendbuf);
        }
        //prijimanie
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);     //funkcia na príjimanie

        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("Message : ");
            right(hConsole, recvbuf);
            logposielanie(file,"Your message",recvbuf);
           //printf("Bytes received : %d\n", iResult);     //prisli validne data, vypis poctu
        }
        else if (iResult == 0)
            printf("Connection closed\n");     //v tomto pripade server ukoncil komunikaciu
        else
            printf("recv failed with error : %d\n", WSAGetLastError());     //ina chyba
        //posielanie
        sprintf(sendbuf,"%d",753422);
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed : %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("code: %s\n", sendbuf);
        logposielanie(file,"My message",sendbuf);
        //prijimanie
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);     //funkcia na príjimanie

        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("Message : ");
            right(hConsole, recvbuf);
            logposielanie(file,"Your message",recvbuf);
            //printf("Bytes received : %d\n", iResult);     //prisli validne data, vypis poctu
        }
        else if (iResult == 0)
            printf("Connection closed\n");     //v tomto pripade server ukoncil komunikaciu
        else
            printf("recv failed with error : %d\n", WSAGetLastError());     //ina chyba

    //posielanie
        int student_id = 126890;
        int remainder_result = compute_remainder(student_id);
        sprintf(sendbuf,"%d",remainder_result);

        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed : %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("Remainder of the division: %s\n", sendbuf);
        logposielanie(file,"My message",sendbuf);

        //prijimanie
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);     //funkcia na príjimanie

        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("Message : ");
            right(hConsole, recvbuf);
            logposielanie(file,"Your message",recvbuf);
            //printf("Bytes received : %d\n", iResult);     //prisli validne data, vypis poctu
        }
        else if (iResult == 0)
            printf("Connection closed\n");     //v tomto pripade server ukoncil komunikaciu
        else
            printf("recv failed with error : %d\n", WSAGetLastError());     //ina chyba
        while (index2 < MAX_SIZE) {
            // Send data
            sprintf(sendbuf, "%s", answer2[index2]);
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            printf("Sent: %s\n", sendbuf);
            logposielanie(file,"My message",sendbuf);

            // Receive until the peer closes the connection
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                recvbuf[iResult] = '\0';
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("Received: ");
                right(hConsole, recvbuf);
                logposielanie(file,"Your message",recvbuf);
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed: %d\n", WSAGetLastError());

            // Change the value of ansver
            index2 = (index2 + 1) % MAX_SIZE;
        }
        // posielanie
        sprintf(sendbuf,  "%s", desifrovka(recvbuf));
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("deciphered: %s\n", sendbuf);
        logposielanie(file,"My message",sendbuf);


        //prijimanie

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0){
            recvbuf[iResult] = '\0';
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("Message : ");
            right(hConsole, recvbuf);
            logposielanie(file,"Your message",recvbuf);
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

        while (index3 < MAX_SIZE) {
            // Send data
            sprintf(sendbuf, "%s", answer3[index3]);
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            printf("Sent: %s\n", sendbuf);
            logposielanie(file,"My message",sendbuf);

            // Receive until the peer closes the connection
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                recvbuf[iResult] = '\0';
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("Received: ");
                right(hConsole, recvbuf);
                logposielanie(file,"Your message",recvbuf);
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed: %d\n", WSAGetLastError());

            // Change the value of ansver
            index3 = (index3 + 1) % MAX_SIZE;
        }
        //posielanie
        sprintf(sendbuf, vytahPrvocisla(recvbuf));
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed : %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf("Code: %s\n", sendbuf);
        logposielanie(file,"My message",sendbuf);

        //prijimanie
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);     //funkcia na príjimanie

        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            printf("Message : ");
            right(hConsole, recvbuf);
            logposielanie(file,"Your message",recvbuf);
            //printf("Bytes received : %d\n", iResult);     //prisli validne data, vypis poctu
        }
        else if (iResult == 0)
            printf("Connection closed\n");     //v tomto pripade server ukoncil komunikaciu
        else
            printf("recv failed with error : %d\n", WSAGetLastError());     //ina chyba
        while (index4 < MAX_SIZE) {
            // Send data
            sprintf(sendbuf, "%s", answer4[index4]);
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            printf("Sent: %s\n", sendbuf);
            logposielanie(file,"My message",sendbuf);

            // Receive until the peer closes the connection
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                recvbuf[iResult] = '\0';
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("Received: ");
                right(hConsole, recvbuf);
                logposielanie(file,"Your message",recvbuf);
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed: %d\n", WSAGetLastError());

            // Change the value of ansver
            index4 = (index4 + 1) % MAX_SIZE;
        }

        fclose(file);
        closesocket(ConnectSocket);
        WSACleanup();
    }