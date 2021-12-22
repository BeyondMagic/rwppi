#include <curl/curl.h>
#include <iostream>

/* 
 * Finnally, this class will be the way to download our page and return its source code so that we can send it to the parser.
 * This is using cURL currently, though I don't see a better future ahead if it is not this minimal library that can be compiled
 * in almost every system out there. Be happy with the ugliness it provides because of its benefits, it is fast!
 */
class SourcePage
{

  private:
    std::string response;
    std::string language;
    char * query;
    CURL * curl;

  public:

    SourcePage ( std::string lang, std::string & text )
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

    bool render( std::string sources )
    {

      // The sources' string-option are split by ",". We may modify this latter for better syntax, but for now, it does what it is supposed to do.
      const std::string delimiter = ","; 

      // Loop through each source to download
      size_t pos = 0;
      while ( (pos = sources.find(delimiter)) != std::string::npos ) {

        // Current source on the loop
        const std::string source = sources.substr(0, pos);
        sources.erase(0, pos + delimiter.length());

        // Run download to get our little page :)
        std::cout << this->download(source) << std::endl;

      }; std::cout << this->download(sources) << std::endl;

      // If it doesn't find anything.
      return false;

    }

    // Use cURL to get the source code based on the type of source (std::string), such as "Google" or "Wikipedia".
    // This will return a std::string. If it doesn't find a source that matches one of the list, then it will return nothing.
    // So don't be dumb and send a source here to that doesn't exist at all.
    const std::string & download( std::string source )
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
    static size_t write( void* ptr, size_t size, size_t nmemb, std::string* data )
    {
 
      data->append( (char*)ptr, size * nmemb );
      return size * nmemb;

    }
};

