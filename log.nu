#!/usr/bin/env nu
#
# Module for logging things with a kinda cute way.
# Reference for terminfo (colours, reverse, etc):
#	https://www.mankier.com/5/terminfo#Description-Predefined_Capabilities
#	https://linux.101hacks.com/ps1-examples/prompt-color-using-tput/
#
# João Farias © 2022-2024 BeyonadMagic <beyondmagic@mail.ru>

const NAME_DEFAULT = "log.nu"
const FORMAT_DEFAULT = "%s"

# Printf parser.
def fprint [
	...args: any     # Strings/number to print.
	--name: string   # Name of the program.
	--format: string # Format of the program.
	--colour: string # Colour of the whole message.
	--left: int = 0  # Extra padding.
] -> null {

	# Reverse video (foreground and background) characters.
	let t_reverse = (tput rev)
	# Turn off all attributes.
	let t_reset = (tput sgr0)
	# Bold character.
	let t_bold = (tput bold)

	let final_name = '[' + $name + ']'

	# Count how many "padders" we need to add based on newlines.
	let pad_chars = ($final_name | str length) + 1 + $left

	# Create the string full of spaces
	mut spaces = ''
	mut i = 1
	while $i <= $pad_chars {
		$i = $i + 1
		$spaces = $spaces + ' '
	}

	let formats_replacement = (char newline) + $spaces + '$1'
	let final_format = "%s " + ($format | str replace --regex --all (char newline) $formats_replacement) + "%s"

	let command = [
		'printf'
		'--'
	# Parse separately adding double-quote to all arguments so that printf parse it correctly.
	] | append ([
		$final_format
		($colour + $t_bold + $t_reverse + $final_name + $t_reset + $colour)
		...$args
		($t_reset)
	] | each {|arg|
		'"' + $arg + '"'
	})

	nu -c ($command | str join (char space))
	printf '\n'
}

# Print a crossed item in red (represents task failed).
export def fail [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	let t_fg_red = (tput setaf 1)
	fprint --left 4 --name $name --format ('[X] ' + $format) --colour $t_fg_red ...$message
}

# Print a checked item in green (represents task was done successfuly).
export def success [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	let t_fg_green = (tput setaf 2)
	fprint --left 4 --name $name --format ('[✓] ' + $format) --colour $t_fg_green ...$message
}

# Print in red (represents information that needs to be paid).
export def error [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	let t_fg_red = (tput setaf 1)
	fprint --name $name --format $format --colour $t_fg_red ...$message
}

# Print in yellow (represents information to be paid attention).
export def warning [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	let t_fg_yellow = (tput setaf 3)
	fprint --name $name --format $format --colour $t_fg_yellow ...$message
}

# Print in gray (represents extra information).
export def debug [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	let t_fg_white = (tput setaf 7)

	fprint --name $name --format $format --colour $t_fg_white ...$message
}
