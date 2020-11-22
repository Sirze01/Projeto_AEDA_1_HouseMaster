
#include "Menu.h"

/**
 * @brief menu's constructor
 * @param prompt the prompt
 * @param options the options
 */
Menu::Menu(std::string prompt, std::map<std::string, std::function<void()>> options)
        : _prompt(std::move(prompt)), _options(std::move(options)), _choice() {

}

/**
 * @brief shows the menu
 */
void Menu::show() {

    unsigned count = 1;
    unsigned long back = _options.size() + 1;

    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << _prompt << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;

    for (const auto &option : _options) {
        std::cout << "| [" << std::right << std::setw(2) << count << "] " << std::setw(46) << std::left << option.first << "|" << std::endl;
        count++;
    }

    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [ 0] EXIT (no save)" << "                     [" << std::setw(2) << std::right << back
              << "] BACK  |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
}

/**
 * @brief selects an option
 */
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

/**
 * @brief executes the functions
 * @param running
 */
void Menu::execute(bool &running) {

    if (!_choice) {
        std::cout << "Leaving without saving...\n";
        exit(0);
    }
    if (_choice == _options.size() + 1) {
        running = false;
        return;
    }
    std::vector<std::string> keys{};
    std::transform(_options.begin(), _options.end(), back_inserter(keys),
                   [](const std::pair<std::string, std::function<void()>> &funcs) { return funcs.first; });
    _options.at(keys.at(_choice - 1))();

}

