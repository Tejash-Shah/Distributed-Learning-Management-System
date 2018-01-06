#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <pthread.h>
#include <string.h>
#include <stdlib.h> 




#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <string>
#include <pthread.h>
#include <cstring>
//#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#define server_port 58055
#define Sender_Buff 20000
#define Receive_Buff 20000
#define rec_buff 5000


int accept_value;
FILE *fp,*fp1;
sqlite3 *db;
char *zErrMsg = 0;
int row;
int rc;
char *sql;
int x;
int r,f,c;
char file_integer[2048];
static int asscogra(void *);
static int assignment(void *);
static int courses(void *);
static int grades(void *);
static int options(void *);
static int file_name(void *);
void send_grades_to_client(int, char*);
void open_file_from_database(char []);
int read_file_from_database();
void read_assignments_from_database();
void open_assignments_from_database(char[]);
int read_grades_from_database();
int file_open_checker();
int tf(int, char*);
void open_grades_from_database(char []);
void send_assignment_to_client();
int transfer_assignmentfile_to_client(int, char*);
int transfer_coursefile_to_client(int, char*);
int transfer_gradefile_to_client(int, char*);
char strings[2048];
ssize_t sbyte,len,rfsize,rf,rstr[rec_buff];
char tFilename[255];
char grade_answer[2048];
char file_name_to_send[2048];
char assignment_response[2048];
char course_response[2048];
char options_response[2048];
char login_response[2048];
char assignment_name_to_send;
int client_socket;








using namespace std;

//methods signatures
void CheckDatabase();
int choiceConverter(char *buffer, int size);
int SocketFactory(struct sockaddr_in server_address);
struct sockaddr_in CreateServerAddress();
void databaseTableChecks(sqlite3* database, const char tablename[]);
void* handle_client(void* thread_args);


void* UserLogin(void* thread_args);
void* ListAllStudents(void* thread_args);
void* InstructorAddStudent(void* thread_args);
void* InstructorNotifyAllStudents(void* thread_args);

int InstructorModifyGrade();
void* InstructorAddFile(void* thread_args);
int InstructorAddExam();
//Admin Functionality
void* AdminAddClassAddClass(void* thread_args);
int AdminDeleteClass();
int AdminDeleteStudent();

void* admin(void* thread_args);
void* instructor(void* thread_args);
void* student(void* thread_args);
void* AboutUs(void* thread_args);
void*  ContactUs(void* thread_args);
void* ForgotPassword(void* thread_args);
void* ForgotUsername(void* thread_arg);

//queries and database related constant strings
static const char* DATABASE_LOCATION = "/home/tejashshah/Downloads/record.db";
static const char* DATABASE_GET_ALL_STUDENTS_QUERY="SELECT * FROM student;";
static const char* DATABASE_GET_ALL_USERS_QUERY="SELECT * FROM USERS;";
static char* DATABASE_CREATE_USERS_TABLE="CREATE TABLE USERS(" \
                                        "ID             TEXT,"\
                                        "ROLE           TEXT,"\
                                        "NAME           TEXT,"\
                                        "PASSWORD       TEXT);";
static char* DATABASE_CREATE_STUDENTS_TABLE="CREATE TABLE STUDENT(" \
                                        "ID             TEXT,"\
                                        "NAME           TEXT ,"\
                                        "CLASS          TEXT,"\
                                        "  UNIQUE(ID));";
static const char* DATABASE_CREATE_CLASSES_TABLE="CREATE TABLE CLASSES(" \
                                        "CLASS_ID           TEXT ,"\
                                        "INSTRUCTOR          TEXT);";

static const char* DATABASE_CREATE_SINGLE_CLASS_TABLE="CREATE TABLE ?1 (" \
                                        "STUDENT_ID           TEXT,"\
                                        "NAME          TEXT, "\
                                         "POINTS     TEXT default NULL,"\
                                        "GRADE VARCHAR default NULL);";
static const char* DATABASE_CREATE_CMPE207_CLASS_TABLE="CREATE TABLE '207' (" \
                                        "STUDENT_ID           TEXT,"\
                                        "FIRSTNAME          TEXT, "\
                                         "LASTNAME          TEXT, "\
                                         "POINTS     TEXT default NULL,"\
                                        "GRADE VARCHAR default NULL);";
static const char* DATABASE_CREATE_CMPE209_CLASS_TABLE="CREATE TABLE '209' (" \
                                        "STUDENT_ID           TEXT,"\
                                        "FIRSTNAME          TEXT, "\
                                         "LASTNAME          TEXT, "\
                                         "POINTS     TEXT default '0',"\
                                        "GRADE VARCHAR default NULL);";

static const char* DATABASE_CREATE_FILES_TABLE="CREATE TABLE FILES (" \
                                        "DEPARTMENT           TEXT,"\
                                        "CLASS_ID          TEXT,"\
                                        "FILE_NAME          TEXT,"\
                                        "FILE_CONTENT       BLOB);";

static const char* DATABASE_CREATE_NOTIFICATIONS_TABLE="CREATE TABLE NOTIFICATIONS (" \
                                        "DEPARTMENT     TEXT,"\
                                        "CLASS_ID       TEXT,"\
                                        "CREATOR        TEXT,"\
                                        "NOTIFICATION_SUBJECT       TEXT,"\
                                        "NOTIFICATION_MESSAGE        TEXT,"\
                                        "NOTIFICATION_EXPIRATION        TEXT);";

static const char* DATABASE_ADD_NOTIFICATION = "INSERT INTO NOTIFICATIONS (DEPARTMENT, CLASS_ID,"\
                                                "CREATOR,NOTIFICATION_SUBJECT, NOTIFICATIN_MESSAGE) values"\
                                                "(?1,?2,?3,?4,?5);";
//static const char* DATABASE_CREATE_INSTRUCTOR_DETAILS = "create table Instructor_details(rowid INTEGER PRIMARY KEY ASC, name text NOT NULL, subject text NOT NULL, UNIQUE(name, subject), FOREIGN KEY(name) REFERENCES Login(name));";
//
//static const char* DATABASE_CREATE_ASSIGNMENT_DETAILS = "create table assignment_detials(assignmentid INTEGER PRIMARY KEY ASC, subject text NOT NULL, file text, UNIQUE(subject, assignmentid));";

static const char* DATABASE_ADD_CLASS_QUERY = "INSERT INTO classes(CLASS_ID, INSTRUCTOR,ENROLLED_STUDENT_COUNT ) "\
                                         "VALUES (?1,?2,?3);";

static const char* DATABASE_ADD_STUDENT_QUERY = "INSERT INTO STUDENT( NAME, CLASS)"\
                                      "VALUES ('?', '?');";
static const char* DATABASE_ADD_STUDENT_TO_CMPE207_QUERY = "INSERT INTO CMPE207 (STUDENT_ID, NAME)"\
                                      "VALUES (?1,?2);";

static const char* DATABASE_ADD_STUDENT_TO_CMPE209_QUERY = "INSERT INTO CMPE209 (STUDENT_ID, NAME)"\
                                      "VALUES (?1,?2);";

static const char* DATABASE_ADD_STUDENT_TO_CLASS_QUERY = "INSERT INTO '1000' (STUDENT_ID, NAME)"\
                                      "VALUES (?1,?2);";
static const char* DATABASE_STUDENT_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='student';";
static const char* DATABASE_CLASSES_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='classes';";
static const char* DATABASE_USERS_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='users';";
static const char* DATABASE_CMPE209_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='cmpe209';";
static const char* DATABASE_CMPE207_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='cmpe207';";
static const char* DATABASE_FILES_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='files';";
static const char* DATABASE_NOTIFICATIONS_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='notifications';";
static const char* DATABASE_ASSIGNMENT_DETAILS_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='assignment_detials';";
static const char* DATABASE_INSTRUCTOR_DETAILS_TABLE_EXIST = "select count(type) from sqlite_master where type='table'"\
                                "and name='Instructor_details';";

static const char* DATABASE_INSTRUCTOR_ADD_FILE = "insert into FILES (DEPARTMENT, CLASS_ID,FILE_NAME, FILE_CONTENT) values (?1,?2,?3,?4);";

static const char* DATABASE_USER_LOGIN_CHECK = "SELECT ROLE FROM USERS WHERE ID=?1 and PASSWORD=?2; ";
//TABLE NAMES
static const char* DATABASE_STUDENT_TABLE_NAME = "STUDENT";
static const char* DATABASE_CLASSES_TABLE_NAME = "CLASSES";
static const char* DATABASE_USERS_TABLE_NAME = "USERS";
static const char* DATABASE_CMPE209_TABLE_NAME = "CMPE209";
static const char* DATABASE_CMPE207_TABLE_NAME = "CMPE207";
static const char* DATABASE_FILES_TABLE_NAME = "FILES";
static const char* DATABASE_NOTIFICATIONS_TABLE_NAME = "NOTIFICATIONS";

static const char* DATABASE_INSTRUCTOR_DETAILS_TABLE_NAME = "Instructor_details";
static const char* DATABASE_ASSIGNMENT_DETAILS_TABLE_NAME = "assignment_detials";


//MESSAGES
static const char* SERVER_WELCOME_INIT_MESSAGE = "*********************\n"\
                                            "WELCOME TO CANVAS_PRO\n"\
                                            "*********************\n1] User Login\n"\
        "2] About\n3] Contact us\n4] Forgot password\n5] Forgot Username\nEnter your choice number: \n";
static const char* SERVER_INSTRUCTOR_INIT_ACTION_MENU = "\n*********************\n"\
                                                    "Instructor Menu \n"\
                                                    "*********************\n1] Add Student\n2] Add Files\n3] Notify All\n4] View Students\n5] Modify Grades\n ";
static const char* SERVER_INSTRUCTOR_ADDFILE_MESSAGE_NAME = "\n[?] Enter File Name:  ";
static const char* SERVER_INSTRUCTOR_ADDFILE_MESSAGE_CATEGORY = "\n[?] Choose Category  \n";
static const char* SERVER_INSTRUCTOR_ADDFILE_MESSAGE_CATEGORY_LIST = "1] Syllabus \n2] Slides \n3] Others\n";
static const char* SERVER_STUDENT_NAME_REQUEST = "[?] Enter the student name:\n";
static const char* SERVER_STUDENT_CLASS_NAME_REQUEST = "[?] Enter the class name:\n";
static const char* SERVER_CLASS_ID_REQUEST = "[?] Enter the your class id:\n";
static const char* SERVER_GET_CLASS_NAME = "[?] Enter the class name:\n";
static const char* SERVER_GET_DEPARTMENT_NAME = "[?] Enter the Department name:\n";
static const char* SERVER_GET_FILE_NAME = "[?] Enter the file name:\n";
static const char* SERVER_GET_FILE_CONTENT = "[?] Enter your file path:\n";
static const char* SERVER_GET_CLASS_ID = "[?] Enter your class id:\n";
static const char* SERVER_FILE_ADDED_SUCCESS = "[+] File uploaded successfully\n";
static const char* SERVER_FILE_NOT_FOUND_ERROR = "[-] File upload error\n";
const static char* SERVER_GET_NOTIFICATION_MESSAGE = "[?] Enter your notification message: \n";
const static char* SERVER_GET_NOTIFICATION_MESSAGE_SENT = "[+] Notification Sent\n";
const static char* SERVER_ABOUT_MESSAGE = "Our Server provide services to schools around the world, Group4\n";
const static char* SERVER_CONTACT_US_MESSAGE = "Email us at group4@sjsu.edu\n";


const static char* SERVER_ADD_FILE_COMMAND_HELP = "Command Format: <DEPARTMENT> <CLASS_ID> <FILE_NAME> <LOCAL_FILE_PATH>\n";
const static char* SERVER_ADD_NOTIFICATION_HELP = "Command Format: <DEPARTMENT> <CLASS_ID>"\
    " <CREATOR> <SUBJECT> <EXPIRE>\n";
const static char* SERVER_ADD_STUDENT_COMMAND_HELP = "Command Format: <FIRST_NAME> <LAST_NAME> <STUDENT_ID> <CLASS_DEPARTMENT> <CLASS_ID>\n";


static const char* SERVER_STUDENT_ID_REQUEST = "[?] Enter the student's id: ";
static const char* SERVER_CHOICE_ERROR_MESSAGE = "\n[-] Wrong choice, try again later\nBYE\n";
static const char* DATABASE_INSERTION_SUCCESS =  "[+] Data inserted into database";
static const char* DATABASE_INSERTION_ERROR = "[-] Error inserting into database";
static const char* DATABASE_OPEN_SUCCESS = "[+] Opened database successfully\n";

static const char* SERVER_LOGIN_MESSAGE_USERNAME = "**********LOGIN**********\nUSERNAME: \n";
static const char* SERVER_LOGIN_MESSAGE_PASSWORD = "PASSWORD: \n";
static const char* SERVER_LOGIN_SUCCESS = "[+] Login Success\n";
static const char* SERVER_LOGIN_FAILURE = "[-] Login Failed\n";
static const char* SERVER_ADD_STUDENT_SUCCESS_MESSAGE = "Student added to the class\n";

//SERVER INFORMATION
static const int SERVER_PORT = 58055;
static const char* SERVER_ADDRESS = "127.0.0.1";

//OTHER GLOBALS
static const int SUCCESS = 1;
static const int FAIL = 0;
static const int STUDENT_ROLE = 100;
static const int INSTRUCTOR_ROLE = 10;
static const int ADMIN_ROLE = 1;
static const int MAX_BUFFER = 128;
static const int MAX_LARGE_BUFFER = 4096;
static const int MAX_USERNAME = 20;
static const int MAX_STUDENT_ID_LENGTH = 20;
static const int MAX_CLASS_ID_LENGTH = 20;
static const int MAX_EMAIL_LENGTH = 40;
static const int MAX_PASSWORD = 20;
static const int MAX_STUDENT_NAME_LENGTH = 20;
static const int MAX_ENROLLED_CLASSES = 5;
static const int MAX_FILE_SIZE = 4096;
static const char* TABLES[] = {"NOTIFICATIONS" ,"FILES", "USERS" , "STUDENT", "CLASSES", "CMPE207","CMPE209"};

static sqlite3* DATABASE;

struct STUDENT {
    char id[MAX_STUDENT_ID_LENGTH];
    char class_id[MAX_CLASS_ID_LENGTH];
    char first_name[MAX_STUDENT_NAME_LENGTH];
    char last_name[MAX_STUDENT_NAME_LENGTH];
    int age;
    int classes[MAX_ENROLLED_CLASSES];
    char email[MAX_EMAIL_LENGTH];
};

struct FILE_STRUCT {
    char department[MAX_BUFFER];
    char file_name[MAX_BUFFER];
    char file_content[MAX_FILE_SIZE];
    char class_id[MAX_CLASS_ID_LENGTH];
};

struct thread_data{
    sqlite3* db;
    int client_socket;
};

int main() {

    int server_socket;
    sqlite3 *database;
    struct sockaddr_in server_address;
    socklen_t addr_size = (socklen_t )sizeof(server_address);
    int return_code;
    int counter = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    server_address = CreateServerAddress();
    server_socket = SocketFactory(server_address);
    system("lsof -i tcp| grep canvas_pr");
    CheckDatabase();
    while (true) {
        int client_socket;
        pthread_t thread;
        client_socket = accept(server_socket, (struct sockaddr *) &server_address, &addr_size);
        return_code = pthread_create(&thread, NULL, handle_client, (void *)client_socket);
        cout << return_code << endl;
        counter ++;
    }

    return 0;
}

void* handle_client(void* args) {
    char buffer[MAX_BUFFER];
    int role;
    int client_socket = (int)(size_t)args;
    struct thread_data data;
    data.db = DATABASE;
    data.client_socket = client_socket;
    int recv_bytes = 0;

    send(client_socket, SERVER_WELCOME_INIT_MESSAGE, strlen(SERVER_WELCOME_INIT_MESSAGE),0);
    recv_bytes = recv(client_socket, buffer ,MAX_BUFFER, 0);
    if (recv_bytes > 0) {
        int choice = choiceConverter(buffer, MAX_BUFFER);
        cout << choice << endl;
        //switch for main menu choice
        switch (choice) {
            case 1:
                role = (int)(size_t)UserLogin((void *) &data);
                cout << role << endl;
                //need to know what role the user has
                if (role == ADMIN_ROLE) {
                    admin((void *) &data);
                } else if (role == INSTRUCTOR_ROLE) {
                    instructor((void *) &data);
                } else if (role == STUDENT_ROLE) {
                    student((void *) &data);
                }
                break;
            case 2:
                AboutUs((void *) &data);
                break;
            case 3:
                ContactUs((void *) &data);
                break;
            case 4:
                ForgotPassword((void *)&data);
                break;
            case 5:
                ForgotUsername((void *)&data);
                break;
            default:
                break;
        }
    }

//        if ((UserLogin(database, client_socket))){
//                InstructorAddStudent(database, client_socket);
//
//
//        }else {
//            cout << "Fail" << endl;
//
    close(client_socket);

}

/**

 *  address builder
 * @return
 */
struct sockaddr_in CreateServerAddress(){

    struct sockaddr_in server_address;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_family = AF_INET;
    inet_aton(SERVER_ADDRESS,&server_address.sin_addr);
    memset(&server_address.sin_zero, 0, 8);
    return server_address;
}
int SocketFactory( sockaddr_in server_address){
    int server_socket, client_socket;
    //init socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket,(struct sockaddr *) &server_address, sizeof(server_address));
    listen(server_socket, 5);
    return server_socket;
}

/**
 *
 * @return
 */
void CheckDatabase(){
    int return_code;
    sqlite3 *database;
    //init DB
    //check the student table existence
    return_code = sqlite3_open(DATABASE_LOCATION, &database);
    if(return_code){
        fprintf(stderr, "[-] Can't open database: %s\n", sqlite3_errmsg(database));

    }else{
        fprintf(stderr, DATABASE_OPEN_SUCCESS);
    }
    for (int i = 0 ; i < sizeof(TABLES)/ sizeof(TABLES[0]) ; i++){
    //check if the database already exist or not
    databaseTableChecks(database, TABLES[i]);
    }
    DATABASE = database;
}

int choiceConverter(char *buffer, int size){
    int ascii = 48;
    int choice = int(buffer[0]) - ascii;
    memset(buffer, 0 , MAX_BUFFER);
    cout << "Choice: " << choice << endl;
    return choice;
}
void databaseTableChecks(sqlite3* database, const char *tablename){

    int return_code;
    sqlite3_stmt *statement;
    const char *err;
    const char* EXIST_QUERY;
    const char* CREATION_QUERY;
    const char* TABLE_NAME;

    if (tablename == DATABASE_STUDENT_TABLE_NAME) {
        EXIST_QUERY = DATABASE_STUDENT_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_STUDENTS_TABLE;
        TABLE_NAME = DATABASE_STUDENT_TABLE_NAME;
    }
    else if (tablename == DATABASE_CLASSES_TABLE_NAME){
    EXIST_QUERY = DATABASE_CLASSES_TABLE_EXIST;
    CREATION_QUERY = DATABASE_CREATE_CLASSES_TABLE;
    TABLE_NAME = DATABASE_CLASSES_TABLE_NAME;
    }
    else if (tablename == DATABASE_USERS_TABLE_NAME){
        EXIST_QUERY = DATABASE_USERS_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_USERS_TABLE;
        TABLE_NAME = DATABASE_USERS_TABLE_NAME;
    }
    else if (tablename == DATABASE_CMPE207_TABLE_NAME){
        EXIST_QUERY = DATABASE_CMPE207_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_CMPE207_CLASS_TABLE;
        TABLE_NAME = DATABASE_CMPE207_TABLE_NAME;
    }
    else if (tablename == DATABASE_CMPE209_TABLE_NAME){
        EXIST_QUERY = DATABASE_CMPE209_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_CMPE209_CLASS_TABLE;
        TABLE_NAME = DATABASE_CMPE209_TABLE_NAME;
    }
    else if (tablename == DATABASE_FILES_TABLE_NAME){
        EXIST_QUERY = DATABASE_FILES_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_FILES_TABLE;
        TABLE_NAME = DATABASE_FILES_TABLE_NAME;
    }

    else if (tablename == DATABASE_NOTIFICATIONS_TABLE_NAME){
        EXIST_QUERY = DATABASE_NOTIFICATIONS_TABLE_EXIST;
        CREATION_QUERY = DATABASE_CREATE_NOTIFICATIONS_TABLE;
        TABLE_NAME = DATABASE_NOTIFICATIONS_TABLE_NAME;
    }
//    else if (tablename == DATABASE_ASSIGNMENT_DETAILS_TABLE_NAME){
//        EXIST_QUERY = DATABASE_ASSIGNMENT_DETAILS_TABLE_EXIST;
//        CREATION_QUERY = DATABASE_CREATE_ASSIGNMENT_DETAILS;
//        TABLE_NAME = DATABASE_ASSIGNMENT_DETAILS_TABLE_NAME;
//    }
//    else if (tablename == DATABASE_INSTRUCTOR_DETAILS_TABLE_NAME){
//        EXIST_QUERY = DATABASE_INSTRUCTOR_DETAILS_TABLE_EXIST;
//        CREATION_QUERY = DATABASE_CREATE_INSTRUCTOR_DETAILS;
//        TABLE_NAME = DATABASE_INSTRUCTOR_DETAILS_TABLE_NAME;
//    }

    //check the database table existence
    return_code = sqlite3_prepare_v2(database, EXIST_QUERY, -1, &statement, &err);
//    sqlite3_bind_blob(statement,1,TABLE_NAME, -1, SQLITE_STATIC);
    return_code = sqlite3_step(statement);
    //create the table
    if (sqlite3_column_int(statement, 0) == 0){
        cout << "[-] Table doesnt exist, need to create it" << endl;
        return_code = sqlite3_prepare_v2(database, CREATION_QUERY, -1,&statement,&err);
        return_code = sqlite3_step(statement);
        sqlite3_finalize(statement);
        //check if table creation was successful
        if (return_code == SQLITE_OK) {
            cout << "[+] Created " << tablename << " Table" << endl;
        }
        else {
            cout << sqlite3_errmsg(database) << endl;
        }
    }
    else{
        cout << "[+] No need to create "<<tablename<<" table" << endl;
    }
}

/**
 * User Authentication to access the server
 * @param db
 * @param client_socket
 * @return
 */
void* UserLogin(void* thread_args){

    char id[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int return_code;
    const char *err;
    char* role;
    int code = 0;
    sqlite3_stmt *statement;
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;
    //get username
    memset(id, 0 , MAX_USERNAME);
    memset(password, 0 , MAX_PASSWORD);
    id[strlen(id)-1] = '\0';
    password[strlen(password)-1] = '\0';

    send(client_socket,SERVER_LOGIN_MESSAGE_USERNAME, strlen(SERVER_LOGIN_MESSAGE_USERNAME), 0);
    recv(client_socket, id, MAX_USERNAME, 0);
    //get password
    send(client_socket, SERVER_LOGIN_MESSAGE_PASSWORD, strlen(SERVER_LOGIN_MESSAGE_PASSWORD), 0);
    recv(client_socket, password, MAX_PASSWORD, 0);

    id[strlen(id)-1] = '\0';
    password[strlen(password)-1] = '\0';

    //check data base
    return_code = sqlite3_prepare_v2(database,DATABASE_USER_LOGIN_CHECK,-1, &statement, &err);
    if (return_code != SQLITE_OK){
        cout << sqlite3_errmsg(database) << endl;
    }else{
        sqlite3_bind_text(statement, 1, id, strlen(id), SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, password, strlen(password), SQLITE_STATIC);
        return_code = sqlite3_step(statement);
        if (return_code == SQLITE_ROW){
                send(client_socket, SERVER_LOGIN_SUCCESS,(size_t) strlen(SERVER_LOGIN_SUCCESS),0);
                role =  (char *)sqlite3_column_text(statement, 0) ;
                cout << role << endl;
                if (strcmp(role,"student") == 0){
                    code = STUDENT_ROLE;
                }
                else if (strcmp(role, "instructor") == 0){
                    code = INSTRUCTOR_ROLE;
                }
                else if (strcmp(role, "admin")==0){
                    code = ADMIN_ROLE;
                }
//                sqlite3_column_text(statement, )
        }else {

            send(client_socket, SERVER_LOGIN_FAILURE, (size_t) strlen(SERVER_LOGIN_FAILURE), 0);
                close(client_socket);
                return (void *)FAIL;
        }

    }
    return (void *)code;
}
/**
 * Method will list all the students enrolled in the class
 * @param db: sqlite3 Database obj
 * @return SUCCESS
 */
void* ListAllStudents(void* thread_args){
    sqlite3_stmt *statement;
    int return_code;
    const char* err;
    char* name;
    char * role;
    char *cls;
    cout << "check 1" << endl;
    struct thread_data* data= (struct thread_data *)thread_args;
    cout << "check 2" << endl;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;
    cout << "check 3" << endl;
    return_code = sqlite3_prepare_v2(database, DATABASE_GET_ALL_USERS_QUERY, -1, &statement, &err);
    if (return_code == SQLITE_OK) {
//        sqlite3_bind_text(statement, 1, DATABASE_USERS_TABLE_NAME, -1, SQLITE_STATIC);
        while ((return_code = sqlite3_step(statement)) == SQLITE_ROW) {
            role =  (char *)sqlite3_column_text(statement, 0) ;
            name =  (char *)sqlite3_column_text(statement, 1) ;
            cls =  (char *)sqlite3_column_text(statement, 2) ;
            stringstream stream;
            stream << name << " " << role << " " << cls << "\n";
            string s = stream.str();
            send(client_socket, s.c_str(), strlen(s.c_str()), 0);
         }

    } else {
        cout << "ListAllStudents else" << endl;
        cout << sqlite3_errmsg(database)<< endl;
    }
}

/**
 *
 * @param database
 * @param client_socket
 * @return
 */
void* InstructorAddStudent(void* thread_args){

    sqlite3_stmt *statement;
    const char* err;
    int return_code;
    string fname, lname, id, department, class_id,q;
    char buffer[MAX_LARGE_BUFFER];
    memset(buffer, 0 , MAX_LARGE_BUFFER);
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

    send(client_socket,SERVER_ADD_STUDENT_COMMAND_HELP, strlen(SERVER_ADD_STUDENT_COMMAND_HELP), 0 );
    recv(client_socket, buffer, MAX_LARGE_BUFFER, 0);
    string tmp(buffer, MAX_LARGE_BUFFER);
    istringstream args(tmp);
    string t;
    args >> t;
    fname = t;
    args >> t;
    lname = t;
    args >> t;
    id = t;
    args >> t;
    department = t;
    args >> t;
    class_id = t;
    memset(buffer, 0 , MAX_LARGE_BUFFER);
    stringstream query;

    query << "INSERT INTO " << "'"<< class_id <<"'"<<
                                        " (student_id,firstname, lastname) values('" <<id <<"','"<<
                                         fname <<"','"<<lname << "');";

    q = query.str();
    const char *insert_query = q.c_str();

    return_code= sqlite3_prepare_v2(database, insert_query, -1,&statement,&err);
    if (return_code == SQLITE_OK) {
        return_code = sqlite3_step(statement);
        sqlite3_finalize(statement);
        if (return_code != SQLITE_DONE) {
            cout << DATABASE_INSERTION_ERROR << endl;
            cout << sqlite3_errmsg(database) << endl;
        } else {
            cout << DATABASE_INSERTION_SUCCESS << endl;
            send(client_socket,SERVER_ADD_STUDENT_SUCCESS_MESSAGE, strlen(SERVER_ADD_STUDENT_SUCCESS_MESSAGE), 0 );

        }
    }else {
        cout << sqlite3_errmsg(database)<<endl;
    }

}
//
///*
// *
// *
// */
void* AdminAddClass(void* thread_args){
    sqlite3_stmt *statement;
    const char* err;
    int return_code;
    char buffer[MAX_STUDENT_NAME_LENGTH];
    struct STUDENT new_student;
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

//    //get class id from isntructor
//    send(client_socket, SERVER_CLASS_ID_REQUEST, strlen(SERVER_CLASS_ID_REQUEST), 0);
//    recv(client_socket, buffer, sizeof(buffer), 0);
//    new_student.class_id = atoi(buffer);
//    //ask the client to enter the data
//    send(client_socket, SERVER_STUDENT_NAME_REQUEST, strlen(SERVER_STUDENT_NAME_REQUEST), 0);
//    recv(client_socket, buffer, sizeof(buffer), 0);
//    strcpy(new_student.NAME , buffer);
//    //get student id
//    send(client_socket, SERVER_STUDENT_ID_REQUEST, strlen(SERVER_STUDENT_ID_REQUEST), 0);
//    recv(client_socket, buffer, sizeof(buffer), 0);
//    new_student.id = atoi(buffer);

    return_code= sqlite3_prepare_v2(database, DATABASE_ADD_CLASS_QUERY, -1,&statement,&err);
    if (return_code == SQLITE_OK) {
        sqlite3_bind_text(statement, 1, "1000", -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 2, "jedi", -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 3, "1", -1, SQLITE_STATIC);
        return_code = sqlite3_step(statement);
        sqlite3_finalize(statement);
        if (return_code != SQLITE_DONE) {
            cout << DATABASE_INSERTION_ERROR << endl;
            cout << sqlite3_errmsg(database) << endl;
        } else {
            cout << DATABASE_INSERTION_SUCCESS << endl;
        }
    }else {
        cout << sqlite3_errmsg(database)<<endl;
    }

}


void* InstructorAddFile(void* thread_args){

    //DEPARTMENT, CLASS_ID, FILE_NAME, CONTENT
    sqlite3_stmt *statement;
    int return_code;
    const char* err;
    char buffer[MAX_LARGE_BUFFER];
    struct FILE_STRUCT file;
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

    memset(buffer, 0 , MAX_LARGE_BUFFER);
    // dept class_id file_name file_path
    send(client_socket,SERVER_ADD_FILE_COMMAND_HELP, strlen(SERVER_ADD_FILE_COMMAND_HELP), 0 );
    recv(client_socket, buffer, MAX_LARGE_BUFFER, 0);
    buffer[strlen(buffer)-1] = '\0';
    string tmp(buffer, MAX_LARGE_BUFFER);
    istringstream args(tmp);
    string t;
    args >> t;
    strcpy(file.department, t.c_str());
    args >> t;
    strcpy(file.class_id ,t.c_str());
    args >> t;
    strcpy(file.file_name ,t.c_str());
    memset(buffer, 0 , MAX_LARGE_BUFFER);

    send(client_socket,SERVER_GET_FILE_CONTENT, strlen(SERVER_GET_FILE_CONTENT), 0 );
    recv(client_socket, buffer, MAX_LARGE_BUFFER, 0);
    buffer[strlen(buffer)-1] = '\0';
    strcpy(file.file_content,buffer);
    memset(buffer, 0 , MAX_LARGE_BUFFER);

    return_code = sqlite3_prepare_v2(database, DATABASE_INSTRUCTOR_ADD_FILE, -1, &statement, &err);
    sqlite3_bind_text(statement, 1, file.department, -1, SQLITE_STATIC );
    sqlite3_bind_text(statement, 2, file.class_id, -1, SQLITE_STATIC );
    sqlite3_bind_text(statement, 3, file.file_name, -1, SQLITE_STATIC );
    sqlite3_bind_blob(statement, 4, file.file_content, -1, SQLITE_STATIC );
    return_code = sqlite3_step(statement);
    sqlite3_finalize(statement);

    if (return_code == SQLITE_OK) {
            send(client_socket, SERVER_FILE_ADDED_SUCCESS, strlen(SERVER_FILE_ADDED_SUCCESS), 0);
            memset(buffer, 0 , strlen(buffer));
        }
    else {
        cout << sqlite3_errmsg(database)<<endl;
        send(client_socket, SERVER_FILE_NOT_FOUND_ERROR, strlen(SERVER_FILE_NOT_FOUND_ERROR), 0);

    }
}

/*
 *
 *
 */
int AdminDeleteStudent(){


}


/*
 *
 *
 */
void* InstructorNotifyAllStudents(void* thread_args){

//    IFICATIONS_TABLE="CREATE TABLE NOTIFICATIONS (" \
//                                        "DEPARTMENT     TEXT,"\
//                                        "CLASS_ID       TEXT,"\
//                                        "CREATOR        TEXT,"\
//                                        "NOTIFICATION_SUBJECT       TEXT,"\
//                                        "NOTIFICATIN_MESSAGE        TEXT,"\
//                                        "NOTIFICATION_EXPIRATION        TEXT);";

    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

    sqlite3_stmt *statement;
    int return_code;
    const char* err;
    char buffer[MAX_BUFFER];
    string department, class_id, creator, notif_subj,notif_expire, q;

    memset(buffer, 0 , MAX_BUFFER);
    // dept class_id file_name file_path

    send(client_socket,SERVER_ADD_NOTIFICATION_HELP, strlen(SERVER_ADD_NOTIFICATION_HELP), 0 );
    recv(client_socket, buffer, MAX_BUFFER, 0);
    buffer[strlen(buffer)-1] = '\0';
    string tmp(buffer, strlen(buffer));
    istringstream args(tmp);
    string t;
    args >> t;
    department = t;
    args >> t;
    class_id = t;
    args >> t;
    creator = t;
    args >> t;
    notif_subj = t;
    args >> t;
    notif_expire = t;
    memset(buffer, 0 , MAX_BUFFER);

    send(client_socket,SERVER_GET_NOTIFICATION_MESSAGE, strlen(SERVER_GET_NOTIFICATION_MESSAGE), 0 );
    recv(client_socket, buffer, MAX_BUFFER, 0);
    buffer[strlen(buffer)-1] = '\0';
    string notif_msg(buffer, strlen(buffer));
    //building the query
    stringstream stream;
    stream << "INSERT INTO NOTIFICATIONS "<< "(department, class_id,creator,notification_subject,"\
                                                               "notification_message, notification_expiration) values ('"<< department <<"','"<<class_id<<"','"<<creator<<"','"<<notif_subj<<"','"<<notif_msg<<"','"<<notif_expire<<"');";
    q = stream.str();
    const char* query = q.c_str();

    return_code = sqlite3_prepare_v2(database, query, -1, &statement, &err);
    if (return_code == SQLITE_OK){
        return_code = sqlite3_step(statement);
        sqlite3_finalize(statement);
        if (return_code == SQLITE_DONE){
            send(client_socket,SERVER_GET_NOTIFICATION_MESSAGE_SENT, strlen(SERVER_GET_NOTIFICATION_MESSAGE_SENT), 0 );
        }else {
            cout << sqlite3_errmsg(database) << endl;
        }
    }
}

/*
 *
 *
 */
int AdminModifyGrade(){


return SUCCESS;
}

/*
 *
 *
 */
int AdminAddExam(){



 return SUCCESS;
}

void* admin(void* thread_args) {
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    int rc;
    sqlite3_stmt *stmnt;
    int n1;
    char *id = NULL;
    char *name = NULL;
    char *password = NULL;
    char *role = NULL;
    char *query;
    const char *errmsg;
    char *result;
    char client_buffer[1500], *decrypt;
    char *Succ_Login = "\n\nLogged in Successfully!!!!!\n\n\nWhat would you like to do next\n\n1. Register Student/Instructor\n2. Add Subject to Instructor\n3.View subject for an instructor\n4.View all Instructors\n5.View all students\n6.Delete instructor from a subject\n7.Delete Student/Instructor \n8.Exit\n";
    char *Failure_Login = "\n****ERROR****Incorrect Username or Password\n Please try again <Username> <Password>\n";
    char *Welcome_screen_sa = "\nWelcome System Admin. Please login. Enter <Username> <Password>\n\n";
    char *Sys_admin_options = "\n\nWhat would you like to do next\n\n1. Register Student/Instructor\n2. Add Subject to Instructor\n3.View subject for an instructor\n4.View all Instructors\n5.View all students\n6.Delete instructor from a subject\n7.Delete Student/Instructor \n8.Exit\n";
    char *add_user = "\n\nPlease enter user details to add a user: <ID> <role> <username> <password>:\n\n";
    char *modify_user = "\n\nPlease enter new password: <Username> <password>:\n\n";
//	strcat(add_user,"Enter in the below format:\n");
//	strcat(add_user,"<User Name> <Password>");

//        bzero(client_buffer,1500);
//        printf("writing to the client\n");
//        strcpy(client_buffer,Welcome_screen_sa);
//
//        n1=write(client_socket,client_buffer,strlen(client_buffer));
//        if (n1 <= 0){
//            perror("Server:ERROR writing to socket\n");
//        }
//        while(1){
//            bzero(client_buffer,1500);
//            /*Reading value from the client*/
//
//            n1 = read(client_socket,client_buffer,1500);
//            if (n1 < 0) {
//                perror("ERROR reading from socket \n");
//            }
//            decrypt=Decrypt(client_buffer);
//            strcpy(client_buffer,decrypt);
//            if(login(client_buffer,"System_Admin","Login")==1){
    bzero(client_buffer, 1500);
    printf("Logged in successfully\n");
    strcpy(client_buffer, Succ_Login);
    n1 = write(client_socket, client_buffer, strlen(client_buffer));
    bzero(client_buffer, 1500);

//                pass_match = NULL;
//                break;
//            }
//            else {
//                bzero(client_buffer,1500);
//                printf("writing to the client\n");
//                strcpy(client_buffer,Failure_Login);
//                n1=write(client_socket,client_buffer,strlen(client_buffer));
//            }
//
//            if (n1 <= 0){
//                perror("Server:ERROR writing to socket\n");
//            }
//        }

    while (true) {
        bzero(client_buffer, 1500);
        n1 = read(client_socket, client_buffer, 1500);
        if (n1 < 0) {
            perror("ERROR reading from socket\n");
        }
        //printf("option asked:%s:\n",client_buffer);
        client_buffer[strlen(client_buffer) - 1] = '\0';
        printf("option asked:%c:\n", client_buffer[0]);
        if (client_buffer[0] == '1') {
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, add_user);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            n1 = read(client_socket, client_buffer, 1500);
            if (n1 < 0) {
                perror("ERROR reading from socket \n");
            }
            id = NULL;
            name = NULL;
            password = NULL;
            role = NULL;
            id = strtok(client_buffer, " ");
            role = strtok(NULL, " ");
            name = strtok(NULL, " ");
            int i = 0;
            while (i < 1) {
                password = strtok(NULL, " ");
                i++;
            }
            asprintf(&query, "insert into users (ID, ROLE, NAME, PASSWORD) values ('%s','%s','%s', '%s');", id, role, name, password);
            printf(query);
            sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            rc = sqlite3_step(stmnt);
            if (rc == SQLITE_OK) {
                cout << "User inserted" << endl;
            }
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;
        }
        if (client_buffer[0] == '2') {
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, "\n\nPlease add an instructor to class: <Instructor Name> <Class{209/207}>\n\n");
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            n1 = read(client_socket, client_buffer, 1500);
            if (n1 < 0) {
                perror("ERROR reading from socket \n");
            }
            name = NULL;
            role = NULL;
            client_buffer[strlen(client_buffer) - 1] = '\0';
            name = strtok(client_buffer, " ");
            role = strtok(NULL, " ");
            asprintf(&query, "insert into classes (CLASS_ID, INSTRUCTOR) values ('%s', '%s');", role, name);
            printf(query);
            sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            rc = sqlite3_step(stmnt);
            if (rc == SQLITE_OK) {
                cout << ""
                        "Instructor added" << endl;
            }
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;
        }
        if (client_buffer[0] == '3') {
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, "\n\nEnter the instructor name: <name>\n\n");
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            n1 = read(client_socket, client_buffer, 1500);
            if (n1 < 0) {
                perror("ERROR reading from socket \n");
            }
            client_buffer[strlen(client_buffer) - 1] = '\0';
            asprintf(&query,"SELECT CLASS_ID from CLASSES where instructor = '%s';",client_buffer);
            rc = sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            if (rc != SQLITE_OK) {
                printf("unable to prep");
            }
            bzero(client_buffer, 1500);
            rc = sqlite3_step(stmnt);
            strcat(client_buffer,"Subjects for the instructor are:\n");
            while (rc == SQLITE_ROW) {
                asprintf(&result,"%s", sqlite3_column_text(stmnt, 0));
                strcat(client_buffer,result);
                strcat(client_buffer,"\n");
                rc = sqlite3_step(stmnt);
            }
            sqlite3_finalize(stmnt);
            printf("writing to the client\n");
            strcat(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;

        }
        if (client_buffer[0] == '4') {
            bzero(client_buffer, 1500);
            asprintf(&query,"SELECT name from users where role = 'instructor';");
            rc = sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            if (rc != SQLITE_OK) {
                printf("unable to prep");
            }
            rc = sqlite3_step(stmnt);
            strcat(client_buffer,"Instructors in this system:\n");
            while (rc == SQLITE_ROW) {
                asprintf(&result,"%s", sqlite3_column_text(stmnt, 0));
                strcat(client_buffer,result);
                strcat(client_buffer,"\n");
                rc = sqlite3_step(stmnt);
            }
            sqlite3_finalize(stmnt);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;
        }
        if (client_buffer[0] == '5') {
            bzero(client_buffer, 1500);
            asprintf(&query,"SELECT name from users where role = 'student';");
            rc = sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            if (rc != SQLITE_OK) {
                printf("unable to prep");
            }
            rc = sqlite3_step(stmnt);
            strcat(client_buffer,"Students in this system:\n");
            while (rc == SQLITE_ROW) {
                asprintf(&result,"%s", sqlite3_column_text(stmnt, 0));
                strcat(client_buffer,result);
                strcat(client_buffer,"\n");
                rc = sqlite3_step(stmnt);
            }
            sqlite3_finalize(stmnt);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;
        }
        if (client_buffer[0] == '6') {
            bzero(client_buffer, 1500);
            strcat(client_buffer,"Please enter <class_name {");
            asprintf(&query,"SELECT DISTINCT class_id from classes;");
            rc = sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            if (rc != SQLITE_OK) {
                printf("unable to prep");
            }
            rc = sqlite3_step(stmnt);
            strcat(client_buffer,"Select from below classes\n");
            while (rc == SQLITE_ROW) {
                asprintf(&result,"%s", sqlite3_column_text(stmnt, 0));
                strcat(client_buffer,result);
                strcat(client_buffer,"/");
                rc = sqlite3_step(stmnt);
            }
            sqlite3_finalize(stmnt);
            strcat(client_buffer,"} and <instructor name>:\n\n");
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            n1 = read(client_socket, client_buffer, 1500);
            if (n1 < 0) {
                perror("ERROR reading from socket \n");
            }
            client_buffer[strlen(client_buffer) - 1] = '\0';
            name = NULL;
            role = NULL;
            name = strtok(client_buffer, " ");
            role = strtok(NULL, " ");
            asprintf(&query, "Delete from CLASSES where CLASS_ID = '%s' AND INSTRUCTOR = '%s';", name, role);
            printf(query);
            sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            rc = sqlite3_step(stmnt);
            if (rc == SQLITE_OK) {
                cout << "Class deleted" << endl;
            }
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            query = NULL;
        }
        if (strcmp(client_buffer, "7") == 0) {
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, "Please enter User name: <name>\n\n");
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            n1 = read(client_socket, client_buffer, 1500);
            if (n1 < 0) {
                perror("ERROR reading from socket \n");
            }
            client_buffer[strlen(client_buffer) - 1] = '\0';
            asprintf(&query,"SELECT CLASS_ID from CLASSES where instructor = '%s';",client_buffer);
            rc = sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
            if (rc != SQLITE_OK) {
                printf("unable to prep");
            }
            int flag=0;
            rc = sqlite3_step(stmnt);
            while (rc == SQLITE_ROW) {
                flag++;
                //asprintf(&result,"%s", sqlite3_column_text(stmnt, 0));
                rc = sqlite3_step(stmnt);
            }
            sqlite3_finalize(stmnt);
            if(flag==0){
                asprintf(&query, "DELETE from users where name = '%s';",client_buffer);
                printf(query);
                sqlite3_prepare_v2(database, query, -1, &stmnt, &errmsg);
                rc = sqlite3_step(stmnt);
                if (rc == SQLITE_OK) {
                    cout << "User Deleted" << endl;
                }
                bzero(client_buffer, 1500);
            }
            else{
                bzero(client_buffer,1500);
                strcat(client_buffer,"The instructor is mapped to a subject. Kindly remove the mapping before deleting the user.\n\n");
            }
            printf("writing to the client\n");
            strcat(client_buffer, Sys_admin_options);
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer,1500);
        }
            if (strcmp(client_buffer, "8") == 0) {
            bzero(client_buffer, 1500);
            printf("writing to the client\n");
            strcpy(client_buffer, "Have a Wonderfull day!!!!\n");
            n1 = write(client_socket, client_buffer, strlen(client_buffer));
            if (n1 <= 0) {
                perror("Server:ERROR writing to socket\n");
            }
            bzero(client_buffer, 1500);
            return 0;
        }
    }
}

void* AboutUs(void* thread_args){
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;
    send(client_socket, SERVER_ABOUT_MESSAGE, strlen(SERVER_ABOUT_MESSAGE), 0);

}

void*  ContactUs(void* thread_args){
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;
    send(client_socket, SERVER_CONTACT_US_MESSAGE , strlen(SERVER_CONTACT_US_MESSAGE), 0);


}

void* ForgotPassword(void* thread_args) {
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

}

void* ForgotUsername(void* thread_args) {
    struct thread_data* data= (struct thread_data *)thread_args;
    sqlite3* database = data->db;
    int client_socket = data->client_socket;

}


void* instructor(void* thread_args){

    int choice;
    char buffer[MAX_BUFFER];
    int recv_byte = 0;
    memset(buffer, 0, MAX_BUFFER);
    struct thread_data* data= (struct thread_data *)thread_args;
    int client_socket = data->client_socket;
    //after login
    send(client_socket, SERVER_INSTRUCTOR_INIT_ACTION_MENU, strlen(SERVER_INSTRUCTOR_INIT_ACTION_MENU),0);
    recv_byte = recv(client_socket, buffer ,MAX_BUFFER, 0);
    cout << recv_byte << endl;
    cout << buffer << endl;
    if (recv_byte > 0) {
        choice = choiceConverter(buffer, MAX_BUFFER);
        switch (choice) {
            case 1:
                //add student
                InstructorAddStudent(thread_args);
                break;
            case 2:
                InstructorAddFile(thread_args);
                break;
            case 3:
                InstructorNotifyAllStudents(thread_args);
                break;
            case 4:
                //list all students
                ListAllStudents(thread_args);
                break;
            case 5:
                //Modify grades
                break;
            default:
                send(client_socket, SERVER_CHOICE_ERROR_MESSAGE, strlen(SERVER_CHOICE_ERROR_MESSAGE), 0);
//                close(client_socket);
        }
    }
}



void* student(void* thread_args){
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;


    //write(client_socket,"Hi\n",2);

    char *zErrMsg = 0;
    int rc;
    int SID;
    char ans1[2048];
    int socktcp,listen_value=0;
    char file_integer[2048];
    struct sockaddr_in server,client;
    socklen_t client_size;
    client_size = sizeof(struct sockaddr_in);
    //memset(&server, 0, sizeof(server));
    //server.sin_addr.s_addr = htonl(INADDR_ANY);
    
  //memset
  //server.sin_family=AF_INET;
  //server.sin_port=htons(server_port);
  //return server

  //----------------------------------------------------------------CREATE AND CHECK SOCKET--------------------------------------------//////
   /* socktcp=socket(AF_INET,SOCK_STREAM,0);
    if(socktcp<0)
    {
        printf(" \n \n Error Creating Socket \n \n ");
        exit(-1);
    }
    
    //----------------------------------------------------------------BIND SOCKET--------------------------------------------------------//////
     
     if ( bind(socktcp, (struct sockaddr*)&server, sizeof(server)) < 0 )
    {
        printf("Error in Binding\n");
        exit(-1);   
    }
    //----------------------------------------------------------------LISTEN AND ACCEPT--------------------------------------------------//////
    listen_value=listen(socktcp,5);
    for (;;)
    {
        printf("Waiting for client to connect\n");
        accept_value =accept(socktcp, (struct sockaddr*)&client, &(client_size));
    write(accept_value,"Student or Instructor ?",23);
    read(accept_value,&login_response,sizeof(login_response));
    */
  //------------------------------------------------------------OPEN DATABASE----------------------------------------------------------/////
        rc = sqlite3_open("student.db", &db);
        if( rc ){
           fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
           return(0);
                }
        else 
        {
            printf("Student Database open\n");
        }
    //----------------------------------------------ASK FOR ASSIGNMENTS, COURSES AND GRADES -----------------------------------------------/////
  
    R : asscogra(thread_args);
        read(client_socket,ans1,sizeof(ans1));
        printf("Student opts for %s\n",ans1);
        ans1[strlen(ans1) - 1] = '\0';
        printf("Value=%d\n",strcmp(ans1,"Assignments"));
        if (strcmp(ans1,"Assignments")==0)
        {
            courses(thread_args);
            read(client_socket,course_response,sizeof(course_response));
            printf(course_response);
            assignment(thread_args);
    Label : read(client_socket,&assignment_response,sizeof(assignment_response));
            assignment_response[strlen(assignment_response) - 1] = '\0';
            printf("Hi\n");
            printf("assignment_response initally=%s\n",&assignment_response);
            sprintf(tFilename,"%s.pdf",assignment_response);
            printf("Filename to be sent : %s\n",tFilename);
            open_assignments_from_database(assignment_response);
            transfer_assignmentfile_to_client(accept_value,tFilename);        
        }
        else if (strcmp(ans1,"Courses")==0)
        {
            courses(thread_args);
            read(client_socket,course_response,sizeof(course_response));
            printf("Course response=%s\n",course_response);
            options(thread_args);
            read(client_socket,options_response,sizeof(options_response));
            options_response[strlen(options_response) - 1] = '\0';
            if(strcmp(options_response,"Files")==0)
            {
                file_name(thread_args);
                read(client_socket,&file_integer,sizeof(file_integer));
                file_integer[strlen(file_integer) - 1] = '\0';
                printf("File integer is %s\n",&file_integer);
                printf("file_response initally=%s\n",file_integer);
                sprintf(tFilename,"%s.pdf",file_integer);
                printf("Filename to be sent : %s\n",tFilename);
                open_file_from_database(file_integer);
                transfer_coursefile_to_client(accept_value,tFilename);
            }
            else if (strcmp(options_response,"Assignments")==0)
            {
                assignment(thread_args);
                goto Label ;
            }
        else
            { 
                write(client_socket, "Invalid Input, Try Again\n",26);
                goto R;
            }

        }
        else if (strcmp(ans1,"Grades")==0)
        {
            grades(thread_args);
            read(client_socket,&grade_answer,sizeof(grade_answer));
            grade_answer[strlen(grade_answer) - 1] = '\0';
            printf("grade answer is %s\n",&grade_answer);
            printf("file_response initally=%s\n",grade_answer);
            sprintf(tFilename,"%s.pdf",grade_answer);
            printf("Filename to be sent : %s\n",tFilename);
            open_grades_from_database(grade_answer);
            transfer_gradefile_to_client(accept_value,tFilename);
            printf("After open_grades_from_database\n");
        }

        else
        {
            write(client_socket, "Invalid Input, Try Again\n",26);
            goto R;
        }
  //goto R;

}


 
//---------------------------------------------------ASK FOR ASSIGNMENTS, COURSE OR GRADES----------------------------------------------////
int asscogra(void * thread_args)
{
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich section you want to go into:\n1] Assignments\n2] Courses\n3] Grades\n\n",79);
    /*write(client_socket,"Assignments  ",13);
    write(client_socket,"Courses  ",9);
    write(client_socket,"Grades  ",8);
    */
}

//----------------------------------------------------------------ASK ASSIGNMENTS ------------------------------------------------------////

int assignment(void *thread_args)
{
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich Week Assignment you want to download:\n1] WEEK1\n2] WEEK2\n3] WEEK3\n4] WEEK4\n\n",88);
    //write(client_socket,"WEEK1 ",6);
    //write(client_socket,"WEEK2 ",6);
    //write(client_socket,"WEEK3 ",6);
    //write(client_socket,"WEEK4 ",6);
}

//----------------------------------------------------------------ASK COURSES ---------------------------------------------------------////
int courses (void *thread_args)
{ 
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich course no:\n1] 207\n2] 209\n\n",38);
    //write(client_socket,"209   ",6);
    //write(accept_value,"English  ",9);
}
//----------------------------------------------------------ASK WHICH SUBJECT GRADES ---------------------------------------------------------////
int grades(void *thread_args)
{
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich course grades you want to download?\n1] 207Grades\n2] 209Grades\n\n",70);
    //write(client_socket,"209Grades   ",12);
    //write(accept_value,"English===Grades   ",16);
}
//----------------------------------------------------------------DISPLAY OPTIONS ---------------------------------------------------------////
int options(void *thread_args)
{
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich section you want to go into:\n1] Files\n2] Assignments\n\n",62);
    //write(client_socket,"Assignments  ",13);
}
//----------------------------------------------------------------DISPLAY FILES NAMES ---------------------------------------------------------////
int file_name(void * thread_args)
{
    struct thread_data *data = (struct thread_data *) thread_args;
    sqlite3 *database = data->db;
    int client_socket = data->client_socket;
    write(client_socket,"\nWhich Chapter File you want to download:\n1] Chapterone\n2] Chaptertwo\n3] Chapterthree \n4] Chapterfour\n\n",104);
}
//------------------------------------------------------------OPEN ASSIGNMENTS FROM DATABASE ---------------------------------------------------------////


void open_assignments_from_database(char assignment_response[])
   {
    printf("assignment_response in database =%s\n",assignment_response);
    if(strcmp(assignment_response,"WEEK1")==0) 
    {
      printf("checker.....in assignment response .....\n");
      fp = fopen("WEEK1.pdf", "wb");
      file_open_checker();
      row=1;
      read_assignments_from_database();
    }
    else if(strcmp(assignment_response,"WEEK2")==0) 
    {
      fp = fopen("WEEK2.pdf", "wb");
      file_open_checker();
      row=2;
      read_assignments_from_database();
    }
    else if(strcmp(assignment_response,"WEEK3")==0)
    {     
      fp = fopen("WEEK3.pdf", "wb");
      file_open_checker();
      row=3;
      read_assignments_from_database(); 
    }
    if(strcmp(assignment_response,"WEEK4")==0)
    {     
      fp = fopen("WEEK4.pdf", "wb");
      file_open_checker();
      row=4;
      read_assignments_from_database();
    }
  }
//----------------------------------------------------------------OPEN FILES FROM DATABASE------------------------------------------------------//
   void open_file_from_database(char file_integer[])
   {
    //printf("\n\n****************%d***********\n",file_integer);

   if (strcmp(file_integer, "Chapterone")==0)
   {
      fp = fopen("Chapterone.pdf", "wb");
      file_open_checker();
      row=1;
      read_file_from_database();
    }
    else if (strcmp(file_integer, "Chaptertwo")==0)
    {
      fp = fopen("Chaptertwo.pdf", "wb");
      file_open_checker();
      row=2;
      read_file_from_database();
    }
    else if (strcmp(file_integer, "Chapterthree")==0)
    {
      fp = fopen("Chapterthree.pdf", "wb");
      file_open_checker();
      row=3;
      read_file_from_database(); 
    }
    else
    { 
      fp = fopen("Chapterfour.pdf", "wb");
      file_open_checker();
      row=4;
      read_file_from_database();
    }
    }
   

   //---------------------------------------------------------OPEN GRADES FROM DATABASE-------------------------------------------------------//// 


void open_grades_from_database(char grade_answer[])
   {
    printf("grade answer inside open_grades_from_database=%s\n",&grade_answer);
    printf("HI in grades\n");
    if (strcmp(grade_answer,"207Grades")==0)
    {
      printf("checker.....in ...grades.....\n");
      fp = fopen("207Grades.pdf", "wb");
      file_open_checker();
      row=1;
      read_grades_from_database();
    }
    else if (strcmp(grade_answer,"209Grades")==0)
    {  
      fp = fopen("209Grades.pdf", "wb");
      file_open_checker();
      row=2;
      read_grades_from_database();
    } 
  } 
//--------------------------------------------------------------FILE OPEN CHECKER---------------------------------------------------//    

int file_open_checker()
    {
        //printf("hi\n");
    if (fp == NULL) 
        {
        fprintf(stderr, "Error opening File\n");    
        return 1;
        }
      return 0;
    }


//------------------------------------------FETCH ASSIGNMENTS FROM DATABASE AND KEEP IT IN DIRECTORY-----------------------------------//   
void read_assignments_from_database()
       {
        printf("inside read_assignments_from_database\n");
          char *sql2 = "SELECT name FROM assignments WHERE  id = ?";
       sqlite3_stmt *pStmt;
       rc = sqlite3_prepare_v2(db, sql2, -1, &pStmt, 0); // return number of bytes in BLOB file
       if (rc != SQLITE_OK ) 
          {
          fprintf(stderr, "Failed to prepare statement\n");
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          } 
        sqlite3_bind_int(pStmt,1, row); 
       rc = sqlite3_step(pStmt);
       int bytes = 0;
       if (rc == SQLITE_ROW) 
          {
          bytes = sqlite3_column_bytes(pStmt, 0);
          }
       fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);
       if (ferror(fp)) 
          {            
          fprintf(stderr, "fwrite() failed\n");
          } 
       else 
          {
            printf("File fetched from Database\n");
          } 
       int r = fclose(fp);
       if (r == EOF) 
          {
          fprintf(stderr, "Cannot close file handler\n");
          }       
       rc = sqlite3_finalize(pStmt);
       }


//------------------------------------------FETCH FILES FROM DATABASE AND KEEP IT IN DIRECTORY----------------------------------------------//
   
int read_file_from_database()
       {
       char *sql2 = "SELECT name FROM files WHERE  id = ?";
       sqlite3_stmt *pStmt;
       rc = sqlite3_prepare_v2(db, sql2, -1, &pStmt, 0);
       if (rc != SQLITE_OK ) 
          {
          fprintf(stderr, "Failed to prepare statement\n");
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          return 1;
          } 
        sqlite3_bind_int(pStmt,1, row); 
       rc = sqlite3_step(pStmt);
       int bytes = 0;
       if (rc == SQLITE_ROW) 
          {
          bytes = sqlite3_column_bytes(pStmt, 0);
          }
       fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);
       if (ferror(fp)) 
          {            
          fprintf(stderr, "fwrite() failed\n");
          return 1;      
          } 
       else 
          {
            printf("File fetched from Database\n");
          } 
       int r = fclose(fp);
       if (r == EOF) 
          {
          fprintf(stderr, "Cannot close file handler\n");
          }       
       rc = sqlite3_finalize(pStmt);
       return 0;
       }
//------------------------------------FETCH GRADES FROM DATABASE AND KEEP IT IN DIRECTORY----------------------------------------------//

int read_grades_from_database()
       {
       char *sql2 = "SELECT name FROM grades WHERE  id = ?";
       sqlite3_stmt *pStmt;
       rc = sqlite3_prepare_v2(db, sql2, -1, &pStmt, 0); // return number of bytes in BLOB file
       if (rc != SQLITE_OK ) 
          {
          fprintf(stderr, "Failed to prepare statement\n");
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          return 1;
          } 
        sqlite3_bind_int(pStmt,1, row); 
       rc = sqlite3_step(pStmt);
       int bytes = 0;
       if (rc == SQLITE_ROW) 
          {
          bytes = sqlite3_column_bytes(pStmt, 0);
          }
       fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);
       if (ferror(fp)) 
          {            
          fprintf(stderr, "fwrite() failed\n");
          return 1;      
          } 
       else 
          {
            printf("File fetched from Database\n");
          } 
       int r = fclose(fp);
       if (r == EOF) 
          {
          fprintf(stderr, "Cannot close file handler\n");
          }       
       rc = sqlite3_finalize(pStmt);
       return 0;
       }


//----------------------------------------------------------------SEND FILES TO CLIENT-----------------------------------------------//// 

int tf(int sock, char *file_name)
    {
    int count; 
    ssize_t readsize,sentsize,sentfilesize;
    char send_buf[Sender_Buff]; 
    char * error_message = "File not found\n";
    int f; //file handler
    count = 0;
    sentfilesize = 0;
    
    if( (f = open(file_name, O_RDONLY)) < 0) 
    {
    printf(file_name);
        if( (sentsize = send(client_socket, error_message , strlen(error_message), 0)) < 0 )
            {
            printf("send error");
            return -1;
            }
    }   
        else 
            {
            printf("Sending file: %s\n", file_name);
            while( (readsize = read(f, send_buf, Receive_Buff)) > 0 )
                {
                if( (sentsize = send(client_socket, send_buf, readsize, 0))< readsize )
                {
                printf("send error");
                return (-1);
                }
                count++;
                sentfilesize += sentsize;
                }
            close(f);
            } 

printf("Done with this client\n");
return count;
}
//---------------------------------------------------TRANSFER ASSIGNMENT TO CLIENT----------------------------------------------------//

int transfer_assignmentfile_to_client(int sock, char* filename)
{
  printf("Assignment %s\n", filename);
  printf("Sending File\n");
  int count; 
  ssize_t readsize,sentsize,sentfilesize;
  char send_buf[Sender_Buff]; 
  char * error_message = "File not found\n";
  int f; //file handler
  count = 0;
  sentfilesize = 0;
  

  if( (f = open(filename, O_RDONLY)) < 0) 
  {
  printf(filename);
    if( (sentsize = send(client_socket, error_message , strlen(error_message), 0)) < 0 )
      {
      printf("send error");
      return -1;
      }
  } 
    else 
      {
      printf("Sending file: %s\n", filename);
      while( (readsize = read(f, send_buf, Receive_Buff)) > 0 )
        {
        if( (sentsize = send(client_socket, send_buf, readsize, 0))< readsize )
        {
        printf("send error");
        return (-1);
        }
        count++;
        sentfilesize += sentsize;
        }
      close(f);
      } 

printf(" \n \n Done with this client\n \n " );
return count;
  
}

//---------------------------------------------------TRANSFER COURSEFILE TO CLIENT----------------------------------------------------//

int transfer_coursefile_to_client(int sock, char *filename)
{
  printf("Course %s\n", filename);
  printf("Sending File\n");
  int count; 
  ssize_t readsize,sentsize,sentfilesize;
  char send_buf[Sender_Buff]; 
  char * error_message = "File not found\n";
  int f; //file handler
  count = 0;
  sentfilesize = 0;
  

  if( (f = open(filename, O_RDONLY)) < 0) 
  {
  printf(filename);
    if( (sentsize = send(client_socket, error_message , strlen(error_message), 0)) < 0 )
      {
      printf("send error");
      return -1;
      }
  } 
    else 
      {
      printf("Sending file: %s\n", filename);
      while( (readsize = read(f, send_buf, Receive_Buff)) > 0 )
        {
        if( (sentsize = send(client_socket, send_buf, readsize, 0))< readsize )
        {
        printf("send error");
        return (-1);
        }
        count++;
        sentfilesize += sentsize;
        }
      close(f);
      } 

printf(" \n \n Done with this client\n \n " );
return count;
  
}
//---------------------------------------------------TRANSFER GRADES TO CLIENT----------------------------------------------------//

int transfer_gradefile_to_client(int sock, char *filename)
{
  printf("Grade %s\n",filename);
  printf("Sending File%s\n",filename);
  int count; 
  ssize_t readsize,sentsize,sentfilesize;
  char send_buf[Sender_Buff]; 
  char * error_message = "File not found\n";
  int f; //file handler
  count = 0;
  sentfilesize = 0;
  

  if( (f = open(filename, O_RDONLY)) < 0) 
  {
  printf(filename);
    if( (sentsize = send(client_socket, error_message , strlen(error_message), 0)) < 0 )
      {
      printf("send error");
      return -1;
      }
  } 
    else 
      {
      printf("Sending file: %s\n", filename);
      while( (readsize = read(f, send_buf, Receive_Buff)) > 0 )
        {
        if( (sentsize = send(client_socket, send_buf, readsize, 0))< readsize )
        {
        printf("send error");
        return (-1);
        }
        count++;
        sentfilesize += sentsize;
        }
      close(f);
      } 

printf(" \n \n Done with this client\n \n " );
return count;
      
}






