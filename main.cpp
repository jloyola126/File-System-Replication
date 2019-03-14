// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
//Programmers: Jonathan Loyola and Chandler Borrero
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <regex>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}
//Function for accepting fin and cin...
void twoInputs(istream& fin){     
  
  //REGEX EXPRESSIONS...
  regex empty {R"(^\s*?$)"};
  regex hashSign {R"(#.*)"};
  regex keyEqualsValue {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
  regex keyItself {R"(^\s*(.*)\s*?$)"};
  regex keyEquals{R"(^\s*(.*)\s*=\s*$)"};
  regex equalsSign{R"(^\s*[=]\s*$)"};
  regex equalsValue {R"(^\s*[=]\s*(.*?)\s*$)"};
  str_str_map test; //using the str_str_map that we wrote...
  str_str_pair p; //using the str_str_pair that we wrote...
  string s; //string s for input...
  string fileN; //string filename...
  smatch result; //smatch to refer to key and values....
  int counter = 0; //counter for line execution numbering....
  while(getline(fin,s))
      {
        if(regex_search(s,result,empty))
        {
          //outputs nothing important--
           cout << fileN << ": " << ++counter << ": " << endl;
           continue; //ignore and continue to the next iteration... 
        }
        if(regex_search(s,result,hashSign))
        {
          //ignore input...
          cout << fileN << ": " << ++counter << ": " << endl;
          continue;
        }
        if(regex_search(s,result,equalsSign)) //if regex is =, print out all keys and values...
        {
          cout << fileN << ": " << ++counter << ": " << endl;
          for (str_str_map::iterator iter = test.begin();iter != test.end();++iter)
          {
            cout << *iter;
          }
          continue;
        }
        if(regex_search(s,result,equalsValue)) //if regex is just the = value, print according to value...
        {
          for (str_str_map::iterator iter = test.begin();iter != test.end();++iter)
          {
            if(iter->second == result[1])
            {
              cout << *iter;
            }
          }
          continue;
        }
        if(regex_search(s,result,keyEquals)) //if regex is key = with an empty value...
        {
          cout << fileN << ": " << ++counter << ": " << endl;
          str_str_map::iterator iter = test.find(result[1]);
          if(iter != test.end())
            test.erase(iter);
        }
        if(regex_search(s,result,keyEqualsValue)) //if regex is key = value...
        {
          cout << fileN << ": " << ++counter << ": " << endl;
          str_str_pair p(result[1], result[2]);
          test.insert(p);
          cout << p << endl;
          continue;  
        }
        if(s == "Quit") break; //Break from loop and exit...
        if(regex_search(s,result,keyItself)) //if regex is just the key with no = value...
        {
          cout << fileN << ": " << ++counter << ": " << endl;
          str_str_map::iterator iter = test.find(result[1]);
          if(iter == test.end())
            cout << result[1] << ": key was not found" << endl;
          else
            cout << *iter << endl;
          continue;
        }
      }
    }//file.close();

int main (int argc, char** argv) 
{
  string f = "-";
  if (argc == 1) twoInputs(cin); //call cin...
  
  //else determine if we need to use cin or fin...
  for(int i = 1; i < argc; i++)
  {
    if(f == "-")
    {
      twoInputs(cin); //modular function added to accept cin or fin...
    }
    else
    {
      //create a file and initiliaze it to the default string "-"
      ifstream fileTester(f);
      if(fileTester)  //if fileTester is in a working state...
      {
        twoInputs(fileTester); //modular function added to accept cin or fin...
      }
      else
        cout << "Oh Oh, no such file or directory!" << endl;
    }
  }
  sys_info::execname (argv[0]);
  scan_options (argc, argv);
 
  cout << "EXIT_SUCCESS" << endl;
  return EXIT_SUCCESS;
}
