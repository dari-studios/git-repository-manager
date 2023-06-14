#include "addignore.h"

// List all directories and probe those directories for .git folders
// Unfortunately this only goes down by 1 level
void findRepos(std::vector<std::string> &repoList, std::string currPath)
{
    // For each directory entry found, scan them for a .git folder
    for (auto dir : std::filesystem::directory_iterator(currPath))
    {
        // Variable to store the path to check
        std::string dirCheck = dir.path().string() + "\\.git";

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

    if (hasChange)
    {
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
        std::filesystem::remove(std::filesystem::current_path().string() + "\\COMMITMSG.txt");
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
    std::filesystem::remove(std::filesystem::current_path().string() + "\\gitstatus.txt");
}

// General function for commands
void gitComm(std::vector<std::string> repoList, std::string currPath, int option)
{
    // Boolean for detecting changes
    bool hasChange = false;

    // For every repository in the list, we will execute the requested command
    // Note that some commands are run-once
    for (std::string currRepo : repoList)
    {
        // Navigate to repo directory
        currPath.append(currRepo);
        // std::cout << "Target dir: " << currPath << "\n";
        std::filesystem::current_path(currPath);

        // Execute given command
        switch (option)
        {
        case 0:
            // Fetch
            std::cout << "Fetching from " << currRepo << "...\n";
            system("git fetch");
            std::cout << "Done.\n\n";
            break;

        case 1:
            // Get status
            std::cout << "\nStatus of " << currRepo << ": \n\n";
            system("git status");
            std::cout << "\n-------------------------------------";
            break;

        case 2:
            // Add files to staging
            std::cout << "Adding files from " << currRepo << " to staging...\n";
            system("git add -A");
            break;

        case 3:
            // Commit changes

            // Mark repositories with changes
            checkForChange(hasChange);
            commitWithMsg(hasChange, currRepo);
            break;

        case 4:
            // Push to remote
            std::cout << "Pushing changes to remote of " << currRepo << "...\n";
            system("git push | true");
            std::cout << "Done.\n\n";
            break;

        case 5:
            // Launch shell environment
            std::cout << "Launching bash environment...\n";
            system("bash");
            std::cout << "Exiting Shell environment...\n";

            // Reset and return, do not want a shell for every repository
            currPath.erase(currPath.size() - currRepo.length());
            std::filesystem::current_path(currPath);
            return;

        case 6:
            // Provide user option to edit gitignore
            std::cout << "Launching .gitignore editor...";

            // Reset path before entering environment
            currPath.erase(currPath.size() - currRepo.length());
            std::filesystem::current_path(currPath);
            addIgnore(repoList, currPath);
            // Reset
            return;

        case 7:
            // Pull from remotes
            std::cout << "Pulling changes from remote of " << currRepo << "...\n";

            system("git pull");

            break;

        case 8:
            // Exit program
            exit(0);
        }
        // Cut off attachment and navigate back to base
        currPath.erase(currPath.size() - currRepo.length());
        // std::cout << "Reference dir: " << currPath << "\n";
        std::filesystem::current_path(currPath);
    }
}