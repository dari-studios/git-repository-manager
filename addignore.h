/*
    This file provides the .gitignore editor for git-repository-manager
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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Struct to store list of repositories
// Also stores the hasChange flag
struct repoData
{
    std::vector<std::string> repoList;
    bool repoStatus = false;
};

// Lists repositories and allow user to choose one to edit
// This is effectively a mini program
void addIgnore(std::vector<std::string> &repoList, std::string currPath)
{
    // Main loop for the ignore program
    while (true)
    {
        // iterator for displaying options
        int i = 0;

        int choice;

        std::cout << "GRM .gitignore editor v1.0\n";
        std::cout << "Please select which repository you would like to edit (or type 0 to exit):";

        // List repositories here...
        for (std::string repo : repoList)
        {
            std::cout << "\n1 > " << repo;
        }

        // Prompt user for choice
        while (true)
        {
            std::cout << "\nYour option: ";
            std::cin >> choice;

            // Ignore newline character
            std::cin.ignore();

            // Verify entry is within bounds
            if (choice < 0 || choice > repoList.size())
            {
                std::cout << "Error! Please enter a valid option\n";
                choice = -1;
                continue;
            }

            break;
        } //..end while

        // Check for exit code
        if (choice == 0)
        {
            return;
        }

        // Navigate to corresponding directory based on user choice
        currPath.append(repoList.at(choice - 1));
        std::filesystem::current_path(currPath);

        // Open the file in append mode to avoid rewriting existing entries
        std::fstream gitIgnore;
        gitIgnore.open(".gitignore", std::fstream::app);

        std::string toIgnore;

        // Takes user-specificed paths and writes them to .gitignore
        while (true)
        {
            // Reset files to be ignored
            toIgnore = "";

            // Get user input
            std::cout << "Enter the path of of the file/directory you want to ignore (type !? to exit): ";
            std::getline(std::cin, toIgnore);

            // Check for exit code
            if (toIgnore == "!?")
            {
                break;
            }

            gitIgnore << toIgnore << "\n";
        } //.. end while

        // Cleanup
        currPath.erase(currPath.size() - repoList.at(choice - 1).length());
        std::filesystem::current_path(currPath);

        gitIgnore.close();
    } //.. end while
}