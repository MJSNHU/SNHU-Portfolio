    - Briefly summarize the requirements and goals of the app you developed. What user needs was this app designed to address?

    The goal of this app was to develop a simple reminder list that incorporated a login/registration feature with a password that saved them to a database, use a database to save and retrieve user events and display them in a list, and obtain permission from the user to send SMS notifications when the event time arrives.

    - What screens and features were necessary to support user needs and produce a user-centered UI for the app? How did your UI designs keep users in mind? Why were your designs successful?

    The app consists of a login/registration screen, event list, and event editor page.  Each page has labels or familiar buttons with either labels or hints to tell the user what each item of the interface is used for.  Keeping the options simple for the user allowed them to focus on their needs.

    - How did you approach the process of coding your app? What techniques or strategies did you use? How could those techniques or strategies be applied in the future?

    I approached coding in sections.  First, I developed a rough outline of the interface screens, then worked on each one at a time: the login/registration feature, the event list, and the event editor page.  Working on one design and feature at a time helped make each more robust when it came time to integrate them.

    - How did you test to ensure your code was functional? Why is this process important, and what did it reveal?

    I used the emulator and Logcat constantly to work on a small change and test it out.  The emulator showed overall how and when the app was working or not and Logcat helped see what parts of the code were actually breaking down or passing.

    - Consider the full app design and development process from initial planning to finalization. Where did you have to innovate to overcome a challenge?

    Learning to use the tools available made it possible to innovate solutions.  For the longest time I had to guess whether my database was being correctly built and accessed by log results.  Finally, I learned to use adb to get into the filesystem of the emulator and access the database directly to verify it was being initiated, populated and updated correctly.    

    - In what specific component of your mobile app were you particularly successful in demonstrating your knowledge, skills, and experience?

    The edit event screen worked fairly well to create, update, or delete an event.  I thought it was intuitive how to save or delete and what fields to update.