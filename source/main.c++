// João Farias © GNU-AGPL3 2024 BeyondMagic <beyondmagic@mail.ru>
#include <fmt/format.h>

int main (const int argc, char** argv)
{
	constexpr auto hello_world = "Hello World!";
	fmt::print("My first message is: {}\n", hello_world);
}
