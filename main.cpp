#include "options.h"

int main()
{
    bool firstRun = true;
    std::fstream mainDir;

    // Store current path for later use
    std::string currPath = std::filesystem::current_path().string();

    // Create vector to store list of repositories and their status
    std::vector<std::string> repoList;

    while (true)
    {
        std::cout << "Git Repository Manager v1.0\n";

        // Clear out any old entries to avoid corruption
        repoList.clear();

        // Startup routine
        // Find valid repositories
        findRepos(repoList, currPath);

        if (firstRun)
        {
            std::cout << "Refreshing repositories...\n";
            // Fetch from remotes
            gitComm(repoList, currPath, 0);

            // List status
            gitComm(repoList, currPath, 1);

            firstRun = false;
        }

        while (true)
        {
            int choice;

            std::cout << "\nWhat would you like to do?\n\n";
            std::cout << "\
            1) Get status \n\
            2) Add all files to staging \n\
            3) Commit all changes with a message \n\
            4) Push to all remotes \n\
            5) Launch shell \n\
            6) Add files to ignore \n\
            7) Pull from all remotes \n\
            8) Exit\n";

            std::cout << "\nEnter option: ";
            std::cin >> choice;

            //ignore newline character
            std::cin.ignore();

            if (choice < 1 || choice > 8)
            {
                std::cout << "Error! Please enter a valid option\n";
                choice = 0;
                continue;
            }
            gitComm(repoList, currPath, choice);
            break;
        }
    }
}
