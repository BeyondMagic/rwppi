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
 */

#include "main.hpp"

/*
 * https://github.com/iris-cli/iris
 *
 * v0.0.1
 */
int main( const int argc, char** argv )
{

  AITricks tricks;
  Util util;

  const std::string language = getenv("LANG")
    ? std::string( getenv("LANG") ).substr(0, 5)
    : "en_US";
  std::string  query;
  std::string  sources   = "0";
  bool         log       = false;
  bool         load_file = false;
  bool         save_file = false;
  bool         unit_two  = true; // In case we want to skip local methods.
  std::string  methods   = "0";
  unsigned int unit      = 3;    // Default is all without log.


  /*
   * UNIT 0: Parse options and parameters.
   *
   * Modules: ArgumentParser, Util, AITricks.
   */
  {
    try {

      InputParser arguments(argc, argv);

      {
        if ( arguments.exists( "-h", "--help"    ) ) return util.print_help();
        if ( arguments.exists( "-S", "--sources" ) ) return util.print_sources();
        if ( arguments.exists( "-M", "--methods" ) ) return util.print_methods();
        if ( arguments.exists( "-l", "--log"     ) ) log = true;
        if ( arguments.exists( "-t", "--to"      ) ) save_file = true;
        if ( arguments.exists( "-L", "--load"    ) ) load_file = true;

        if ( isatty(STDIN_FILENO) and isatty(fileno(stdin)) )
        {
          is_atty = true;
        }

        if (save_file and load_file) throw "[1] EE: '--to' and '--load' options are exclusive.";
      }

      {

        std::string handler_settings;

        handler_settings = arguments.get( "-s", "--source" );
        if ( !handler_settings.empty() )
        {
          sources = handler_settings;
          handler_settings.clear();
          unit_two = false;
        }
        handler_settings = arguments.get( "-m", "--method" );
        if ( !handler_settings.empty() )
        {
          methods = handler_settings;
          handler_settings.clear();
          unit = 4;
        }

        handler_settings = arguments.get( "-u", "--unit"   );
        if ( !handler_settings.empty() )
        {
          unit = stoi( handler_settings );
          handler_settings.clear();

          if  (unit == 1) log = true;
          else if ( unit < 1 || unit > 3 ) throw "[1] EE: The unit option must be betweeen 1-3.";
        }

      }

      query = arguments.left();

      if (log)
      {
        std::cout << "[1] Logging settled defaults... "              << '\n';
        std::cout << "[1] Unit set:                   " << unit      << '\n';
        std::cout << "[1] Language set:               " << language  << '\n';
        std::cout << "[1] Log set:                    " << log       << '\n';
        std::cout << "[1] Sources set:                " << sources   << '\n';
        std::cout << "[1] Method set:                 " << methods   << '\n';
        std::cout << "[1] Your query is exactly:      " << query     << '\n';
        std::cout << "[1] Is on ATTY (Terminal):      " << is_atty   << '\n';
        std::cout << "[1] Save file:                  " << save_file << '\n';
        std::cout << "[1] Load file:                  " << load_file << '\n';
      }

      if ( query.empty() ) throw 0;

    }

    // For out of range unit numbers:
    catch ( const char * & a )
    {
      std::cerr << a << std::endl;
      return 1;
    }

    // For empty query.
    catch ( const int & a )
    {
      std::cerr << tricks.nothing() << std::endl;
      return 1;
    }

    // For stoi throwing an invalid_argument error.
    catch ( const std::invalid_argument & ia )
    {
      std::cerr << "[1] EE: You must use the unit option along a number." << std::endl;
      return 1;
    }

  }

  /*
   * In case the user does not define methods to use as listed by "--methods" and defined by "--method".
   */
  if (unit != 4)
  {

    /*
     * UNIT 2: Run the local methods for simple computation tasks such as mathematical ones.
     */
    if (unit >= 2 and unit_two)
    {

      MethodLocal local(query);

      local.All();

      util.has_response();

    }

    /*
     * UNIT 3: Run the remote methods, those are the scrapers.
     */
    if (unit == 3)
    {

      URLDownloader downloader(language, query);

      std::vector<int> sources_list = util.list_to_vector(sources);

      for (int name : sources_list) {

        std::string page;

        if (load_file) {

          std::ifstream t(query);
          std::stringstream buffer;
          buffer << t.rdbuf();
          page = buffer.str();

        } else {

          page = downloader.download(name);

        }

        if (save_file) {

          if (page.empty()) exit(1);

          std::cout << page << std::endl;

          exit(0);

        }

        MethodRemote remote(page);

        switch (name) {

          // Google.
          case 0: remote.Google_All(); break;

        }

      }

      util.has_response();
      exit(1);

    }
  }

  /*
   * UNIT 4:
   *
   * The only way to call this unit is when define methods.
   *
   */
  else {

    if (log) std::cout << "[4] Special unit." << std::endl;

    // Loop through each method
    // method_string
    // {
    //    method_hash = unique_number_from_string(method_string)
    //
    //    switch (method_hash)
    //    {
    //      case LOCAL_MATH:
    //        Local local;
    //        break;
    //
    //      case GOOGLE_MATH:
    //        Remote remote;
    //
    //        break;
    //
    //      // Don't accept unknown methods.
    //      default:
    //        std::cerr << "[4] EE: The method '" << method_string <<' hashed '" << method_hash << "' is unknown." << std::endl;
    //
    //        return 1;
    //    }
    // }

  }

  return 1;

};
