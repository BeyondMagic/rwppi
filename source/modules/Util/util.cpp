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

#include <iostream>

/*
 * Functions rather marvelous to remove the necessity of repeating so many times the same words in the same way.
 *  void print_iris( ... )         -> Will print to the stderr in the order of the given strings.
 * I don't think there's a way to minimize this anymore, so for now, it is what it is.
 */

/*
 * Print the help list of Iris; should always be updated in case of major changes.
 * <void> -> 0
 */
const unsigned int
print_help( void )
{

  std::cout << R"(iris - Version: 0.0.1

DESCRIPTION:
  iris is a CLI-assistant that receives any type of input and tries to find an answer for it.
  It strives to have a simple codebase while being extremely performant and lightweight.

USAGE:
  iris [--help] [--] [--unit 0-6] [--source sources]

  --help | -h
    Print out this and immediately exit the program.

  --
    Query will be literal, it won't parse it into URL enconding. Useful if you have a encoded query already.

  --unit | -u
    Print out all logging information of an unit, this is preferably for debugging to
    contributors of the program.

  --source source1,source2,... | -s source1,source2,...
    Read respectively each source and tries to find and answer on it. If it doesn't recognize by
    name, it will ignore and try to look at the next one. For the first answer it finds, it will
    stop.

    Sources can be any of the following:
      Google      -> https://www.google.com/
      Wikipedia   -> https://??.wikipedia.org/ (Supported languages by Wikipedia)
      LetrasMusBr -> https://www.letras.mus.br/

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
