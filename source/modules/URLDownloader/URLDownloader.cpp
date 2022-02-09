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

#include "URLDownloader.hpp"

URLDownloader::URLDownloader ( const std::string & lang, std::string & text )
{

  language = lang;
  curl = curl_easy_init();

  if (curl)
  {
    // Encode the string into URL type, like " " to "%20".
    query = curl_easy_escape(curl, text.c_str(), 0);

    // Set up the options for cURL
    curl_easy_setopt( curl, CURLOPT_USERAGENT,
                      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/5.15.2 Chrome/87.0.4280.144 Safari/537.36" );
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

URLDownloader::~URLDownloader ()
{
  // Clean cURL method
  curl_easy_cleanup( curl );
  curl_global_cleanup();
}

const std::string
URLDownloader::download( int & id )
{

  std::string source;

  // Set URL based on the ID.
  switch (id) {

    // Google.
    case 0:
      source = "https://www.google.com/search?hl=" + language + "&q=" + query;
      break;

    default:
      std::cout << "[URLDownloader] EE: There is no source URL for that ID: " << id << std::endl;
      exit(1);
      break;

  }

  // Set up the URL
  curl_easy_setopt( curl, CURLOPT_URL, source.c_str() );

  // Run our cURL method
  curl_easy_perform( curl );

  return response;

}
