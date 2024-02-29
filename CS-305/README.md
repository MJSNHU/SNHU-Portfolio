   - Briefly summarize your client, Artemis Financial, and their software requirements. Who was the client? What issue did they want you to address?
         
        Artemis Financial develops financial plans to include savings, retirements, investments, and insurance. Artemis has a publicly facing website and want to make sure communications through it are secure.  They want to add a file verification step over HTTPS. 


- What did you do very well when you found your client’s software security vulnerabilities? Why is it important to code securely? What value does software security add to a company’s overall wellbeing?

	I think researching how to prevent Cross-site scripting via OWASP's java encoder went well.  Secure coding is important for a defense-in-depth approach to mitigating vulnerabilities.  If one layer of defense fails, the next layer makes it harder to exploit.  Software security may seem like a cost burden, but when it works (which is hard to see because absense of proof is not proof of absense) it can save the company countless hours of mitigation and, just as importantly, customer trust. 

   - What part of the vulnerability assessment was challenging or helpful to you?

        Figuring out the rate limiter was a particular challenge for me.  I researched and tried at least three methods before getting the bucket option to work.  

   - How did you increase layers of security? In the future, what would you use to assess vulnerabilities and decide which mitigation techniques to use?

        I added a self-signed certificate and a keystore and ensured require-ssl was set to true to ensure traffic would be conducted over https only. The risk of cross-site-scripting (XSS) was introduced when adding the functionality for the user to provide a string to be encoded.  I included the [OWASP java encoder](https://owasp.org/www-project-java-encoder/) to prevent the raw input from being processed and evaluated by the server. I also included functionality for a rate limiter to prevent overwhelming the server by rapid requests.  This required the dependency [bucket4j-core](https://bucket4j.com/).

   - How did you make certain the code and software application were functional and secure? After refactoring the code, how did you check to see whether you introduced new vulnerabilities?

	I confirmed the presence of an XSS vulnerability introduced by the client input parameter, added the OWASP encoder functionality and tested again to see that the XSS code was rendered as text instead of executed. I tried to access the site via http and it failed.  I made a script to rapidly connect to the server and saw that the rate was indeed limited. Finally, I ran the maven dependency vulnerability checker and ensured no new vulnerabilities were introduced.

   - What resources, tools, or coding practices did you use that might be helpful in future assignments or tasks?

        The Maven vulnerability checker is very useful to find potential vulnerabilities in projects with numerous dependencies.  Sanitizing input is always a good idea to prevent unintended code execution.   	

   - Employers sometimes ask for examples of work that you have successfully completed to show your skills, knowledge, and experience. What might you show future employers from this assignment?

        Showing employees the security assessment is a good summary of the lessons learned in this course.  Additionally, one could add the original code and refactored project to show what was added to the code by using git diff.
