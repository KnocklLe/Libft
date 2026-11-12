This project has been created as part of the 42 curriculum by edolemai.

Libft
📋 Description

Libft is the first project of the 42 Common Core curriculum. Its goal is to create a personal C library by reimplementing several functions from the standard C library and developing additional utility functions.

The project provides an opportunity to understand fundamental C programming concepts, including:

Memory allocation and management.
String manipulation.
Pointer arithmetic.
Data structures and linked lists.
File descriptor handling.
Function pointers.

The result is a reusable static library called libft.a, which can be integrated into future 42 projects and other C applications.

⚙️ Instructions
Compilation

To compile the library, clone the repository and navigate to its root directory.

Run:

bash
make


This generates the static library libft.a and the corresponding object files.

Makefile Commands
Command	Description
make	Compiles the library.
make all	Compiles the library.
make clean	Removes object files.
make fclean	Removes object files and the library.
make re	Performs a full recompilation.
Using the Library

To use Libft in another C project, include the header file:

c
#include "libft.h"


Compile your program by linking the library:

bash
cc main.c -I. -L. -lft -o program


Run the executable:

bash
./program


Requirements:

A C compiler (such as cc or gcc).
GNU Make.
A Unix-like operating system.
📚 Library Contents

The library is divided into three main parts, containing a total of 44 functions.

Part 1 – Libc Functions

This section consists of reimplementations of standard C library functions using the ft_ prefix.

The implementations reproduce the behavior of their standard counterparts while respecting the requirements of the 42 project.

Character Checking
Function	Description
ft_isalpha	Checks whether a character is alphabetic (a-z, A-Z).
ft_isdigit	Checks whether a character is a digit (0-9).
ft_isalnum	Checks whether a character is alphanumeric.
ft_isascii	Checks whether a character belongs to the ASCII character set.
ft_isprint	Checks whether a character is printable.
Character Conversion
Function	Description
ft_toupper	Converts a lowercase letter to uppercase.
ft_tolower	Converts an uppercase letter to lowercase.
String Functions
Function	Description
ft_strlen	Calculates the length of a string.
ft_strchr	Finds the first occurrence of a character in a string.
ft_strrchr	Finds the last occurrence of a character in a string.
ft_strncmp	Compares two strings up to a specified number of characters.
ft_strlcpy	Copies a string with a size limit.
ft_strlcat	Concatenates strings with a size limit.
ft_strnstr	Searches for a substring within a string up to a specified length.
Memory Functions
Function	Description
ft_memset	Fills a memory area with a specified byte.
ft_bzero	Sets a memory area to zero.
ft_memcpy	Copies a memory area from one location to another.
ft_memmove	Copies memory while handling overlapping regions.
ft_memchr	Searches for a byte in a memory area.
ft_memcmp	Compares two memory areas.
Conversion
Function	Description
ft_atoi	Converts a string into an integer.
Memory Allocation
Function	Description
ft_calloc	Allocates memory and initializes all bytes to zero.
ft_strdup	Duplicates a string by allocating new memory.
Part 2 – Additional Functions

This section introduces additional utility functions that simplify string manipulation, memory management, and output operations.

String Manipulation
Function	Description
ft_substr	Extracts a substring from a given string.
ft_strjoin	Joins two strings into a newly allocated string.
ft_strtrim	Removes specified characters from the beginning and end of a string.
ft_split	Splits a string into an array of substrings using a delimiter.
ft_itoa	Converts an integer into a string.
ft_strmapi	Applies a function to each character and returns a new string.
ft_striteri	Applies a function to each character, allowing the original string to be modified.
Output Functions

These functions write data to a specified file descriptor.

Function	Description
ft_putchar_fd	Writes a character to a file descriptor.
ft_putstr_fd	Writes a string to a file descriptor.
ft_putendl_fd	Writes a string followed by a newline.
ft_putnbr_fd	Writes an integer to a file descriptor.
Part 3 – Linked Lists

This section implements a collection of functions for creating and manipulating singly linked lists.

The library uses the following node structure:

c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;


Each node contains:

content: A pointer to the data stored in the node.
next: A pointer to the next node in the list.
Linked List Functions
Function	Description
ft_lstnew	Creates a new linked list node.
ft_lstadd_front	Adds a node to the beginning of a list.
ft_lstsize	Counts the number of nodes in a list.
ft_lstlast	Returns the last node of a list.
ft_lstadd_back	Adds a node to the end of a list.
ft_lstdelone	Deletes a single node and frees its content.
ft_lstclear	Deletes a node and all following nodes.
ft_lstiter	Applies a function to the content of each node.
ft_lstmap	Creates a new linked list by applying a function to each node's content.
🛠️ Technical Overview
Static Library

Libft is compiled into a static library named libft.a.

A static library is an archive of object files that can be linked into other programs during compilation. This allows functions to be reused without copying their implementations into every project.

Memory Management

Several functions in Libft allocate memory dynamically using malloc or calloc.

Functions that return newly allocated memory require the caller to release it using free when it is no longer needed.

Particular attention is given to:

Correct allocation sizes.
Null pointer handling.
Avoiding memory leaks.
Correctly freeing allocated memory.
Handling allocation failures.
Function Pointers

Some functions accept function pointers as arguments, allowing custom operations to be applied to strings and linked lists.

Examples include:

ft_strmapi
ft_striteri
ft_lstiter
ft_lstmap

This provides flexibility and introduces concepts commonly used in more advanced C programming.

📊 Project Summary
Part	Category	Number of Functions
Part 1	Libc Functions	24
Part 2	Additional Functions	11
Part 3	Linked Lists	9
Total	Libft	44
📖 Resources
Documentation and References
42 Libft Subject
Linux Manual Pages
C Reference
GNU C Library Documentation
GNU Make Documentation

These resources were used to understand standard C function behavior, memory management, string manipulation, linked lists, and compilation.

AI Usage

AI tools were used as supplementary learning and development resources throughout the project.

Their use included:

Understanding concepts: Clarifying C programming concepts, pointer manipulation, memory allocation, and linked list structures.
Function behavior: Understanding the expected behavior of standard library functions and their edge cases.
Debugging: Helping identify potential errors and understand compilation issues.
Documentation: Assisting with the organization and writing of this README file.

AI was used as a learning and support tool rather than a replacement for understanding the code. The goal was to improve programming knowledge and develop the ability to implement and debug C functions independently.

👤 Author

edolemai

42 Student