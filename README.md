This project has been created as part of the 42 curriculum by edolemai.

Libft
1. Description

Libft is the first project of the 42 Common Core curriculum.

The goal of this project is to create a personal C library by recreating several functions from the standard C library and implementing additional useful functions.

Through this project, I learned how to:

Understand how standard C functions work.
Manage memory allocation and deallocation.
Manipulate strings and memory.
Work with pointers and pointer arithmetic.
Implement and manipulate linked lists.
Build and use static libraries.

The final result is a static library called libft.a, which can be reused in future 42 projects.

2. Installation and Compilation
Clone the repository
bash
git clone https://github.com/KnocklLe/Libft.git
cd Libft

Compile the library
bash
make


This generates the static library libft.a.

Makefile Commands
Command	Description
make	Compiles the library.
make all	Compiles the library.
make clean	Removes object files.
make fclean	Removes object files and the library.
make re	Cleans and recompiles everything.
Using the Library

Include the header file in your C program:

c
#include "libft.h"


Compile your program by linking the library:

bash
cc main.c -I. -L. -lft -o program


Run the executable:

bash
./program

3. Library Contents

The library is divided into three main parts, containing a total of 44 functions.

Part 1 - Libc Functions

Reimplementation of standard C library functions using the ft_ prefix.

Character Checking and Conversion
Function	Description
ft_isalpha	Checks whether a character is alphabetic.
ft_isdigit	Checks whether a character is a digit.
ft_isalnum	Checks whether a character is alphanumeric.
ft_isascii	Checks whether a character belongs to ASCII.
ft_isprint	Checks whether a character is printable.
ft_toupper	Converts a lowercase letter to uppercase.
ft_tolower	Converts an uppercase letter to lowercase.
String Functions
Function	Description
ft_strlen	Calculates the length of a string.
ft_strchr	Finds the first occurrence of a character.
ft_strrchr	Finds the last occurrence of a character.
ft_strncmp	Compares two strings up to n characters.
ft_strlcpy	Copies a string with a size limit.
ft_strlcat	Concatenates strings with a size limit.
ft_strnstr	Searches for a substring within a limited length.
Memory Functions
Function	Description
ft_memset	Fills memory with a specified byte.
ft_bzero	Sets a memory area to zero.
ft_memcpy	Copies a memory area.
ft_memmove	Copies memory while handling overlapping regions.
ft_memchr	Searches for a byte in memory.
ft_memcmp	Compares two memory areas.
Conversion and Memory Allocation
Function	Description
ft_atoi	Converts a string into an integer.
ft_calloc	Allocates zero-initialized memory.
ft_strdup	Duplicates a string using dynamic allocation.
Part 2 - Additional Functions

Additional functions designed to simplify string manipulation and output operations.

String Manipulation
Function	Description
ft_substr	Extracts a substring from a string.
ft_strjoin	Joins two strings into a new string.
ft_strtrim	Removes specified characters from both ends of a string.
ft_split	Splits a string using a delimiter.
ft_itoa	Converts an integer into a string.
ft_strmapi	Applies a function to every character and returns a new string.
ft_striteri	Applies a function to each character in place.
Output Functions
Function	Description
ft_putchar_fd	Writes a character to a file descriptor.
ft_putstr_fd	Writes a string to a file descriptor.
ft_putendl_fd	Writes a string followed by a newline.
ft_putnbr_fd	Writes an integer to a file descriptor.
Part 3 - Linked Lists

Implementation of functions to create, manage and manipulate singly linked lists.

The library uses the following structure:

c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;


Each node contains a pointer to its content and a pointer to the next node.

Linked List Functions
Function	Description
ft_lstnew	Creates a new linked list node.
ft_lstadd_front	Adds a node at the beginning of a list.
ft_lstsize	Counts the number of nodes.
ft_lstlast	Returns the last node.
ft_lstadd_back	Adds a node at the end of a list.
ft_lstdelone	Deletes a single node and frees its content.
ft_lstclear	Deletes and frees an entire list.
ft_lstiter	Applies a function to each node's content.
ft_lstmap	Creates a new list by applying a function to each node.
4. Project Statistics
Part	Category	Number of Functions
Part 1	Libc Functions	24
Part 2	Additional Functions	11
Part 3	Linked Lists	9
Total	Libft	44
5. Technical Concepts

This project helped me develop a better understanding of fundamental C programming concepts.

The main concepts covered are:

C programming and standard library functions.
Pointers and pointer arithmetic.
Dynamic memory allocation using malloc and free.
Memory management and memory manipulation.
String manipulation.
Linked lists and data structures.
Function pointers.
Static libraries and Makefiles.
Debugging and problem solving.
6. Resources

The following resources were used to understand C programming, standard library functions and memory management.

42 Libft Subject
Linux Manual Pages
C Reference
GNU C Library Documentation
GNU Make Documentation
AI Usage

AI was used as a learning and development support tool throughout the project.

Its use included:

Understanding concepts: Clarifying C programming concepts, pointers, memory allocation and linked list structures.
Function behavior: Understanding standard library functions and their expected behavior.
Debugging: Helping identify potential errors and understand compilation issues.
Documentation: Assisting with the organization and writing of this README file.

AI was used to support the learning process, improve my understanding of the code and develop better problem-solving skills.

7. Author

edolemai

42 Student | C Developer