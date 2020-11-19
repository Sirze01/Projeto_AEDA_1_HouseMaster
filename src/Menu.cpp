
/*
#include <iostream>
#include "Menu.h"
#include <limits>


int Menu::initialMenu()
{
    int role_choice = -1;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| Choose your role:              |" << std::endl;
    std::cout << "| [1] Administration             |" << std::endl;
    std::cout << "| [2] Collaborator               |" << std::endl;
    std::cout << "| [3] Client                     |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [0] Exit                       |" << std::endl;
    std::cout << "|________________________________|" << std::endl;

    std::cin >> role_choice;

    while (true) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Make sure you are entering a valid number: " << std::endl;
                std::cin >> role_choice;
            }
        } else if (role_choice < 1 || role_choice > 3) {
            std::cout << "Invalid choice. Make sure you chose one of the available options: " << std::endl;
            std::cin >> role_choice;
        } else {
            break;
        }
    }


    switch (role_choice) {
        case administrator:
            checkAdmin();
            break;
        case collaborator:
            break;
        case client:
            break;
    }

*/
/*
    while (role_choice == -1)
    {
        if (std::cin.fail())
        {
            role_choice = -1;
            std::cout << "Invalid choice. Please write a number: " << std::endl;
        }
        else if (role_choice < 0 || role_choice > 3)
        {
            role_choice = -1;
            std::cout << "Invalid choice. Please write the correct number: " << std::endl;
        }
        else
        {
            switch (role_choice)
            {
                case 1: individualType = 1; break;
                case 2: individualType = 2; break;
                case 3: individualType = 3; break;
            }
        }
    }

    *//*

    return role_choice;
}

void Menu::exitMenu()
{
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "|                                |" << std::endl;
    switch (individualType)
    {
        case 1:
                std::cout << "|             Saving             |" << std::endl;
                std::cout << "|          your changes          |" << std::endl;
                break;
        case 2:
            std::cout << "|           Thank you            |" << std::endl;
            std::cout << "|      for working with us!      |" << std::endl;
            break;
        case 3:
            std::cout << "|           Thank you            |" << std::endl;
            std::cout << "|   for believing in our work    |" << std::endl;
            break;
    }
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
}

int Menu::checkAdmin()
{
    std::string password;
    int MAX_TRIES = 3;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| Insert                         |" << std::endl;
    std::cout << "| Administration                 |" << std::endl;
    std::cout << "| Password:                      |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin >> password;
    for (int i = 0; i <= MAX_TRIES; i++)
    {
        if (password == "admin")
        {
            adminMenu();
            return 0;
        } else
        {
            std::cout << "Invalid password! Try again: " << std::endl;
            std::cin >> password;
        }
    }
    std::cout << "Too many tries! Logging out..." << std::endl;
    return -1;
}

int Menu::checkCollab()
{
    int id = 0;
    char join;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| Insert                         |" << std::endl;
    std::cout << "| ID:                            |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin >> id;
    for (int i = 0; i < 3; i++)
    {
        if (id  )     // search Id
        {
            return 0;
        } else
        {
            std::cout << "Invalid ID! Try again: " << std::endl;
        }
    }
    do {
        std::cout << "You are not a collaborator yet. Do you want to join? (Y/N): " << std::endl;
        std::cin >> join;
        if (std::cin.fail())
        {
            std::cout << "Invalid answer. Please try again: " << std::endl;
        }
        else if (join == 'Y' || join == 'y')
        {
            return 1;
        }
        else if (join == 'N' || join == 'n')
        {
            return -1;
        }
        else
        {
            std::cout << "Invalid answer. Please try again: " << std::endl;
        }
    } while (join != 'Y' || join != 'y' || join != 'N' || join != 'n');
    return -1;
}
int Menu::checkClient()
{
    int id = 0;
    char join;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| Insert                         |" << std::endl;
    std::cout << "| ID:                            |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin >> id;
    for (int i = 0; i < 3; i++)
    {
        if (id)     // search Id
        {
            return 0;
        } else
        {
            std::cout << "Invalid ID! Try again: " << std::endl;
        }
    }
    do {
        std::cout << "You are not a registered client yet. Do you want to access our services? (Y/N): " << std::endl;
        std::cin >> join;
        if (std::cin.fail())
        {
            std::cout << "Invalid answer. Please try again: " << std::endl;
        }
        else if (join == 'Y' || join == 'y')
        {
            return 1;
        }
        else if (join == 'N' || join == 'n')
        {
            return -1;
        }
        else
        {
            std::cout << "Invalid answer. Please try again: " << std::endl;
        }
    } while (join != 'Y' || join != 'y' || join != 'N' || join != 'n');
    return -1;
}


int Menu::adminMenu()
{
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "|                 Administration |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [1] Financial data             |" << std::endl;
    std::cout << "| [2] Clients data               |" << std::endl;
    std::cout << "| [3] Collaborators data         |" << std::endl;
    std::cout << "| [4] Services data              |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [0] Exit                       |" << std::endl;
    std::cout << "|________________________________|" << std::endl;

    // Ver parte financeira
    // Ver clientes
    // Ver colaboradores
    // Ver avaliações
    // Ver colaboradores com menores avaliações
    return 0;
}

int Menu::collabMenu()
{
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << " Welcome back, !\n\n" << std::endl;
    std::cout << " [1] Evaluations" << std::endl;
    std::cout << " [2] Services" << std::endl;
    std::cout << " [3] Scheduled Services" << std::endl;
    std::cout << " [4] Finished Services" << std::endl;
    std::cout << " [5] Financial State\n" << std::endl;
    std::cout << " [0] Exit" << std::endl;

    // Ver as avaliações
    // Ver serviços que presta e alterar os mesmos
    // Ver serviços agendados
    // Ver serviços feitos
    // Ver dinheiro ganho
    return 0;
}

int Menu::clientMenu()
{
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << " Welcome back, "  << "!\n\n" << std::endl;

    // Ver serviços agendados
    // Ver serviços oferecidos pela house master
    return 0;
}

int Menu::newClient()
{
    std::string name;
    int id, nif;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "We need to collect some data about you. Please enter the following information:\nName: " << std::endl;
    do
    {
        std::cin >> name;
        if (std::cin.fail())
        {
            std::cout << "Invalid name. Try again: " << std::endl;
        }
    } while (std::cin.fail());
    std::cout << "Id number: " << std::endl;
    do
    {
        std::cin >> id;
        if (std::cin.fail())
        {
            std::cout << "Invalid format. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() < 8)
        {
            std::cout << "Id too short. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() > 8)
        {
            std::cout << "Too many digits. Try again:" << std::endl;
        }
    } while (std::cin.fail() || std::to_string(id).length() != 8);
    std::cout << "NIF number: " << std::endl;
    do
    {
        std::cin >> nif;
        if (std::cin.fail())
        {
            std::cout << "Invalid format. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() < 9)
        {
            std::cout << "Id too short. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() > 9)
        {
            std::cout << "Too many digits. Try again:" << std::endl;
        }
    } while (std::cin.fail() || std::to_string(id).length() != 9);
    std::cout << "\n\n\n Thanks for believing on our work. Let's start!" << std::endl;
    return 0;
}

int Menu::newCollab()
{
    std::string name;
    int id, nif;
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "We need to collect some data about you. Please enter the following information:\nName: " << std::endl;
    do
    {
        std::cin >> name;
        if (std::cin.fail())
        {
            std::cout << "Invalid name. Try again: " << std::endl;
        }
    } while (std::cin.fail());
    std::cout << "Id number: " << std::endl;
    do
    {
        std::cin >> id;
        if (std::cin.fail())
        {
            std::cout << "Invalid format. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() < 8)
        {
            std::cout << "Id too short. Try again:" << std::endl;
        }
        else if (std::to_string(id).length() > 8)
        {
            std::cout << "Too many digits. Try again:" << std::endl;
        }
    } while (std::cin.fail() || std::to_string(id).length() != 8);
    std::cout << "From the following services select the ones you are capable of doing: " << std::endl;
    // print dos serviços e meter maneira de escolher
    std::cout << "Are you a professional in any type of service? " << std::endl;
    // forma de dizer que é pro em X trabalho
    std::cout << "\n\n\n Thanks for joining de team! We are excited to work with you, " << name << "!" << std::endl;

}*/

#include "Menu.h"

Menu::Menu(std::string prompt, std::map<std::string, std::function<void()>> options)
        : _prompt(std::move(prompt)), _options(std::move(options)), _choice() {

}

void Menu::show() {

    unsigned count = 1;
    unsigned back = _options.size() + 1;

    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << _prompt << " |" << std::endl;
    std::cout << "|                                |" << std::endl;

    for (const auto &option : _options) {
        std::cout << "| [" << count << "] " << std::setw(27) << std::left << option.first << "|" << std::endl;
        count ++;
    }

    std::cout << "|                                |" << std::endl;
    std::cout << "| [0] EXIT (no save)" << "   [" << back << "] BACK  |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
}

void Menu::select() {

    unsigned choice = 0;
    std::cout << "Your choice: \n";
    std::cin >> choice;

    while (true) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Make sure you are entering a valid number: " << std::endl;
                std::cin >> choice;
            }
        } else if (choice > _options.size() + 1) {
            std::cout << "Invalid choice. Make sure you chose one of the available options: " << std::endl;
            std::cin >> choice;
        } else {
            break;
        }
    }
    _choice = choice;
}

void Menu::execute(bool &running) {

    if (!_choice) exit(0);
    if (_choice == _options.size() + 1) {
        running = false;
        return;
    }
    std::vector<std::string> keys{};
    std::transform(_options.begin(), _options.end(), back_inserter(keys), [](const std::pair<std::string, std::function<void()>>& funcs){return funcs.first;});
    _options.at(keys.at(_choice-1))();

}

