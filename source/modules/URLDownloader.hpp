/*
 * Copyright 2021-2022 João F. BeyondMagic <koetemagie@gmail.com>
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 */

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

/*
 * Definitions..
 */

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
