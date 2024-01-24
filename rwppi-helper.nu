#!/usr/bin/env nu
#
# Helper for building, testing, and checking rwppi.
#
# On Nushell, start with:
#	$ use ./build.nu
#
# João Farias © 2022-2024 BeyonadMagic <beyondmagic@mail.ru>

const library = './library/'
const name = 'rwppi-helper'
use ./log.nu

# Build/update all libraries.
export def libraries [
	--lexbor = true # Compile/update lexbor.
] -> null {
	if (test -d $library | complete).exit_code != 0 {
		log debug --name $name $'No folder found at "($library)", creating one.'
		mkdir $library
	}

	let data = [
		{
			name: 'lexbor',
			object: 'liblexbor_static.a'
			git: 'https://github.com/lexbor/lexbor',
			submodules: false,
			build: {
				cmake . -DLEXBOR_BUILD_TESTS=ON -DLEXBOR_BUILD_EXAMPLES=ON
				make
				make test
			},
		}
	]

	$data | each {|item|
		if $item.name == 'lexbor' and (not $lexbor) {
			log warning --name $name $'Skipping "($item.name)".'
			continue
		}
		log debug --name $name $'Getting the library "($item.name)".'

		let lib_folder = ($library | path join $item.name)

		mut result = test -d $lib_folder | complete

		mut changes = false

		if $result.exit_code != 0 {

			log debug --name $name $'Library folder for "($item.name)" does not exist, creating one.'
			mkdir $lib_folder

			# Get the repository.
			$result = (git clone --recursive $item.git $lib_folder | complete)
			if $result.exit_code != 0 {
				log fail --name $name --format "%s:\n%s" $'Failed to clone the repository for ($item.name):' $item.git
				log warning --name $name $'Skipping the library ($item.name).'
				continue
			} else {
				log success --name $name --format "%s\n%s" $'Successfully cloned the repository for ($item.name):' $item.git
			}

			$changes = true

			cd $lib_folder
		} else {
			cd $lib_folder

			log debug --name $name $"Updating the remote\(s\) of '($item.name)'."

			# Get the remote repository.
			$result = (git remote update | complete)
			if $result.exit_code != 0 {
				log fail --name $name $"Could not update the remote\(s\) of the '($item.name)'."
				log warning --name $name $'Skipping the library "($item.name)".'
				continue
			} else {
				log success --name $name $"Updated the remote\(s\) of the repository of '($item.name)'."
			}

			# Return the amount of different commits compared to the remote tree.
			$result = (git rev-list HEAD...origin/master --count | complete)
			if ($result.stdout | str substring 0..1) != '0' {
				log debug --name $name --format "%s\n%s" $'Pulling new ($result.stdout) changes from repository for the library "($item.name)".' $item.git

				$changes = true

				# Get latest update.
				$result = (git pull | complete)
				if $result.exit_code != 0 {
					log fail --name $name --format "%s\n%s" $'Failed to pull from the remote for the library "($item.name)":' $item.git
					log warning --name $name $'Skipping the library ($item.name).'
					continue
				} else {
					log success --name $name $'Updated the repository of the library "($item.name)".'
				}
			}

			if $item.submodules {
				git submodule foreach git rev-list HEAD...origin/master --count | complete

				log debug --name $name $'Updating the submodules from the repository for the library "($item.name)".'

				# Update the remote of each submodule.
				$result = (git submodule foreach git remote update | complete)
				if $result.exit_code != 0 {
					print $result.sdout
					log fail --name $name $"Could not update the remote\(s\) of the submodule\(s\) for the library '($item.name)', one submodule failed:"
					log warning --name $name $'Skipping the library ($item.name).'
					continue
				} else {
					log success --name $name $"Updated the remote\(s\) of the submodule\(s\) of the library '($item.name)'."
				}

				# TODO: Try to find if there is one change for one submodule the remote of each submodule.
				# $result = (git submodule foreach git remote update | complete)
				# if $result.exit_code != 0 {
				# 	print $result.sdout
				# 	log fail --name $name $"Could not update the remote\(s\) of the submodule\(s\) for the library '($item.name)', one submodule failed:"
				# 	log warning --name $name $'Skipping the library ($item.name).'
				# 	continue
				# } else {
				# 	log success --name $name $"Updated the remote\(s\) of the submodule\(s\) of the library '($item.name)'."
				# }

				# Update recursively all submodules of the repository.
				$result = (git submodule update --init --recursive | complete)
				if $result.exit_code != 0 {
					log fail --name $name $'Could not update the submodules for the library "($item.name)":'
					log warning --name $name $'Skipping the library ($item.name).'
					continue
				} else {
					log success --name $name $'Updated the submodules of the library "($item.name)".'
					if $result.stdout {
						$changes = true
					}
				}
			}
		}

		# Building the library.
		if not $changes {
			log debug --name $name $'No changes for the library "($item.name)".'
			continue
		}

		log debug --name $name $'Building the library "($item.name)".'
		do $item.build
		if $env.LAST_EXIT_CODE != 0 {
			log fail --name $name $'Could not build the library "($item.name)".'
			log warning --name $name $'Skipping the library ($item.name).'
			continue
		} else {
			log success --name $name $'Built the library "($item.name)".'
		}

		# Copying the files for the source code to build.
		let object_out = ($item.object | prepend '../' | str join)

		log debug --name $name --format "%s\n%s\n%s\n%s" ...[
			$'Copying the necessary files of the library "($item.name)" from:'
			($item.object | path expand)
			'to'
			($object_out | path expand)
		]

		$result = (cp -f $item.object $object_out)
		if $result.exit_code != 0 {
			log fail --name $name $"Could not place the file\(s\)/folder\(s\) of the library '($item.name)' in the expected place."
			log warning --name $name $'Skipping the library ($item.name).'
			continue
		} else {
			log success --name $name $"Put file\(s\)/folder\(s\) the library '($item.name)'."
		}

		# Return to the $library folder.
		cd -
	}
	null
}

# Build source code.
export def build [
	--output : string = './binary/rwppi',   # Binary output.
	--input : string = './source/main.c++', # Source main input.
	--eyes = false,                         # Print the command of build.
	--optimise = false,                     # Optimise (O3).
	--debug = true,                         # Add debug flags.
] -> any {
	mut args = [
		# Latest standard of C++, however we'll set at C++23 upon module support in GCC.
		'-std=c++2b'
	]

	let libs = [
		# Curl: request POST to HTTP address.
		'-lcurl'
		# PThread: parse multiple things at the same time in different threads.
		'-lpthread'
		# fmt: for formating the output.
		'-lfmt'
		# Lexbor: parse HTML we'll fetch.
		($library | path join 'liblexbor_static.a')
	]

	if $optimise {
		$args = ($args | append [
			# Optimise in accordance to O3 specification.
			'-O3'
		])
	} else {
		$args = ($args | append [
			# No unused variables.
			'-Wno-unused'
			# All warnings to be treated as errors.
			'-Wall'
			# Extra flgs.
			'-Werror'
			# Further extra flags.
			'-Wextra'
			# Stick to the standardad.
			'-pedantic'
			# Disallow implicit conversion.
			'-Wconversion'
			# Disallow implicit sign conversion.
			'-Wsign-conversion'
			# Variable shadowing/repeating names of variables in the same context.
			'-Wshadow'
		])
	}

	if $debug {
		$args = ($args | append [
			# For debugging with gdb.
			'-ggdb'
			# To catch UB as must as it can.
			'-fsanitize-undefined-trap-on-error'
			# To catch memory-leak-specific UB as must as it can.
			'-fsanitize-address-use-after-scope'
			# Sanitize all addresses and UB by default.
			'-fsanitize=address,undefined'
		])
	}

	let command = [
		'c++'
		...$args
		...$libs
		$input
		'-o'
		$output
	]

	mkdir ($output | path dirname)

	let result = (nu -c ($command | str join (char space)) | complete)

	if ($eyes) or ($result.exit_code != 0) {
		print $command
	}
}
