/*
    This file provides the main functions for git-repository-manager
    Copyright (C) 2023 dari-studios

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "addignore.h"

// List all directories and probe those directories for .git folders
// Unfortunately this only goes down by 1 level
void findRepos(std::vector<std::string> &repoList, std::string currPath)
{
    // For each directory entry found, scan them for a .git folder
    for (auto dir : std::filesystem::directory_iterator(currPath))
    {
        // Variable to store the path to check
        std::string dirCheck = dir.path().string() + "/.git";

        // If .git folder exists, then add it to the list
        if (std::filesystem::is_directory(dirCheck))
        {
            repoList.push_back(dir.path().string().erase(0, currPath.size()));
        }
    } //..end dir
}

// For each repo that has changes, add a commit message
void commitWithMsg(bool hasChange, std::string currRepo)
{
    std::fstream commitMsgFile;
    std::string commitMsg;

    // Only run if current repository has changes
    // Otherwise do nothing
    if (hasChange)
    {
        // Open file in append-only mode to create it
        commitMsgFile.open("COMMITMSG.txt", std::fstream::app);

        // Add commit message
        std::cout << "Enter commit message for " << currRepo << ": ";
        std::getline(std::cin, commitMsg);

        // Close file so it gets written
        commitMsgFile << commitMsg;
        commitMsgFile.close();

        // Make commit
        std::cout << "Committing changes from " << currRepo << "...\n";
        system("git commit -a -F COMMITMSG.txt");

        // Clean up
        std::filesystem::remove(std::filesystem::current_path().string() + "/COMMITMSG.txt");
        std::cout << "Done.\n\n";
    }
}

// Checks the current repository and flags whether or not it has any changes
void checkForChange(bool &hasChange)
{
    system("git status | cat > gitstatus.txt");

    std::fstream gitStatus;
    int lines = 1;
    std::string buffer;

    gitStatus.open("gitstatus.txt");

    // While we are still reading lines, add to count
    while (std::getline(gitStatus, buffer))
    {
        lines++;
    }

    // If there are more than 9 lines in the output, there is a change
    // Otherwise do nothing
    hasChange = lines > 9 ? true : false;

    gitStatus.close();
    std::filesystem::remove(std::filesystem::current_path().string() + "/gitstatus.txt");
}

// General function for commands
void gitComm(std::vector<std::string> repoList, bool &hasChange, std::string currPath, int option)
{

    // For every repository in the list, we will execute the requested command
    // Note that some commands are run-once
    for (std::string currRepo : repoList)
    {
        // Navigate to repository directory
        currPath.append(currRepo);
        std::filesystem::current_path(currPath);

        /* Execute given command

        Reference:
        0: Fetch            1: Get status       2: Add files to staging
        3: Commit changes   4: Push changes     5: Launch shell
        6: Add to gitignore 7: Pull changes     8: Exit

        */
        switch (option)
        {
        case 0:
            std::cout << "\n-------------------------------------\n";
            std::cout << "Fetching from " << currRepo << "...\n";

            // Fetch from remote
            system("git fetch");

            std::cout << "Done.";
            break;

        case 1:
            std::cout << "\n-------------------------------------\n";
            std::cout << "\nStatus of " << currRepo << ": \n\n";

            // Get repository's status
            system("git status");

            break;

        case 2:
            std::cout << "\n-------------------------------------\n";
            std::cout << "Adding files from " << currRepo << " to staging...\n";

            // Add files
            system("git add -A");

            std::cout << "Done.";
            break;

        case 3:
            // Mark if repository has changes
            checkForChange(hasChange);

            // If so, commit
            commitWithMsg(hasChange, currRepo);

            break;

        case 4:
            std::cout << "\n-------------------------------------\n";
            std::cout << "Pushing changes to remote of " << currRepo << "...\n";

            // Push to remote with no output
            system("git push | true");

            std::cout << "Done.";
            break;

        case 5:
            std::cout << "Launching bash environment...\n";

            // Launch shell
            system("bash");

            std::cout << "Exiting Shell environment...\n";

            // Reset and return, do not want a shell for every repository
            currPath.erase(currPath.size() - currRepo.length());
            std::filesystem::current_path(currPath);

            // Exit function
            return;

        case 6:
            // Provide user option to edit gitignore
            std::cout << "Launching .gitignore editor...";

            // Reset path before entering environment
            currPath.erase(currPath.size() - currRepo.length());
            std::filesystem::current_path(currPath);

            // Enter gitignore editor environment
            addIgnore(repoList, currPath);

            // Exit function
            return;

        case 7:
            std::cout << "\n-------------------------------------\n";
            std::cout << "Pulling changes from remote of " << currRepo << "...\n";

            // Pull from fetched data
            system("git pull");

            std::cout << "Done.";
            break;

        case 8:
            // Exit program
            exit(0);
        }

        // Cut off attachment and navigate back to reference directory
        currPath.erase(currPath.size() - currRepo.length());
        std::filesystem::current_path(currPath);
    }
}