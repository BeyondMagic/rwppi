#include <curl/curl.h>
#include <iostream>

/*
 * Finnally, this class will be the way to download our page and return its source code so that we can send it to the parser.
 * This is using cURL currently, though I don't see a better future ahead if it is not this minimal library that can be compiled
 * in almost every system out there. Be happy with the ugliness it provides because of its benefits, it is fast!
 */
class URLDownloader
{

  private:
    std::string response;
    std::string language;
    char * query;
    CURL * curl;

    // The function to write the source code form cURL. It will just be used in one place.
    static size_t
    write ( void* ptr, size_t size, size_t nmemb, std::string* data)
    {

      data->append( (char*)ptr, size * nmemb );
      return size * nmemb;

    }

  public:

    // Initialise cURL.
    URLDownloader ( const std::string &, std::string & );

    // Where we set the URL.
    std::string &
    getURL ( std::string & ) const;

    // Use cURL to get the source code based on the type of source (std::string), such as "Google" or "Wikipedia".
    // This will return a std::string. If it doesn't find a source that matches one of the list, then it will return nothing.
    // So don't be dumb and send a source here to that doesn't exist at all.
    const std::string & download( std::string & );

    // Destroy the cURL method, after this the method will become unusable.
    void clean( void ) const;

};
