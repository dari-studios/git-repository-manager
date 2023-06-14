

int findCommand(std::string option)
{
    // unfortunately due to limitations of the switch statement,
    // we must must match strings to produce an integer to pass instead
    std::string commands[3] = {"status", "push", "pull"};

    // integer to pass down
    int command = 0;

    // iterate through list of commands until a match is found
    for (std::string str : commands)
    {
        command++;
        if (str == option)
        {
            break;
        }
    }
}

void parseStatus(std::fstream gitOutput)
{
}

void gitParse(std::string option)
{
    std::fstream gitOutput;

    gitOutput.open("gitoutput.txt");

    // convert command to int and pass it down
    switch (findCommand(option))
    {
    case 1:
        break;
    }
}
