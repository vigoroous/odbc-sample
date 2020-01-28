#include <windows.h>
#include <iostream>
#include <sqlext.h>



int main() {
    setlocale(LC_ALL, "Russian");
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;
    SQLRETURN retcode;
    
    // Allocate environment handle  
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    std::cout << retcode << '\n';

    // Allocate connection handle  
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
        std::cout << retcode << '\n';

        // Allocate connection handle  
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
            std::cout << retcode << '\n';


            
            char ConnStrIn[] = "FILEDSN=E:/ProjectsCPP/SQLmanager/SQLmanager/my.dsn;";
            char            ConnStrOut[200];
            SQLSMALLINT     cbConnStrOut;
            retcode = SQLDriverConnectA(hdbc,NULL, (SQLCHAR*)ConnStrIn, strlen(ConnStrIn), (SQLCHAR*)ConnStrOut, sizeof(ConnStrOut), &cbConnStrOut, SQL_DRIVER_NOPROMPT);
            //retcode = SQLDriverConnectA(hdbc,GetDesktopWindow(), (SQLCHAR*)"driver = SQL Server", SQL_NTS,NULL,0,NULL,SQL_DRIVER_PROMPT);
            
            std::cout << retcode << '\n';

            SQLINTEGER NativeErrorPtr;
            SQLCHAR* MessageText = (SQLCHAR*)malloc(256);
            SQLCHAR* err = (SQLCHAR*)malloc(6);
            //SQLWCHAR MessageText[256];
            SQLSMALLINT TextLengthPtr;
            //SQLWCHAR err[6];
            retcode = SQLGetDiagRecA(SQL_HANDLE_DBC, hdbc, 1, err, &NativeErrorPtr, MessageText, 256, &TextLengthPtr);
            std::cout << MessageText << '\n';
            std::cout << err << '\n';
            std::cout << NativeErrorPtr << '\n';
            //SQLGetDiagRecA(hdbc);
            free(MessageText);
            free(err);

            
            

            // Allocate statement handle  
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
                std::cout << retcode << '\n';
                SQLCHAR SqlCommand[] = "if not exists(select * from sys.tables where name like 'Test') create table Test(Personid int IDENTITY(1, 1) PRIMARY KEY,  LastName varchar(255), FirstName varchar(255), Age int); ";
                SQLExecDirectA(hstmt, SqlCommand, SQL_NTS);
                std::cout << retcode << '\n';
                // Process data  
                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                }

                SQLDisconnect(hdbc);
            }

            SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        }
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }
}