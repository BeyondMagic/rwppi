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

#define EXIST_STRING(A) ( find( this->tokens.begin(), this->tokens.end(), A ) != this->tokens.end() )

#define GET_STRING(A)		\
    itr = find( this -> tokens.begin(), this -> tokens.end(), A );		\
    remove(this->tokens.begin(), this->tokens.end(), A);		\
    if ( itr != this->tokens.end() && ++itr != this->tokens.end() )

#define REMOVE_STRING(A) (this->tokens.erase(std::remove(this->tokens.begin(), this->tokens.end(), A), this->tokens.end()))

/*
 * This is basically our library-free code to parse our arguments, it is obviously not feature full for the same purporse
 * we created this. Basically, we use it so that we don't have to compile a 1MiB header file to just parse our options.
 * Please, future self or future maintainers, do only replace this if we have a minimal library that can actually do this better.
 *
 * TODO: Find a smart way to parse multiple bool inline commands such as '-abcdef'.
 * TODO: Only use and remove the first match.
 */
class InputParser
{
  private:
    std::vector <std::string> tokens;

  public:

    InputParser (const int argc, char ** argv );

    /* This will return the value after the option, so it verifies two strings.
     * It doesn't matter how you call it, but for both strings it will verify and take the next argument.
     * It will actually remove from the leftovers both the strings even if it just took the first one.
     * Original author is @iain, though it is modified for our needs.
     */
    const std::string
    get( const std::string & short_name, const std::string & long_name );

    /*
     * This will return the rest of our arguments after all the calls are made, such as this->get or this->exist.
     * Use this will great knowledge about what you're doing.
     */
    const std::string
    left ( void ) const;

    /* To return true if an option was passed to the arguments. It will remove from this->tokens the strings it finds.
     * short_name : -a
     * long_name  : --animal
    */
    bool
    exists( const std::string & short_name, const std::string & long_name );

};
