/*
    This is the main executable file of git-repository-manager
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

#include "options.h"

int main()
{
    // Variable to check for first run
    bool firstRun = true;

    // Store current path for later use
    std::string currPath = std::filesystem::current_path().string();

    // Initialize object which contains all data
    repoData repoData;

    // Main loop
    while (true)
    {
        // Clear out any old entries to avoid corruption
        repoData.repoList.clear();

        // Find valid repositories
        findRepos(repoData.repoList, currPath);

        // If there are no repos, exit
        if (repoData.repoList.empty())
        {
            std::cout << "No repositories detected, exiting...\n";
            return 0;
        }

        // Run some commands on first run only
        if (firstRun)
        {
            std::cout << "Refreshing repositories...\n";
            // Fetch from remotes
            gitComm(repoData.repoList, repoData.repoStatus, currPath, 0);

            // List status
            gitComm(repoData.repoList, repoData.repoStatus, currPath, 1);

            firstRun = false;
        }

        // Entry message
        std::cout << "\n-------------------------------------\n";
        std::cout << "Git Repository Manager v1.0\n";

        // Prompt and run command loop
        while (true)
        {
            int choice;

            // Prompt user for choice
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

            // ignore newline character
            std::cin.ignore();

            if (choice < 1 || choice > 8)
            {
                std::cout << "Error! Please enter a valid option\n";
                choice = 0;
                continue;
            }
            gitComm(repoData.repoList, repoData.repoStatus, currPath, choice);
            break;
        } //..end prompt & run loop

    } //.. end main loop
}