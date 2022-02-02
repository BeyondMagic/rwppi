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

extern bool response_found;

typedef struct {

  int i;
  std::string method;

} context_t;

class MethodRemote
{

  public:

    MethodRemote( const std::string & );

    void
    clean( void );

    void
    Google_All();

    void
    Google_Math();

    void
    Google_Lyrics();

    void
    Google_LyricsInfo();

    void
    Google_Definition();

    void
    Google_Translation();

    void
    Google_Weather();

    void
    Google_TrackList();

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

};

class MethodLocal
{

  private:
    std::string expression;

  public:

    MethodLocal( const std::string & );

    void
    Math();

    void
    All();

};
