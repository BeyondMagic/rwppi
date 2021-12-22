/*
 * Copyright 2021 João F. BeyondMagic <koetemagie@gmail.com>
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
 * TODO: When there's full support from GCC and a language server for modules, modularize:
 */
#include "modules/ArgumentParser/arguments.cpp"
#include "modules/Util/util.cpp"

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
  std::string sources;

  // cURL will not encode the query if set to true. If false, e.j: " " will become "%20".
  bool literal = false;

  // Log everything it is doing.
  bool log     = true;

  // To what unit Iris will operate on, by setting this it will be used to log.
  // Default is 7 (all without log).
  unsigned int unitlog = {};

  /*
   * UNIT 0: Parse options and parameters.
   *
   * Parse options and parameters.
   */
  try {

    InputParser arguments(argc, argv);

    if ( arguments.exists( "-h", "--help" )) return print_help();
    if ( arguments.exists( "--", "--" )) literal  = true;

    sources = arguments.get( "-s", "--source" );
    unitlog = stoi( arguments.get( "-u", "--unit"   ) );

    if ( sources.empty() ) sources = "Google";

    query = arguments.left();

    if (log) {
      std::cout << "[0] Setting defaults..." << "\n";
      std::cout << "[0] Unit set: " << unitlog << "\n";
      std::cout << "[0] Language set: " << language << "\n";
      std::cout << "[0] Sources set: "  << sources << "\n";
      std::cout << "[0] Literal set (1 for true]: "  << literal << "\n";
      std::cout << "[0] Your query is exactly: " << query << "\n";
    }

    /*
     * Meta
     */
    if ( query.empty() ) // return AITricks.emtpy();
    {
      std::cout << "Haha, your query is so small! I can't even see it!" << "\n";
      // std::cout << AITricks.empty() << std::endl;
      exit(1);
    }

  }
  catch (const std::invalid_argument& ia) {
    std::cerr << "[0] EE: You must use the unit option along a number.\n" << ia.what() << std::endl;
    return 1;
  }
/*  catch (const unsigned int a) {
    switch (a) {

      case 0: std::cerr << "[0] EE: Unit is too big."; break;
      default: std::cerr << "[0] EE: No idea."; break;

    }
    std::cerr << std::endl;
    return 1;
  }*/

  // UNIT 1:

};
