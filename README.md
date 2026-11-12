*This project has been created as part of the 42 curriculum by edolemai.*

# Libft

**My C library**

## Description

Libft is the first project of the 42 Common Core curriculum.

The objective of this project is to create a personal C library by reimplementing several functions from the standard C library (libc), developing additional utility functions, and implementing linked list manipulation functions.

The purpose is to understand how fundamental C functions work internally, improve memory management skills, and build a reusable library for future 42 projects.

The final result is a static library named `libft.a`, which can be integrated into other C projects.

### Project objectives

- Understand and reimplement standard C library functions.
- Improve string and memory manipulation skills.
- Learn dynamic memory allocation using `malloc` and `free`.
- Work with pointers and pointer arithmetic.
- Understand and manipulate linked lists.
- Create and manage a static library using a Makefile.
- Follow the 42 Norm.

## Instructions

### Requirements

- A C compiler (`cc`).
- `make`.
- The `ar` command.
- A Unix-based operating system.

### Installation

Clone the repository:

```bash
git clone https://github.com/KnocklLe/Libft.git
cd Libft
```

### Compilation

Compile the library using:

```bash
make
```

This generates the static library:

```text
libft.a
```

### Makefile commands

| Command | Description |
|---|---|
| `make` | Compile the library |
| `make all` | Compile the library |
| `make clean` | Remove object files (`.o`) |
| `make fclean` | Remove object files and `libft.a` |
| `make re` | Clean and recompile everything |

### Using the library

Include the library header in your C source file:

```c
#include "libft.h"
```

Compile your program with the library:

```bash
cc main.c -L. -lft -o program
```

Run your program:

```bash
./program
```

## Library Contents

The library is organized into three main parts.

### Part 1 — Libc Functions

Reimplementation of standard C library functions using the `ft_` prefix.

#### Character checking and conversion

| Function | Description |
|---|---|
| `ft_isalpha` | Checks whether a character is alphabetic. |
| `ft_isdigit` | Checks whether a character is a digit. |
| `ft_isalnum` | Checks whether a character is alphanumeric. |
| `ft_isascii` | Checks whether a character belongs to the ASCII character set. |
| `ft_isprint` | Checks whether a character is printable. |
| `ft_toupper` | Converts a lowercase character to uppercase. |
| `ft_tolower` | Converts an uppercase character to lowercase. |

#### String functions

| Function | Description |
|---|---|
| `ft_strlen` | Calculates the length of a string. |
| `ft_strchr` | Finds the first occurrence of a character in a string. |
| `ft_strrchr` | Finds the last occurrence of a character in a string. |
| `ft_strncmp` | Compares two strings up to a specified number of characters. |
| `ft_strlcpy` | Copies a string with a size limit. |
| `ft_strlcat` | Concatenates strings with a size limit. |
| `ft_strnstr` | Searches for a substring within a limited length. |

#### Memory functions

| Function | Description |
|---|---|
| `ft_memset` | Fills a memory area with a specified byte. |
| `ft_bzero` | Sets a memory area to zero. |
| `ft_memcpy` | Copies a memory area. |
| `ft_memmove` | Copies memory while handling overlapping regions. |
| `ft_memchr` | Searches for a byte in a memory area. |
| `ft_memcmp` | Compares two memory areas. |

#### Conversion and allocation

| Function | Description |
|---|---|
| `ft_atoi` | Converts a string into an integer. |
| `ft_calloc` | Allocates memory initialized to zero. |
| `ft_strdup` | Duplicates a string using dynamic memory allocation. |

### Part 2 — Additional Functions

Additional functions designed to simplify string manipulation and output operations.

#### String manipulation

| Function | Description |
|---|---|
| `ft_substr` | Extracts a substring from a string. |
| `ft_strjoin` | Joins two strings into a newly allocated string. |
| `ft_strtrim` | Removes specified characters from the beginning and end of a string. |
| `ft_split` | Splits a string into an array using a delimiter. |
| `ft_itoa` | Converts an integer into a string. |
| `ft_strmapi` | Applies a function to each character and creates a new string. |
| `ft_striteri` | Applies a function to each character using its index, modifying the original string. |

#### Output functions

| Function | Description |
|---|---|
| `ft_putchar_fd` | Writes a character to a file descriptor. |
| `ft_putstr_fd` | Writes a string to a file descriptor. |
| `ft_putendl_fd` | Writes a string followed by a newline. |
| `ft_putnbr_fd` | Writes an integer to a file descriptor. |

### Part 3 — Linked Lists

Implementation of functions to create, manage, and manipulate linked lists.

The library uses the following structure:

```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;
```

| Function | Description |
|---|---|
| `ft_lstnew` | Creates a new linked list node. |
| `ft_lstadd_front` | Adds a node at the beginning of a list. |
| `ft_lstsize` | Counts the number of nodes in a list. |
| `ft_lstlast` | Returns the last node of a list. |
| `ft_lstadd_back` | Adds a node at the end of a list. |
| `ft_lstdelone` | Deletes and frees a single node. |
| `ft_lstclear` | Deletes and frees an entire list. |
| `ft_lstiter` | Applies a function to each node's content. |
| `ft_lstmap` | Creates a new list by applying a function to each node's content. |

## Technical Choices

- **Language:** C.
- **Compilation:** `cc` with `-Wall -Wextra -Werror`.
- **Library format:** Static library (`libft.a`).
- **Library creation:** `ar`.
- **Memory management:** Dynamic allocation with `malloc` and deallocation with `free`.
- **Coding standard:** 42 Norm.

All functions are implemented according to the project requirements, without relying on the original functions being reimplemented.

## Skills & Concepts

Through this project, I developed a better understanding of:

- C programming fundamentals.
- Pointers and pointer arithmetic.
- Dynamic memory allocation.
- String and memory manipulation.
- Memory management and leak prevention.
- Linked lists and data structures.
- Static libraries and Makefiles.
- Debugging and problem solving.

## Resources

The following resources are useful references for this project:

- [Linux Manual Pages](https://man7.org/linux/man-pages/) — Documentation for standard C functions.
- [C Reference](https://en.cppreference.com/w/c) — C language documentation.
- [GNU C Library Documentation](https://www.gnu.org/software/libc/manual/) — Reference for standard library behavior.

### AI Usage

AI tools may be used as supplementary learning resources. If used during development, document precisely which tasks they helped with and which parts of the project were affected.

For this project, AI assistance was used to help organize the README and explain C concepts, memory allocation, pointers, string manipulation, linked lists, and debugging approaches. The implementation and understanding of each function should remain the author's responsibility and should be explainable during peer evaluation.

## Author

**edolemai**

42 Student | C Developer

[GitHub Profile](https://github.com/KnocklLe)

---

*Learning by building, understanding by coding.*
