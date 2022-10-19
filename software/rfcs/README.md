# What is an RFC?

RFC stands for Request For Comments. RFCs are used to put forward proposals for different parts of the avionics system, which should then be reviewed by other members of the avionics team. An RFC may go through several rounds of improvement and review before being accepted. Once an RFC has been accepted and implemented, it should be put in the docs folder.


# How do I make an RFC?
Create a new RFC document in this folder, under its own git branch. Then write your proposal in it.
Once you've written the RFC, push the branch to github and open a Pull Request with it and ask people for feedback.

(See the structFS doc for an example of an RFC that got accepted)

## Template
1. Quick summary of what it does - just to introduce everyone to the main idea behind the proposal.
2. Pre-reading - so everyone is on roughly the same page regarding tech.
3. Why is this needed? - briefly explain the problem.
4. Main proposal - write whatever you think you need to describe what you're proposing.
5. Potential issues (Questionable design decisions) - anything you're not sure is a good idea
6. Extensions - how this could be changed if our requirements change in a predictable way, i.e. we need to store more data, or make this faster.

# How should I review an RFC?
* Leave your comments on the pull request, so we can track the discussion. 
* If you don't understand something, highlight it and ask, other people might not really understand it too.
* Give constructive criticism.