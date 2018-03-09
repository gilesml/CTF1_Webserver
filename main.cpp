#include "webserver.h"
#include "Socket.h"

void Request_Handler(webserver::http_request* r) {
  Socket s = *(r->s_);

  //do we want our response in the form of a webpage? Is that even what this does?
  std::string title;
  std::string body;
  std::string bgcolor="#ffffff";
  std::string links =
      "<p><a href='/red'>red</a> "
      "<br><a href='/blue'>blue</a> "
      "<br><a href='/form'>form</a> "
      "<br><a href='/auth'>authentication example</a> [use <b>rene</b> as username and <b>secretGarden</b> as password"
      "<br><a href='/header'>show some HTTP header details</a> "
      ;
/*
  if(r->path_ == "/") {
    title = "Web Server Home";
    body  = "<h1>Welcome to Rene's Web Server</h1>"
            "I wonder what you're going to click"  + links;
  }
*/
  if(r->path == "www.blue.net/register"){
        //Create a new user with the username r->params_[]("user") and the password r->params_[]("pass")
  }else if (r->path == "www.blue.net/login"){
        bool validUserPass = false;
        //Authenticate the username r->params_[]("user") and the password r->params_[]("pass")
        if(validUserPass){
            //set a cookie for this user
            title = "login successful";
            body = "Your login attempt was successful";
        }else{
            title = "invalid username/password";
            body = "the username or password given does match our records";
        }
  }else if (r->path == "www.blue.net/manage"){
        bool loggedIn = false;
        //Check if they have a cookie
        if(!loggedIn){
            title = "NO";
            body = "You're not logged in";
        }else{
            string action = r->params_[]("action");
            if(action == "deposit"){

            }else if (action == "withdraw"){

            }else if (action == "balance"){

            }else if (action == "close"){

            }
        }

  }else if (r->path == "www.blue.net/logout"){
        //Destroy the cookie
  }else {
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
  webserver(8080, Request_Handler);
}
