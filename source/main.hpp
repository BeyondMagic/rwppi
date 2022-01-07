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

#include <iostream>

/*
 * Print the help list of Iris; should always be updated in case of major changes.
 * Will always return 0. 
 */
const unsigned int
print_help( void )
{

  std::cout << R"(iris - Version: 0.0.1

DESCRIPTION:
  iris is a CLI-assistant that receives any type of input and tries to find an answer for it.
  It strives to have a simple codebase while being extremely performant and lightweight.

USAGE:
  iris [--help] [--unit 0-6] [--source sources]

  --help | -h
    Print out this and immediately exit the program.

  --unit | -u
    Print out all logging information of an unit, this is preferably for debugging to
    contributors of the program.

  --source source1,source2,... | -s source1,source2,...
    Read respectively each source and tries to find and answer on it. If it doesn't recognize by
    name, it will ignore and try to look at the next one. For the first answer it finds, it will
    stop.

MAINTAINER: (s):
  2021-2021 João F. BeyondMagic <koetemagie@gmail.com>

SOURCE-CODE:
  https://github.com/iris-cli/iris

ISSUES:
  https://github.com/iris-cli/iris/issues

CONTRIBUTE:
  https://github.com/iris-cli/iris/blob/master/CONTRIBUTING.md

LICENSE:
  APACHE 2.0
)" << std::endl;

  return 0;

}

/*
 * Print the list of sources that can be used by Iris currently.
 * Will always return 0.
 */
const unsigned int
print_sources ( void )
{

  std::cout << R"(Sources can be any of the following:
  Google      -> https://www.google.com/
  LetrasMusBr -> https://www.letras.mus.br/

)" << std::endl;

  return 0;
}

/*
 * Print the list of methods that can be used by Iris currently.
 * Will always return 0.
 */
const unsigned int
print_methods ( void )
{

  std::cout << R"(Local will always run unless source is specified.

  Local:
    MathExpression     -> 4+6/3+27+sqrt(25)

All remote methods will run by source specification, emphasis on all
since it will scrape in a asynchronous manner for performance.

  Google:
    Math               -> log(26) + 7/3
    Definition         -> meaning of love
    Lyrics             -> K.Flay - Maybe There's A Way lyrics
    Translation        -> palavra fazer em português para japonês
    Weather            -> current weather
    Tracklist          -> noisia outer edges tracklist
    InformationList    -> social network cast
    HeaderList         -> Need for Speed Heat cars list
    Chemistry          -> density of hydrogen
    Pronunciation      -> pronounce linux
    UnitConversion     -> 1m into 1cm
    CurrencyConversion -> 1 USD in rupee
    InformationHeader  -> christmas day
    InformationWindow  -> who is garfield
    QuotesList         -> mahatma gandhi quotes
    TableSport         -> Chelsea next game
    InformationTable   -> the office

  LetrasMusBr -> https://www.letras.mus.br/
    Lyrics

)" << std::endl;

  return 0;
}

