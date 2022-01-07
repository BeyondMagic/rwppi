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

/*
 * Libraries are included in the following files and so is expectable for the language server
 * to recognize support for certain standards libraries classes and functions.
 *
 * TODO: When there's full support from GCC and a language server for modules, modularize the following files:
 */

#include "modules/ArgumentParser/main.hpp"
#include "modules/AssistantFun/main.hpp"

#include "main.hpp"

/*
 * Iris is a CLI-assistant that receives any type of input and tries to find an answer for it.
 * It strives to have a simple and highly documented codebase while being extremely performant and lightweight.
 *
 *  https://github.com/iris-cli/iris
 *
 *  v0.0.1
 */
int main( const int argc, char** argv )
{

  // This is read externally from the environment variable "LANG", set accordingly to the language
  // you want Iris to operate on. Default is set "en_US" or English of USA.
  const std::string language = getenv("LANG")
    // To get only the first four characters.
    ? std::string( getenv("LANG") ).substr(0, 5)
    : "en_US";

  // Query is what you're sending to Iris to as a question, it will be used on all UNITS.
  std::string query;

  // List of sources, which are divided by comma. Default is "Google", e.j: "Google,Wikipedia".
  std::string sources = "Google";

  // Log everything it is doing.
  bool log = false;

  // To what unit Iris will operate on, by setting this it will be used to log.
  // Default is 3 (all without log).
  unsigned int unit = 3;

  // For fun when throwing exceptions.
  AITricks tricks;

  /*
   * UNIT 0: Parse options and parameters.
   *
   * Modules: ArgumentParser, Util, AITricks.
   */
  {
    try {

      // Initialize the method we use to parse our options.
      InputParser arguments(argc, argv);

      // If there is those strings on the argument list.
      {
        if ( arguments.exists( "-h", "--help"    )) return print_help();
        if ( arguments.exists( "-S", "--sources" )) return print_sources();
        if ( arguments.exists( "-M", "--methods" )) return print_methods();
      }

      // To get the value after the strings on the argument list
      {
        std::string sources_handler = arguments.get( "-s", "--source" );
        std::string unit_handler    = arguments.get( "-u", "--unit" );

        if ( !sources_handler.empty() ) sources = sources_handler;
        if ( !unit_handler.empty() )
        {
          unit = stoi( unit_handler );
          log  = true;
        }

        if ( unit < 1 || unit > 3 ) throw "[1] EE: The unit option must be betweeen 1-3.";
      }

      // Return what is left from the argument list after the parsing.
      query = arguments.left();

      // Log everything this unit found.
      if (log) {
        std::cout << "[1] Logging settled defaults..."     << "\n";
        std::cout << "[1] Unit set: "              << unit     << "\n";
        std::cout << "[1] Language set: "          << language << "\n";
        std::cout << "[1] Sources set: "           << sources  << "\n";
        std::cout << "[1] Your query is exactly: " << query    << "\n";
      }

      /*
       * Excpetions/errors that happen and will force the program to exit.
       */
      if ( query.empty() ) throw 0;

    }

    // To out of range numbers:
    catch ( const char * & a )
    {
      std::cerr << a << std::endl;
      return 1;
    }

    // To defined exceptions with numbers to be used along AITricks.
    catch ( const int & a )
    {
      std::cerr << tricks.nothing() << std::endl;
      return 1;
    }

    // In case stoi throws an invalid_argument error.
    // Make explicit use of stoi and throws the rest to C++ handles it...
    catch ( const std::invalid_argument & ia )
    {
      std::cerr << "[1] EE: You must use the unit option along a number." << std::endl;
      return 1;
    }

  }

  // UNIT 2:
  if (unit >= 2) {

    std::cout << "[2] LOL" << std::endl;

  }

  // UNIT 3:
  if (unit >= 3) {

    std::cout << "[3] HAH" << std::endl;

  }

};
