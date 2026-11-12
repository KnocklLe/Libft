<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:050505,50:003B25,100:00FF88&height=220&section=header&text=LIBFT&fontSize=85&fontColor=00FF88&fontAlignY=38&desc=Building%20my%20own%20C%20library&descAlignY=60&descSize=18&animation=fadeIn" width="100%"/>

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=500&size=18&duration=3000&pause=1000&color=00FF88&center=true&vCenter=true&width=600&lines=42+Common+Core+%7C+First+Project;Recreating+the+C+Standard+Library;Memory+Management+%7C+Pointers+%7C+Linked+Lists" />

<br/>

<img src="https://img.shields.io/badge/42-School-black?style=for-the-badge&logo=42&logoColor=white"/>
<img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c"/>
<img src="https://img.shields.io/badge/Library-libft.a-00FF88?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Functions-44-purple?style=for-the-badge"/>

My first step into the world of low-level programming.

</div>

⚠️ This project has been created as part of the 42 curriculum by edolemai.
🖥️ 01 — About the Project

Libft is the first project of the 42 Common Core.

The objective is to create my own C library by recreating several functions from the standard C library and implementing additional useful functions.

Through this project, I learned how to:

Understand how standard C functions work.
Manage memory allocation and deallocation.
Manipulate strings and memory.
Work with pointers and pointer arithmetic.
Implement and manipulate linked lists.

The final result is a static library called libft.a, which can be reused in future 42 projects.

⚙️ 02 — Installation & Compilation
Clone the repository
bash
git clone YOUR_REPOSITORY_URL
cd Libft

Compile the library
bash
make


This generates the static library libft.a.

🛠️ Makefile Commands
Command	Description
make	Compile the library
make all	Compile the library
make clean	Remove object files
make fclean	Remove object files and library
make re	Clean and recompile everything
Using the Library

Include the header file:

c
#include "libft.h"


Compile your program:

bash
cc main.c -L. -lft -o program

📚 03 — Library Contents

The library is divided into 3 main parts, containing a total of 44 functions.

🧩 Part 1 — Libc Functions

Reimplementation of standard C library functions using the ft_ prefix.

<details>
<summary><b>🔤 Character Checking & Conversion</b></summary>

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
<summary><b>📝 String Functions</b></summary>

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
<summary><b>🧠 Memory Functions</b></summary>

Function	Description
ft_memset	Fills memory with a specified byte.
ft_bzero	Sets a memory area to zero.
ft_memcpy	Copies a memory area.
ft_memmove	Copies memory while handling overlapping regions.
ft_memchr	Searches for a byte in memory.
ft_memcmp	Compares two memory areas.

</details>

<details>
<summary><b>🔄 Conversion & Allocation</b></summary>

Function	Description
ft_atoi	Converts a string into an integer.
ft_calloc	Allocates zero-initialized memory.
ft_strdup	Duplicates a string using dynamic allocation.

</details>

⚙️ Part 2 — Additional Functions

Additional functions designed to simplify string manipulation and output operations.

<details>
<summary><b>✂️ String Manipulation</b></summary>

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
<summary><b>📤 Output Functions</b></summary>

Function	Description
ft_putchar_fd	Writes a character to a file descriptor.
ft_putstr_fd	Writes a string to a file descriptor.
ft_putendl_fd	Writes a string followed by a newline.
ft_putnbr_fd	Writes an integer to a file descriptor.

</details>

🔗 Part 3 — Linked Lists

Implementation of functions to create, manage and manipulate linked lists.

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
📊 04 — Project Statistics

<div align="center">

<img src="https://img.shields.io/badge/Total_Functions-44-00FF88?style=for-the-badge&labelColor=101010"/>
<img src="https://img.shields.io/badge/Modules-3-7B2CBF?style=for-the-badge&labelColor=101010"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&labelColor=101010"/>

</div>

Part	Category	Functions
01	Libc Functions	24
02	Additional Functions	11
03	Linked Lists	9
TOTAL	Libft	44
🧠 05 — Skills & Concepts

Through Libft, I developed a better understanding of:

text
┌──────────────────────────────────────────┐
│             CORE CONCEPTS                │
├──────────────────────────────────────────┤
│  > C Programming                         │
│  > Pointers & Pointer Arithmetic        │
│  > Dynamic Memory Allocation             │
│  > String Manipulation                   │
│  > Memory Management                     │
│  > Linked Lists & Data Structures        │
│  > Makefile & Static Libraries           │
│  > Debugging & Problem Solving           │
└──────────────────────────────────────────┘

📖 06 — Resources
42 Libft Subject
Linux Manual Pages
C Reference
GNU C Library Documentation
🤖 07 — AI Usage

AI was used as a learning tool to better understand C concepts, clarify function behaviors and assist with debugging.

It also helped me improve my understanding of memory management, string manipulation and linked lists.

The objective was to use these resources to strengthen my knowledge, improve my problem-solving skills and become more independent in C programming.

👤 08 — Author

<div align="center">

<img src="https://img.shields.io/badge/42-Student-black?style=for-the-badge&logo=42&logoColor=white"/>

edolemai

42 Student | C Developer

Learning by building, understanding by coding.

<br/>

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:00FF88,50:003B25,100:050505&height=100&section=footer" width="100%"/>
</div>