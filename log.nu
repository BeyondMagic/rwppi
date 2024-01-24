#!/usr/bin/env nu
#
# Module for logging things with a kinda cute way.
#
# João Farias © 2022-2024 BeyonadMagic <beyondmagic@mail.ru>

const NAME_DEFAULT = "log.nu"
const FORMAT_DEFAULT = "%s"

export-env {
	# Reference for colours:
	#	https://linux.101hacks.com/ps1-examples/prompt-color-using-tput/
	$env.colours = {
		# Reverse.
		reverse: (tput rev)
		# Turn off all attributes.
		reset: (tput sgr0)
		# Bold character.
		bold: (tput bold)
		# Background color
		bg: {
			white: (tput setab 7)
		}
		# Foreground colour.
		fg: {
			dim: (tput dim)
			black: (tput setaf 0)
			red: (tput setaf 1)
			green: (tput setaf 2)
			yellow: (tput setaf 3)
			white: (tput setaf 7)
		}
	}
}

# Printf parser.
def fprint [
	...args: any     # Strings/number to print.
	--name: string   # Name of the program.
	--format: string # Format of the program.
	--colour: string # Colour of the whole message.
	--left: int = 0  # Extra padding.
] -> null {

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
		($colour + $env.colours.bold + $env.colours.reverse + $final_name + $env.colours.reset + $colour)
		...$args
		($env.colours.reset)
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
	fprint --left 4 --name $name --format ('[X] ' + $format) --colour $env.colours.fg.red ...$message
}

# Print a checked item in green (represents task was done successfuly).
export def success [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	fprint --left 4 --name $name --format ('[✓] ' + $format) --colour $env.colours.fg.green ...$message
}

# Print in red (represents information that needs to be paid).
export def error [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	fprint --name $name --format $format --colour $env.colours.fg.red ...$message
}

# Print in yellow (represents information to be paid attention).
export def warning [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	fprint --name $name --format $format --colour $env.colours.fg.yellow ...$message
}

# Print in gray (represents extra information).
export def debug [
	...message: string,                 # Message to print
	--name: string = $NAME_DEFAULT,     # Name of the program.
	--format: string = $FORMAT_DEFAULT, # Format (printf specification) of the message.
] -> null {
	fprint --name $name --format $format --colour $env.colours.fg.white ...$message
}
