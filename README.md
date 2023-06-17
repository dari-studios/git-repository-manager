# git-repository-manager


### The problem

I have a wide multitude of git repositories that I utilize daily. It is often tedious to manually manage all of them. Any missed step results in merge conflicts that need to be solved, which oftentimes takes some digging.

---

### The solution

This program is designed to simplify the process of managing these repositories. 

It will fetch from the remote and display any repositories that have modified contents, and then provide the user various options on how they would like to manage the repositories.

These options will include:
- Macros to automatically add, commit, and push files
- Specify a commit message per repository (with optional timestamp attachment)
- Load a shell environment for the user to directly execute commands with
- Add files to be ignored (modifying .gitignore easily)

This will prevent the user from forgetting about any changes they have made in the repositories and thus avoid merge conflicts

In the event there is a merge conflict, the program will notify the user of the merge conflict and quit. This is to allow the user to manually deal with it, as it is very hazardous to automate the process.

The program will have basic functions such as ensuring that the correct input is given and protecting against invalid options

---

### Flowchart
```mermaid
flowchart LR
    A(Program start) --> B[Scan folders in directory for git repository] --> C[Show user the list of detected repositories]
    C--> C1[Fetch from remotes] --> C2[Show status of each] --> D[/Show menu and ask user for option/]
    D --> D1[Navigate to first repository] 
    
    D1 --> D2{User picked?}
    D2 -- Get status --> E1[Show status] --> F1
    D2 -- Add to staging area --> E2[Add files] --> F1

    D2 -- Commit all changes --> E3{Repository has changes?}
    E3 -- Yes --> E31[Commit files and ask for message] --> F1
    E3 --> No --> E32[Do nothing] --> F1

    D2 -- Push all changes --> E4[Push to remotes] --> F1
    D2 -- Launch shell --> E5[Launch shell environment] --> G1
    D2 -- Add to .gitignore --> E6[Launch gitignore editor] --> G1
    D2 -- Pull all changes --> E7[Pull changes from remotes] --> F1
    D2 -- exit --> E8([Terminate program])


    G1[Return to base directory] --> D

    F1{Are there are more repositories?} -- Yes --> F2[Navigate to next repository] --> D2
    F1{Are there are more repositories?} -- No --> D


```
---
### Usage

Basic workflow:

1. Clone target repository (git clone <url>)
2. Launch program in the folder that contains the repositories you want to manage
3. Pull in any changes from remote
4. Add any files you want to explicitly ignore to .gitignore
5. Add any untracked files
6. Commit all changes
7. Push to remotes
8. Repeat!

All of the options directly call on git, and do not add any special parameters

Additionally, one should automate managing multiple repositories unless they understand how git itself works

**Please refer to git's documentation for proper usage beyond what has been listed (including info on how to solve merge conflicts!)**

https://git-scm.com/docs

---

### Further notes

I plan on eventually porting this program to the [Rust](https://www.rust-lang.org/) language when I get the time
