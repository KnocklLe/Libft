This project has been created as part of the 42 curriculum by edolemai.

Libft
Description

Libft is the first project of the 42 Common Core. The goal of this project is to create my own C library by recreating several functions from the standard C library and implementing new useful functions.

This project helps me understand how C functions work, how to manage memory, manipulate strings and work with pointers and linked lists.

The library is called libft.a and can be reused in future 42 projects.

Instructions
Compilation

To compile the library, run the following command:

bash
make


This will generate the static library libft.a.

Makefile commands
Command	Description
make	Compiles the library
make all	Compiles the library
make clean	Removes object files
make fclean	Removes object files and the library
make re	Removes everything and recompiles the library
Using the library

To use the library in a C project, include the header file:

c
#include "libft.h"


Then compile your program with the library:

bash
cc main.c -L. -lft -o program

Library Contents

The project is divided into three parts.

Part 1 – Libc functions

Reimplementation of standard C library functions using the ft_ prefix.

Character checking: ft_isalpha, ft_isdigit, ft_isalnum, ft_isascii, ft_isprint.
Character conversion: ft_toupper, ft_tolower.
String functions: ft_strlen, ft_strchr, ft_strrchr, ft_strncmp, ft_strlcpy, ft_strlcat, ft_strnstr.
Memory functions: ft_memset, ft_bzero, ft_memcpy, ft_memmove, ft_memchr, ft_memcmp.
Conversion: ft_atoi.
Memory allocation: ft_calloc, ft_strdup.
Part 2 – Additional functions

Functions created to make string manipulation and output easier.

ft_substr: Extracts a substring from a string.
ft_strjoin: Joins two strings.
ft_strtrim: Removes specified characters from the beginning and end of a string.
ft_split: Splits a string using a delimiter.
ft_itoa: Converts an integer into a string.
ft_strmapi: Applies a function to each character and creates a new string.
ft_striteri: Applies a function to each character and allows it to be modified.
ft_putchar_fd: Writes a character to a file descriptor.
ft_putstr_fd: Writes a string to a file descriptor.
ft_putendl_fd: Writes a string followed by a newline.
ft_putnbr_fd: Writes an integer to a file descriptor.
Part 3 – Linked lists

Implementation of functions to create and manipulate linked lists.

ft_lstnew: Creates a new list node.
ft_lstadd_front: Adds a node at the beginning of a list.
ft_lstsize: Counts the number of nodes.
ft_lstlast: Returns the last node.
ft_lstadd_back: Adds a node at the end of a list.
ft_lstdelone: Deletes one node.
ft_lstclear: Deletes a node and all following nodes.
ft_lstiter: Applies a function to each node's content.
ft_lstmap: Creates a new list by applying a function to each node's content.
Resources
Documentation
42 Libft Subject
Linux Manual Pages
C Reference
GNU C Library Documentation
AI Usage

AI was used as a learning tool to help understand C concepts, clarify function behaviors and assist with debugging.

It was also used to better understand memory management, manipulation and linked lists.

The goal was to use these resources to improve my understanding of the project and learn how to write better C code.

Author

edolemai

42 Student