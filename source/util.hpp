#include <string>      // String methods'
#include <iostream>    // stdout
#include <curl/curl.h> // Download a page's source
#include <vector>
#include <algorithm>

using namespace std;   // Less redudancy of code.

/*
 * Functions rather marvelous to remove the necessity of repeating so many times the same words in the same way.
 *  void print_iris( string )         -> Will print to the stderr in the order of the given strings.
 *  void print_iris( string, string ) -> Will print to the stderr in the order of the given strings.
 * I don't think there's a way to minimize this anymore, so for now, it is what it is.
 */

void print_iris( string our_string )
{

  cout << "[iris] " << our_string << endl;

}

void print_iris( string first_string, string second_string )
{

  cout << "[iris] " << first_string << " " << "\"" << second_string << "\"" << endl;

}

/*
 * This is basically our library-free code to parse our arguments, it is obviously not feature full for the same purporse
 * we created this. Basically, we use it so that we don't have to compile a 1MiB header file to just parse our options.
 * Please, future self or future maintainers, do only replace this if we have a minimal library that can actually do this better.
 * 
 * TODO: Find a smart way to parse multiple inline commands such as '-abcdef'.
 */
class InputParser
{
  private:
    vector <string> tokens;

  public:

    // This obviously will take the arguments of our main() function, so just parse them to this.
    InputParser ( const int & argc, char **argv )
    {
      // Loop through each argument and add it to the tokens private variable.
      for ( int i = 1; i < argc; ++i )
      {
        this->tokens.push_back( string(argv[i]) );
      }
    }

    // This will return the value after the option, so it verifies two strings.
    // It doesn't matter how you call it, but for both strings it will verify and take the next argument.
    // It will actually remove from the leftovers both the strings even if it just took the first one.
    // Original author is @iain, though it is modified for our needs.
    const string get( const string & short_name, const string & long_name )
    {
      vector <string>::const_iterator itr;

      // Less redudancy of code.
      #define GET_STRING(A)		\
          itr = find( this -> tokens.begin(), this -> tokens.end(), A );		\
          remove(this->tokens.begin(), this->tokens.end(), A);		\
          if ( itr != this->tokens.end() && ++itr != this->tokens.end() )

      GET_STRING(short_name) {
        const string parameter = *--itr;
        remove(this->tokens.begin(), this->tokens.end(), parameter);
        remove(this->tokens.begin(), this->tokens.end(), long_name); // Remove the next string as well.
        return parameter;
      }

      GET_STRING(long_name) {
        const string parameter = *--itr;
        remove(this->tokens.begin(), this->tokens.end(), parameter);
        return parameter;
      }

      // Return nothing if it doesn't find.
      static const string empty_string("");
      return empty_string;
    }

    // This will return the rest of our arguments after all the calls are made, such as this->get or this->exist.
    // Use this will great knowledge about what you're doing.
    inline vector <string> left () const
    {

      return tokens;

    }

    // To return true if an option was passed to the arguments. It will remove from this->tokens the strings it finds.
    // short_name : -a
    // long_name  : --animal
    bool exists( const string & short_name, const string & long_name )
    {

      // Less redudancy of code.
      #define EXIST_STRING(A) ( find( this->tokens.begin(), this->tokens.end(), A ) != this->tokens.end() )

      // Loop through each option to see if it exists there.
      if ( EXIST_STRING(short_name) || EXIST_STRING(long_name) ) {

        remove(this->tokens.begin(), this->tokens.end(), short_name);
        remove(this->tokens.begin(), this->tokens.end(), long_name);

        return true;

      };

      return false;

    }
};

/* 
 * Finnally, this class will be the way to download our page and return its source code so that we can send it to the parser.
 * This is using cURL currently, though I don't see a better future ahead if it is not this minimal library that can be compiled
 * in almost every system out there. Be happy with the ugliness it provides because of its benefits, it is fast!
 */
class SourcePage
{

  private:
    string response;
    string language;
    char * query;
    CURL * curl;

  public:

    SourcePage ( string lang, string & text )
    {

      language = lang;
      curl = curl_easy_init();

      if (curl)
      {
        // Encode the string into URL type, like " " to "%20".
        query = curl_easy_escape(curl, text.c_str(), 0);

        // Set up the options for cURL
        curl_easy_setopt( curl, CURLOPT_USERAGENT,     "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:53.0) Gecko/20100101 Firefox/53.0" );
        curl_easy_setopt( curl, CURLOPT_NOPROGRESS,    1L );
        curl_easy_setopt( curl, CURLOPT_MAXREDIRS,     50L );
        curl_easy_setopt( curl, CURLOPT_TCP_KEEPALIVE, 1L );
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, this->write );
        curl_easy_setopt( curl, CURLOPT_WRITEDATA,     & response );
        //curl_easy_setopt( curl, CURLOPT_HEADERDATA,    & header  );

      }
      else
      {

        print_iris("[util] curl was not found.");
        exit(1);

      }

    }

    // Destroy the cURL method, after this the method will become unusable.
    void clean( void ) const
    {
      // Clean cURL method
      curl_easy_cleanup( curl );
      curl_global_cleanup();
    }

    bool render( string sources )
    {

      // The sources' string-option are split by ",". We may modify this latter for better syntax, but for now, it does what it is supposed to do.
      const string delimiter = ","; 

      // Loop through each source to download
      size_t pos = 0;
      while ( (pos = sources.find(delimiter)) != string::npos ) {

        // Current source on the loop
        const string source = sources.substr(0, pos);
        sources.erase(0, pos + delimiter.length());

        // Run download to get our little page :)
        cout << this->download(source) << endl;

      }; cout << this->download(sources) << endl;

      // If it doesn't find anything.
      return false;

    }

    // Use cURL to get the source code based on the type of source (std::string), such as "Google" or "Wikipedia".
    // This will return a std::string. If it doesn't find a source that matches one of the list, then it will return nothing.
    // So don't be dumb and send a source here to that doesn't exist at all.
    const string & download( string source )
    {

      // To use if no source is found in the following list.
      bool no_sources = false;

      // There really isn't a lot of things that we can do improve this.
           if (source == "Google")    source = "https://www.google.com/search?hl="
                                             + language + "&q=" + query;
      else if (source == "Wikipedia") source = "https://" + language.substr(0, 2)
                                             + ".wikipedia.org/w/index.php?search=" + query;

      else no_sources = true;

      if ( !no_sources ) {

        print_iris( "[util] URL to download:", source );

        // Set up the URL
        curl_easy_setopt( curl, CURLOPT_URL, source.c_str() );

        // Run our cURL method
        curl_easy_perform( curl );

        return response;

      }
      else
      {
        print_iris("[util] No URL was found for:", source);
        this->clean();
        exit(1);
      }

    }

    // The function to write the source code form cURL. It will just be used in one place.
    static size_t write( void* ptr, size_t size, size_t nmemb, string* data )
    {
 
      data->append( (char*)ptr, size * nmemb );
      return size * nmemb;

    }
};
 
string trim_arguments( string & s, const vector <string> left ) {

  // Add all the arguments from the vector into a string.
  for (string word : left)
  {
    s += " " + word;
  }

  // Find trailing characters from the left side and remove them.
  size_t start = s.find_first_not_of(" \n\r\t\f\v");
  s =  (start == string::npos) ? "" : s.substr(start);

  // Find trailing characters from the right side and remove them.
  size_t end = s.find_last_not_of(" \n\r\t\f\v");
  s = (end == string::npos) ? "" : s.substr(0, end + 1);

  return s;

}

const unsigned int print_help( void ) {

  printf(R"(iris - Version: 0.0.1

DESCRIPTION:
  iris is a CLI-assistant that receives any type of input and tries to find an answer for it.
  It strives to have a simple codebase while being extremely performant and lightweight.

SOURCE-CODE:
  https://github.com/iris-cli/iris

MAINTAINER: (s):
  2021-2021 -> João F. BeyondMagic <koetemagie@gmail.com>

USAGE:
  iris [--help|-h] [[--no-remote|-R]|[--no-local|-L]] [--log|-l] [--all|-a] [--source|-s sources]

  --help | -h
    Print out this and immediately exit the program.

  --log | -l
    Print out all logging information of the program, this is preferably for debugging to
    contributors of the program.

  --all-answers | -a
    Run every single method while trying to find an answer for the query.

  --all-sources | -A
    Run every single source while trying to find an answer for the query. This can take a long time
    compared to the normal run.

  --LITERAL | --
    Query will be literal, it won't parse it into URL enconding. Useful if you have a encoded query already.

  --source source1,source2,... | -s source1,source2,...
    Read respectively each source and tries to find and answer on it. If it doesn't recognize by
    name, it will ignore and try to look at the next one. For the first answer it finds, it will
    stop -- if the option '--all' is passed, it will read all of the sources and print out any
    answers it finds.

    Sources can be any of the following:
      Google      -> https://www.google.com/
      Wikipedia   -> https://??.wikipedia.org/ (Supported languages by Wikipedia)
      LetrasMusBr -> https://www.letras.mus.br/

EXAMPLES:
  iris '4+(62+(31/2))+sin(0.78)'

  iris 'What is the meaning of love?'

  irisi 'what Linux is?'

  iris --sources Wikipedia,Google 'Hiragana'

CONTRIBUTE:
  https://github.com/iris-cli/iris/issues

LICENSE:
  MIT
)");

  return 0;

}
