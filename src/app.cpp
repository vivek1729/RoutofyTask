#include "../include/redispp.h"
#include "../include/curl.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace redispp;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
    return buf;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  Connection conn("localhost", "6379", "");
  string from,to,readBuffer;
  char choice;
  CURL *curl;
  CURLcode res;
  StringReply repl;
  do{
  //Take user input
  	cout << "Enter from city: \n";
  	cin>> from;
  	cout << "Enter to city: \n";
  	cin>> to;
  //Sanitize case of the string. Convert to lower case for consistency
  	transform(from.begin(), from.end(), from.begin(), ::tolower);
  	transform(to.begin(), to.end(), to.begin(), ::tolower);
  	string key = from+"-"+to;
  try
  {
    	repl = conn.get(key);
    	string reply = repl;
    	cout<<reply<<endl;   
  }
  catch (NullReplyException e)
  {
    	cout << "Key doesn't exist on the database. Crawling from web........... \n";
    	string url = "http://www.travelyaari.com/api/search/?mode=oneway&departDate="+currentDateTime()+"&fromCity="+from+"&toCity="+to+"&pickups=1&_=1433568200541";
  
  //Initialize curl request 
	curl = curl_easy_init();
  	if(curl) {
		readBuffer = "";
    		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    		res = curl_easy_perform(curl);
    		curl_easy_cleanup(curl);
  //Print helper message to show info crawled from web successfully
    		cout<< "Updating the database..... \n";
  //Had to resize the output string as I got some buffer overflow error with this particular redis client
  //(https://github.com/brianwatling/redispp/issues/5)
    		readBuffer.resize(1000);
    		conn.set(key, readBuffer);
    		cout<<readBuffer<<endl;
  		} 
  
  	}
   	cout<< "\n\nPress (N/n) to exit and any other key to continue...\n";
   	cin >> choice;
  }while(choice != 'n' && choice != 'N');
  
  return 0;
}
