# C data structures
Various data structures implemented in C language for learning purposes 
and possibly further personal use.

## Table of contents
* [General info](#general-information)
* [Linked List v1](#linked-list-v1)
* [Linked List v2](#linked-list-v2)
* [Circular Queue](#circular-queue)
* [Bit Field](#bit-field)

## General information
This repository is a collection of various data structures written in the C language. 
Efficiency was not always the author's priority, some structures were created solely 
for experimental purposes, and may have several implementations, of varying degrees of bugginess. 
Use at your own risk.

<details>
<summary>

## Linked List v1
</summary>

### About
The first version of the linked list structure is created with a void pointer 
in the list node, which can contain the address of an instance of any data type. \
It uses macros as an interface to call all internal functions. 
This is done to avoid errors related to differences in the type of data supplied 
to the input and those already in the list.

### Functionality
List of functions and their complexity:
- Inserting one element at the end of the list. *O(n)*
- Getting an element at some index. *O(n)*
- Removing an element at some index. *O(n)*

### Usage
This structure has four macros for performing manipulations:
```
// Initializes the list
LIST(type of data that will be stored in a list)

// Inserts an element
LIST_INSERT(
    pointer to data, 
    list address, 
    type of input data
)

// Returns an element of the expected type
LIST_GET(
    index, 
    list address, 
    type of expected data
)

// Same as get function, except it also removes the element from the list
LIST_REMOVE(
    index,
    list address,
    type of expected data
)
```
**Important! Do not ever input literals into LIST_INSERT** \
Example with string:

```c
#include "list.h"

int main(void)
{
    // Initializing the list, and setting its type to char*
    LinkedList list = LIST(char *);

    char s1[] = "First";
    char s2[] = "Second";

    // In order to avoid memory problems,
    // we have to pack all data into pointers, and 
    // then pass the address of that newly created pointers to the insert function
    char **sptr1 = (char **)s1;
    char **sptr2 = (char **)s2;

    // Inserting elements
    // At this stage, if the types of input data and 
    // list data are mismatched, the program terminates
    LIST_INSERT(&sptr1, &list, char *);
    LIST_INSERT(&sptr2, &list, char *);

    // Removing an element
    char *s3 = LIST_REMOVE(0, &list, char *);

    return 0;
}
```

Example with int:

```c
#include "list.h"

int main() {
    // Initializing the list, and setting its type to int
    LinkedList list = LIST(int);

    int n1 = 1;
    int n2 = 2;

    // In case of non-pointer types we don't
    // need to create an extra layer of pointers
    LIST_INSERT(&n1, &list, int);
    LIST_INSERT(&n2, &list, int);

    // Removing an element
    int n3 = LIST_REMOVE(0, &list, int);
    printf("%d\n", n3);

    return 0;
}
```
</details>


<details> 
<summary>

## Linked List v2
</summary>

### About 
The second version of the linked list is implemented as a single header file, 
containing a macro that creates adaptive structures and functions. \
Also it uses macros as an interface to call all internal functions. 
This is done to avoid errors related to differences in the type of data supplied 
to the input and those already in the list.

### Functionality
List of functions and their complexity:
- Inserting one element at the end of the list. *O(n)*
- Getting an element at some index. *O(n)*
- Removing an element at some index. *O(n)*

### Usage
This structure has six macros for performing manipulations:
```
// Create set of structures and functions for some type
// Each type should be defined only once!
// Held only in the global scope!
DEFINE_LIST(type)

// Initializes the list
LIST(type of data that will be stored in a list)

// Inserts an element
LIST_INSERT(data, list, type of input data)

// Returns an element of the expected type
LIST_GET(index, list, type of expected data)

// Same as get function, except it also removes the element from the list
LIST_REMOVE(index, list, type of expected data)

// Returns size of the list
LIST_SIZE(list, type)
```

Example of int list:

```c
#include "list.h"

// Defining a list of a specific data type. 
// At this stage all required structures and functions are created
DEFINE_LIST(int)

int main() {
    // Initializing the list, and setting its type to int
    LinkedList list = LIST(int);

    // Inserting elements
    LIST_INSERT(1, list, int);
    LIST_INSERT(2, list, int);
    LIST_INSERT(3, list, int);

    // Removing an element
    int n = LIST_REMOVE(0, list, int);

    return 0;
}
```

Example of pointer-type list (char *):

```c
#include "list.h"

// When dealing with pointer types, you need to create a new 
// data type for them without the asterisk character
typedef char * string;
DEFINE_LIST(string)

int main() {
    // Initializing the list, and setting its type
    LinkedList list = LIST(string);

    // Inserting elements
    LIST_INSERT("Earth", list, string);
    LIST_INSERT("Mars", list, string);
    LIST_INSERT("Jupyter", list, string);

    // Removing an element
    string s = LIST_REMOVE(0, list, string);

    return 0;
}
```
</details>


<details>
<summary>

## Circular Queue
</summary>

### About
Implementation of a circular queue based on an array of static size.

### Functionality
List of functions and their complexity:
- Inserting one element at the end of the queue. *O(1)*
- Removing the first element from the queue. *O(1)*
- Getting the first element of the queue without removing it. *O(1)*

</details>


<details>
<summary>

## Bit Field
</summary>

### About
Implementation of a bit field.

### Functionality
List of functions and their complexity:
- Set value at some index. *O(1)*
- Get value at some index. *O(1)*

</details>