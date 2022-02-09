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

#pragma once

#include <curl/curl.h>
#include <iostream>

/*
 * Finnally, this class will be the way to download our page and return its source code so that we can send it to the parser.
 * This is using cURL currently, though I don't see a better future ahead if it is not this minimal library that can be compiled
 * in almost every system out there. Be happy with the ugliness it provides because of its benefits, it is fast!
 */
class URLDownloader
{

  public:

    // Initialise cURL.
    URLDownloader ( const std::string &, std::string & );

    ~URLDownloader ();

    // Use cURL to get the source code based on the type of source (std::string), such as "Google" or "Wikipedia".
    const std::string download( int & );

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

};
