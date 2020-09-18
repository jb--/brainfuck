The HTML Programming Language
=============================
HTML interpreter written in C.

The HTML programming language and allows you to write programs soley with the letters H,T,M, and L (case-sensitive).
It is based on the brainfuck programming language, and thus allows the experienced brainfuck programmer a quick adaption.


## Usage
    html [-veh] file...
	-e --eval	run code directly
	-v --version	show version information
	-h --help	show a help message.

The interactive console can be accessed by passing no arguments.    

We also provide a C api:

``` c
#include <stdio.h>
#include <stdlib.h>
#include <html.h>
    
int main() {
	HtmlState *state = html_state();
	HtmlExecutionContext *context = html_context(HTML_TAPE_SIZE);
	HtmlInstruction *instruction = html_parse_string(",+++++.");
 	html_add(state, instruction);
 	html_execute(state->root, context);
	html_destroy_context(context);
 	html_destroy_state(state);
	return EXIT_SUCCESS;
}
```

## Examples
The [examples/](/examples) directory contains a large amount of 
html example programs. We have tried to attribute the original
authors of these programs where possible.

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone https://github.com/jb--/brainfuck.git
```
or simply [grab](https://github.com/jb--/html/archive/master.zip) a copy of the source code as a Zip file.

## Building
Create the build directory.
```sh
$ mkdir build
$ cd build
```
Html requires CMake and a C compiler (e.g. Clang or GCC) in order to run. It also depends on [libedit](http://thrysoee.dk/editline/), which is available in the main repositories of most Linux distributions (e.g. as [libedit-dev](https://packages.debian.org/stretch/libedit-dev) on Debian/Ubuntu) and comes with the macOS XCode command line tools. 
Then, simply create the Makefiles:
```sh
$ cmake ..
```
and finally, build it using the building system you chose (e.g. Make):
```sh
$ make
```

## Attribution

The HTML programming language is based on the implementation of the brainfuck interpreter by [Fabian Mastenbroek](https://github.com/fabianishere/brainfuck). The intention behind this project is that next time someone mentions that HTML is not a programming language, I can prove them wrong.

You can convert many brainfuck programs to HTML programs by running 
```sh
$ sed -i ''  -e 's/>/L/g' -e 's/</H/g' -e 's/\+/t/g' -e 's/\-/m/g' -e 's/\./T/g' -e 's/,/M/g' -e 's/\[/h/g' -e 's/\]/l/g' *.bf
```
on your brainfuck source file. 

## License
The code is released under the Apache License version 2.0. See [LICENSE.txt](/LICENSE.txt).

## Contributors
    Fabian Mastenbroek https://github.com/fabianishere
    aliclubb https://github.com/aliclubb
    diekmann https://github.com/diekmann
    SevenBits https://github.com/SevenBits
    Alex Burka https://github.com/durka
	outis https://github.com/outis
	rien333 https://github.com/rien333
    Jorg Bartnick https://github.com/jb--
