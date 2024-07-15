- Summarize the project and what problem it was solving.

This assignment was to create a program to analyze text records a grocery store generated logging items purchased.  The program has four menu options: 
    1) prompt a user to input an item to search for and return the number of times that item appeared in a given list
    2) tally the list and print the items followed by the frequency of the item in the list
    3) return the same results as number two but in a histogram using asterisks to represent each time the item was purchased
    4) quit
In addition, the program needed to create a backup file of each item and the number of times it appeared in the list.

- What did you do particularly well?

There was an optional challenge to provide input validation. I think my program does this well, allowing only the integers 1-4 to be input.  If anything else is input, an appropriate error message is output and the user is given another opportunity to select one of the options. 

- Where could you enhance your code? How would these improvements make your code more efficient, secure, and so on?

I think some of the file handling was redundant and could have been moved to a function or two.  This would have made exception handling more efficient and the code more readable.  The more readable, the more maintainable.

- Which pieces of the code did you find most challenging to write, and how did you overcome this? What tools or resources are you adding to your support network?

The input validation was tricky- stoi returns a valid integer if the string begins with an integer, even if subsequent characters are not digits.  For example the stoi("1.5") is returned as the integer 1.  I found some code on Stack Overflow that measured the length of the string successfully as an integer against the length of the original string, and only returns a valid integer if they are the same length.  I thought that was pretty clever, and will be using Stack Overflow in the future. 

- What skills from this project will be particularly transferable to other projects or course work?

The object- oriented methodologies will be useful in other projects.  This is still a fairly new concept to me, but ensuring only the portions of the program that need to be accessed by the outside can be, while compartmentalizing the private portions will make my programming more secure.

- How did you make this program maintainable, readable, and adaptable?

When documenting my code, I used syntax that could be read by a documentation generation tool called Doxygen.  Having a standard format for inline comments itself makes the code more readable, and proper documentation makes it more maintainable.  I did use global variables for my filenames and used the variable names in my file handling procedures.  This makes it easier to update the filenames as needed or even get user input for them.
