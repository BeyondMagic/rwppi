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

#include <string>
#include <iostream>
#include <future>
#include <cmath>
#include <fstream>
#include <sstream>
#include "tinyexpr/main.h"
//#include "HTMLParser.hpp"

#define RESPONSE(RESPONSE,METHOD) \
  std::cout << METHOD << " " << RESPONSE << "\n"; \
    response_found = true;

/*
 * Anything that can be computed locally needs to be put in here, all of those functions will run not matter the query.
 * So be careful with the speed of it.
 */
struct MethodLocal
{

  /* Return the value of an expression, it will returns the first value it encounters.
   * It will simply return NaN for invalid expressions.
   */
  void
  math( const std::string & );

  /* Run all functions initialised above.
   */
  const bool
  all( const std::string & );

};

/*
 * Scrapers for a web-page.
 */
struct MethodRemote
{

  /*
   * Follow the order of Util::print_methods as listed in there.
   */

  void
  google_math( const std::string & );

  void
  google_definition( const std::string & );

  /* Return the value of an expression using the Google engine. */
  void
  google_lyrics( const std::string & );

  void
  google_translation( const std::string & );

  // Weather            -> current weather
  // Tracklist          -> noisia outer edges tracklist
  // InformationList    -> social network cast
  // HeaderList         -> Need for Speed Heat cars list
  // Chemistry          -> density of hydrogen
  // Pronunciation      -> pronounce linux
  // UnitConversion     -> 1m into 1cm
  // CurrencyConversion -> 1 USD in rupee
  // InformationHeader  -> christmas day
  // InformationWindow  -> who is garfield
  // QuotesList         -> mahatma gandhi quotes
  // TableSport         -> Chelsea next game
  // InformationTable   -> the office

  /* Compiled methods for certain pages.
   */
  const bool
  google( const std::string & );

};

/*
 * Definitions...
 */

bool response_found = false;

/*
 * Local
 */

void
MethodLocal::math( const std::string & expression )
{

  const double answer = te_interp(expression.c_str(), 0);

  if (!std::isnan(answer))
  {
    RESPONSE(answer, "LocalMath");
  }

}

const bool
MethodLocal::all( const std::string & expression )
{

  // Local methods call-in.
  auto math_handler = std::async(&MethodLocal::math, this, expression);

  // Local methods call-out.
  math_handler.get();

  return response_found;

}

/*
 * Remote.
 */

void
MethodRemote::google_lyrics( const std::string & page )
{

  RESPONSE("found a nice lyrics response", "GoogleLyrics");

}

void
MethodRemote::google_math( const std::string & page )
{

  RESPONSE("found a nice math response", "GoogleMath");

}

/*
 * Compiled.
 */

const bool
MethodRemote::google( const std::string & page )
{

  // Local methods call-in.
  auto math_handler   = std::async(&MethodRemote::google_math,   this, page);
  auto lyrics_handler = std::async(&MethodRemote::google_lyrics, this, page);

  // Local methods call-out.
  math_handler.get();
  lyrics_handler.get();

  return response_found;

}
