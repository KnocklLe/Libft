<!-- ==================== HEADER ==================== -->

<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:050505,50:003B25,100:00FF88&height=220&section=header&text=LIBFT&fontSize=85&fontColor=00FF88&fontAlignY=38&desc=Building%20my%20own%20C%20library&descAlignY=60&descSize=18&animation=fadeIn" width="100%"/>

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=500&size=18&duration=3000&pause=1000&color=00FF88&center=true&vCenter=true&width=600&lines=42+Common+Core+%7C+First+Project;Recreating+the+C+Standard+Library;Memory+Management+%7C+Pointers+%7C+Linked+Lists;Learning+C+one+function+at+a+time..." alt="Typing SVG"/>

<br/>

<img src="https://img.shields.io/badge/42-School-black?style=for-the-badge&logo=42&logoColor=white"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white"/>
<img src="https://img.shields.io/badge/Library-libft.a-00FF88?style=for-the-badge&logo=files&logoColor=black"/>
<img src="https://img.shields.io/badge/Functions-44-7B2CBF?style=for-the-badge"/>

<br/><br/>

[ SYSTEM INITIALIZED ]

My first step into the world of low-level programming.

</div>

01 // Introduction
⚠️ This project has been created as part of the 42 curriculum by edolemai.

Libft is the first project of the 42 Common Core.

The objective is to recreate a personal C library by implementing several functions from the standard C library and developing additional useful utilities.

This project is about understanding what happens behind the scenes:

Memory allocation and deallocation.
String manipulation and memory operations.
Pointers and pointer arithmetic.
Data structures and linked lists.
How standard C functions work internally.

The final result is a static library called libft.a, designed to be reused in future 42 projects.

02 // Installation
Clone the repository
bash
git clone git@github.com:KnocklLe/Libft.git
cd Libft

Compile
bash
make


<details>
<summary>🛠️ Available Makefile commands</summary>

Command	Description
make	Compile the library
make all	Compile the library
make clean	Remove object files
make fclean	Remove object files and library
make re	Clean and recompile everything

</details>

Use the library

Include the header:

c
#include "libft.h"


Compile your program:

bash
cc main.c -L. -lft -o program

03 // Library Architecture

<div align="center">

Module	Description	Functions
🧩 Part 1	Libc Functions	24
⚙️ Part 2	Additional Functions	11
🔗 Part 3	Linked Lists	9
TOTAL	Libft Library	44

</div>

🧩 Part 1 — Libc Functions

Reimplementation of standard C library functions using the ft_ prefix.

<details>
<summary>🔤 Character Checking & Conversion — 7 functions</summary>

Function	Description
ft_isalpha	Checks whether a character is alphabetic.
ft_isdigit	Checks whether a character is a digit.
ft_isalnum	Checks whether a character is alphanumeric.
ft_isascii	Checks whether a character belongs to ASCII.
ft_isprint	Checks whether a character is printable.
ft_toupper	Converts lowercase to uppercase.
ft_tolower	Converts uppercase to lowercase.

</details>

<details>
<summary>📝 String Functions — 7 functions</summary>

Function	Description
ft_strlen	Calculates the length of a string.
ft_strchr	Finds the first occurrence of a character.
ft_strrchr	Finds the last occurrence of a character.
ft_strncmp	Compares two strings up to n characters.
ft_strlcpy	Copies a string with a size limit.
ft_strlcat	Concatenates strings with a size limit.
ft_strnstr	Searches for a substring within a limited length.

</details>

<details>
<summary>🧠 Memory Functions — 6 functions</summary>

Function	Description
ft_memset	Fills memory with a specified byte.
ft_bzero	Sets a memory area to zero.
ft_memcpy	Copies a memory area.
ft_memmove	Copies memory while handling overlapping regions.
ft_memchr	Searches for a byte in memory.
ft_memcmp	Compares two memory areas.

</details>

<details>
<summary>🔄 Conversion & Allocation — 4 functions</summary>

Function	Description
ft_atoi	Converts a string into an integer.
ft_calloc	Allocates zero-initialized memory.
ft_strdup	Duplicates a string using dynamic allocation.

</details>

⚙️ Part 2 — Additional Functions

Additional functions designed to simplify string manipulation and output operations.

<details>
<summary>✂️ String Manipulation — 7 functions</summary>

Function	Description
ft_substr	Extracts a substring.
ft_strjoin	Joins two strings into a new string.
ft_strtrim	Removes specified characters from both ends.
ft_split	Splits a string using a delimiter.
ft_itoa	Converts an integer into a string.
ft_strmapi	Applies a function to every character.
ft_striteri	Applies a function to each character in place.

</details>

<details>
<summary>📤 Output Functions — 4 functions</summary>

Function	Description
ft_putchar_fd	Writes a character to a file descriptor.
ft_putstr_fd	Writes a string to a file descriptor.
ft_putendl_fd	Writes a string followed by a newline.
ft_putnbr_fd	Writes an integer to a file descriptor.

</details>

🔗 Part 3 — Linked Lists

Implementation of functions to create, manage and manipulate linked lists.

<details>
<summary>🔗 Linked List Functions — 9 functions</summary>

Function	Description
ft_lstnew	Creates a new linked list node.
ft_lstadd_front	Adds a node at the beginning.
ft_lstsize	Counts the number of nodes.
ft_lstlast	Returns the last node.
ft_lstadd_back	Adds a node at the end.
ft_lstdelone	Deletes a single node.
ft_lstclear	Deletes and frees an entire list.
ft_lstiter	Applies a function to each node's content.
ft_lstmap	Creates a new list by applying a function to each node.

</details>

04 // Project Statistics

<div align="center">

<img src="https://img.shields.io/badge/Total_Functions-44-00FF88?style=for-the-badge&labelColor=101010"/>
<img src="https://img.shields.io/badge/Modules-3-7B2CBF?style=for-the-badge&labelColor=101010"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&labelColor=101010"/>

<br/><br/>

Part	Progress
Part 1 — Libc	🟩🟩🟩🟩🟩 24 functions
Part 2 — Additional	🟩🟩🟩🟩🟩 11 functions
Part 3 — Linked Lists	🟩🟩🟩🟩🟩 9 functions

</div>

05 // Skills & Concepts

Through Libft, I developed a better understanding of:

text
┌──────────────────────────────────────────┐
│             CORE CONCEPTS                │
├──────────────────────────────────────────┤
│  > C Programming                         │
│  > Pointers & Pointer Arithmetic         │
│  > Dynamic Memory Allocation             │
│  > String Manipulation                   │
│  > Memory Management                     │
│  > Linked Lists & Data Structures        │
│  > Makefile & Static Libraries           │
│  > Debugging & Problem Solving           │
└──────────────────────────────────────────┘

06 // Resources
42 Libft Subject
Linux Manual Pages
C Reference
GNU C Library Documentation
07 // AI Usage

AI was used as a learning tool to better understand C concepts, clarify function behaviors and assist with debugging.

It also helped me improve my understanding of memory management, string manipulation and linked lists.

The objective was to use these resources to strengthen my knowledge, improve my problem-solving skills and become more independent in C programming.

08 // Author

<div align="center">

<img src="https://img.shields.io/badge/42-Student-black?style=for-the-badge&logo=42&logoColor=white"/>

edolemai

42 Student | C Developer

Learning by building, understanding by coding.

<br/>

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:00FF88,50:003B25,100:050505&height=100&section=footer" width="100%"/>

</div>