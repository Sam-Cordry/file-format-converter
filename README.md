# File Format Converter

## Overview
This is a personal project that I started in order to learn and become more
familiar with C and some of its more unique aspects, such as dynamic memory
management and structures. As of this version, this code can read PNGs and
JPEGs into their own specially-made structures and write them from said
structures. You can also find the start of similar progress being made for
TIFF files as well.

## How to Use
In its current form, this project does not have the necessary files in it to
help you setup this tool globally on your system. However, you can still use
this tool by either installing it locally yourself or by just referring to the
executable file by its path. Download the repository and run the following
command in the root directory of the project:
```bash
make
```
Once you have this, you can run the program by using the following command:
```bash
./ffc
```
You can also run one of these help commands to see what options are currently
available:
```bash
./ffc -h
./ffc --help
```

## The Current Next Step
As of right now, I am looking into how to algorithmically generate a JPEG from
a PNG and vice versa. This will be the first major step in the "converter" part
of this project. I honestly thought it couldn't be that hard to do when I started
this project. I know now that I was quite wrong, mostly because there is not
a lot of documentation or explanation about the process itself.

## Possible Blog or Tutorial
In doing research for this project, I was never able to find a good place to
find information to accomplish tasks like this. Because of that lack of
information on the Internet, there is the potential that I will create a blog
about what I am working on in this project on [my website](https://samcordry.dev).
Or, as an alternative, it might be more of a tutorial-style thing that just
walks through my thought process and why I chose to do things the way I did.

## Hope for this Project
Eventually, I hope to dedicate enough time to this to make it an all-in-one
CLI file format converting tool that is capable of all of the common file
formats (and maybe even some uncommon ones). As a part of that, I might even
be creating my own specifications for a generic "data" file format in order
to increase the modularity within the code itself. And if I get real crazy
with it, I might even be creating a GUI that utilizes this program so that
all users can comfortably use the program without feeling the need to open
a terminal.
