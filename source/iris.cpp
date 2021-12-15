#include <stdio.h>     // For everythinkg
#include <regex>       // Replace and find
#include <string>      // String methods'
#include <iostream>    // stdout
#include <algorithm>
#include "methods.cpp" // To minimaze code used on the main.

int main( const int argc, char** argv )
{

  // Default language parameter for search set by environment variable LANG.
  const std::string language = getenv("LANG") ? std::string( getenv("LANG") ).substr(0, 5) : "en_US";

  std::string sources; // Can be modified by options to be "Google".
  std::string page;    // Source page.
  std::string query;   // The query which the user sends as argument.

  bool local   = true;  // If we'll just ignore all local functions and work remotelly. 
  bool remote  = true;  // If we'll just ignore all local functions.
  bool literal = false; // No parsing from cURL to the query.
  bool log     = false; // Every "print_iris" function will be used.

  InputParser arguments(argc, argv);

  // NOTE: Parse options and parameters.
  {

    // Options that don't accept arguments:
    // -a
    // --animal
    {
      if ( arguments.exists("-h", "--help") ) return print_help();
      if ( arguments.exists("-l", "--log") )       log      = true;
      if ( arguments.exists("-R", "--no-remote") ) remote   = false;
      if ( arguments.exists("-L", "--no-local") )  local    = false;
      if ( arguments.exists("--", "--LITERAL") )   literal  = false;
    }

    // Options that accept arguments:
    // -f example.txt
    // --filename example.txt
    {
                             sources = arguments.get( "-s", "--source" );
      if ( sources.empty() ) sources = "Google";
    }

    // Log everything we set.
    {
      print_iris("Setting defaults...");
      print_iris("Language set:",  language);
      print_iris("Sources set:",   sources);
    }

  }

  // NOTE: Download sources' pages and already do everything.
  {

    query = trim_arguments( query, arguments.left() );

    if (log) print_iris("Your query is exactly:", query);

    SourcePage source(language, query);

    source.render(sources);

    source.clean();


  }

  /* Asynchoneous loop to use the query to get information
   *
   */

}
