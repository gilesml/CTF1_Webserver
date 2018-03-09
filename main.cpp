// some code taken from http://www.cplusplus.com/forum/general/71099/ for database stuff

#include <string.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "webserver.h"
#include "Socket.h"

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet  *res;

void Request_Handler(webserver::http_request* r) {
  Socket s = *(r->s_);

  //do we want our response in the form of a webpage? Is that even what this does?
  std::string title;
  std::string body;

  //Check if there were sql injections
  if(stoi(params_[]("injections"))!=0){
        title = "Unable to process request.";
        body = "Cookies and request parameters may not contain 'DROP', ')', or ';'. Please try again." + bal;
  }
  //register a player account
  if(r->path == "www.blue.net/register"){
        stmt->execute("INSERT INTO bank.accounts(user, pass, balance) VALUES (" + username r->params_[]("user") + ", " + password r->params_[]("pass") + ", 0)");
        title = "success";
        body = "Your account was successfully created";
  }
  //login and set a cookie
  else if (r->path == "www.blue.net/login"){
        bool validUserPass = false;
        res = stmt->executeQuery("SELECT * FROM bank.accounts WHERE user = '"+r->params_[]("user")) + "')";
        if(res->getString("pass")==r->params_[]("pass")) validUserPass = true;

        if(validUserPass){
            int sessionID = rand();
            stmt->execute("INSERT INTO bank.cookies(user, session) VALUES (" + username r->params_[]("user") + ", " + sessionID + ")");
            //TODO: set a cookie for this user
            title = "login successful";
            body = "Your login attempt was successful";
        }else{
            title = "invalid username/password";
            body = "the username or password given does match our records";
        }
  }
  //manage account
  else if (r->path == "www.blue.net/manage"){
        //TODO: Get the cookie as a string to compare it
        string cookie;
        int numInj = 0;
        std::string::size_type inject = cookie.find(")");
        if(inject != std::string::npos)numInj++;
        inject = cookie.find(";");
        if(inject != std::string::npos)numInj++;
        inject = cookie.find("DROP");
        if(inject != std::string::npos)numInj++;
        if(numInj>0){
            title = "Unable to process request.";
            body = "Cookies and request parameters may not contain 'DROP', ')', or ';'. Please try again." + bal;
        }else{
            bool loggedIn = false;
            res = stmt->executeQuery("SELECT * FROM bank.cookies WHERE session="+cookie+")");
            while(res->next()){
                if(res->getString("session")==stoi(cookie)){
                    loggedIn = true;
                    break;
                }
            }
            string username = res->getString("user");
            res = stmt->executeQuery("SELECT * FROM bank.accounts WHERE session="+username+")");

            if(!loggedIn){
                title = "Invalid Request";
                body = "You're either not logged in or your request featured invalid input";
            }else{
                string action = r->params_[]("action");
                if(action == "deposit"){
                    int bal = res->getString("balance");
                    bal += r->params_[]("amount");
                    title = "deposit successful";
                    stmt->executeQuery("UPDATE bank.accounts SET balance="+bal+" WHERE user=" + username+")");
                    body = "your balance is now $" + bal;
                }else if (action == "withdraw"){
                    int bal = res->getString("balance");
                    bal -= r->params_[]("amount");
                    title = "withdrawal successful";
                    stmt->executeQuery("UPDATE bank.accounts SET balance="+bal+" WHERE user=" + username+")");
                    body = "your balance is now $" + bal;
                }else if (action == "balance"){
                    int bal = res->getString("balance");
                    title = "Balance Check";
                    body = "your balance is $" + bal;
                }else if (action == "close"){
                    title = "Account Closed";
                    body = "your account has been deleted" + bal;
                    stmt->executeQuery("DELETE FROM bank.account WHERE user=" + username + ")");
                }
            }
        }
  }
  //Logout of account
  else if (r->path == "www.blue.net/logout"){
        //TODO: need to find the cookie
        string cookie;
        string cookie;
        int numInj = 0;
        std::string::size_type inject = cookie.find(")");
        if(inject != std::string::npos)numInj++;
        inject = cookie.find(";");
        if(inject != std::string::npos)numInj++;
        inject = cookie.find("DROP");
        if(inject != std::string::npos)numInj++;
        if(numInj>0){
            title = "Unable to process request.";
            body = "Cookies and request parameters may not contain 'DROP', ')', or ';'. Please try again." + bal;
        }else{
            title = "Session Closed";
            body = "your cookie is no longer active" + bal;
            stmt->executeQuery("DELETE FROM bank.cookies WHERE session=" + cookie + ")");
        }
  }
  //unidentified request
  else {
    r->status_ = "404 Not Found";
    title      = "Wrong URL";
    body       = "<h1>Wrong URL</h1>";
    body      += "Path is : &gt;" + r->path_ + "&lt;";
  }

  r->answer_  = "<html><head><title>";
  r->answer_ += title;
  r->answer_ += "</title></head><body bgcolor='" + bgcolor + "'>";
  r->answer_ += body;
  r->answer_ += "</body></html>";
}

int main() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "user", "password");
    stmt = con->createStatement();
    stmt->execute("USE bank");
    stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");

    srand(time(NULL));

	//Connecting to the database
	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		return -1;
	}

    //Running the web server
    webserver(8080, Request_Handler);

    // close connection
    delete stmt;
    delete con;
    delete res;

	return 0;
}
