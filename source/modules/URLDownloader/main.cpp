#include "main.hpp"

// #cwos -> math

URLDownloader::URLDownloader ( const std::string & lang, std::string & text )
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

    std::cout << "[URLDownloader] cURL was not found." << std::endl;
    exit(1);

  }

}

void
URLDownloader::clean ( void ) const
{
  // Clean cURL method
  curl_easy_cleanup( curl );
  curl_global_cleanup();
}

std::string &
URLDownloader::getURL ( std::string & name ) const
{

  // To use if no source is found in the following list.
  bool no_sources = false;

  // There really isn't a lot of alternatives to this, so don't @ me.
       if (name == "Google")     name = "https://www.google.com/search?hl="
                                        + language + "&q=" + query;
  else if (name == "LetraMusBr") name = "https://" + language.substr(0, 2)
                                        + ".wikipedia.org/w/index.php?search=" + query;

  // If there isn't a source with that name.
  else {

    std::cout << "[URLDownloader] EE: There is no source URL for that name: " << name << std::endl;
    this->clean();
    exit(1);

  };

  // If there is.
  return name;

}

const std::string &
URLDownloader::download( std::string & source )
{

  source = this->getURL(source);

  std::cout << "[URLDownloader] URL to download: " << source << "\n";

  // Set up the URL
  curl_easy_setopt( curl, CURLOPT_URL, source.c_str() );

  // Run our cURL method
  curl_easy_perform( curl );

  return response;

}
