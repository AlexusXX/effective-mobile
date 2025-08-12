//
// Created by alexus on 12.08.2025.
//
#include <format>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <unordered_map>


struct Meal {
    int id;
    std::string name;
    double price;
};
std::pmr::unordered_map<int /*mealId*/, Meal> meals;

struct Order {
    std::string id;
    std::vector<std::pair<int /*mealId*/, int /*quant*/>> items;
};
std::map<std::string /*orderId*/, Order> orders;

/**
 * Display list of meals.
 */
void showMenu() {
    if (meals.empty()) {
        std::cout << "No meals available.\n";
        return;
    }

    std::cout << "\nAvailable meals:\n";
    for (const auto& [id, name, price] : meals | std::views::values) {
        std::cout << id << ": " << name << " - $" << std::fixed << std::setprecision(2) << price << "\n";
    }
}

/**
 * @brief Create new order.
 * @return new order ID starts form 1000.
 */
std::string createNewOrder() {
    static int nextOrderId = 1000;
    std::string orderId = std::to_string(nextOrderId++);
    orders[orderId] = {orderId, {}};
    return orderId;
}

/**
 * @brief Add specified quantity meal to specified order.
 * @param orderId order to add meal.
 * @param mealId meal to add.
 * @param quantity meal quantity.
 * @throw std::runtime_error when order ot meal not found.
 * @throw std::runtime_error when quantity is negative.
 */
void addMealToOrder(const std::string& orderId,
                    int mealId,
                    int quantity) {
    const auto it = orders.find(orderId);
    if (it == orders.end()) {
        throw std::runtime_error("Order not found. ID: " + orderId);
    }

    if (!meals.contains(mealId)) {
        throw std::runtime_error("Meal not found. ID: " + std::to_string(mealId));
    }

    if (quantity <= 0) {
        throw std::runtime_error("Quantity must be a positive integer.");
    }

    it->second.items.emplace_back(mealId, quantity);
}

/**
 * @brief Register a new meal.
 * @param id id to register.
 * @param name meal name.
 * @param price meal price.
 * @throw std::runtime_error when meal with specified id is exists.
 */
void registerNewMeal(int id, const std::string &name, const double price) {
    if (const auto it = meals.find(id); it != meals.end())
        throw std::runtime_error(std::format("Meal with id {} is already exists. Meal name is {}", id, it->second.name));
    meals.emplace(id, Meal{id, name, price});
}

/**
 * @brief List meals in order.
 * @param orderId order id to list.
 * @throw std::runtime_error when order not found.
 */
void showOrder(const std::string& orderId) {
    const auto it = orders.find(orderId);
    if (it == orders.end()) {
        throw std::runtime_error("Order not found. ID: " + orderId);
    }

    const auto& [id, items] = it->second;
    std::cout << "\nOrder ID: " << orderId << "\n";
    for (const auto& [mealId, mealQuant] : items) {
        std::cout << "- Meal Name: " << meals.at(mealId).name << ", Quantity: " << mealQuant << "\n";
    }
}

void processMenuCommand(const std::vector<std::string> &tokens) {
    if (tokens.size() < 2) {
        std::cout << "Invalid command. Use 'menu show'.\n";
        return;
    }
    if (tokens[1] == "show") {
        showMenu();
    } else {
        std::cout << "Unknown subcommand. Use 'menu show'.\n";
    }
}

void processOrderCommand(const std::vector<std::string> &tokens) {
    if (tokens.size() < 2) {
        std::cout << "Invalid command. Use 'order new' or 'order <id> add|show'.\n";
        return;
    }

    if (tokens[1] == "new") {
        const std::string orderId = createNewOrder();
        std::cout << "New order created with ID: " << orderId << "\n";
    } else {
        // Process subcommands
        if (tokens.size() < 3) {
            std::cout << "Invalid command. Use 'order <id> add|show'.\n";
            return;
        }

        auto &orderId = tokens[1];
        if (auto &subcommand = tokens[2]; subcommand == "add") {
            if (tokens.size() < 5) {
                std::cout << "Invalid command. Use 'order <id> add <meal-id> <quantity>'.\n";
                return;
            }

            int mealId;
            try {
                mealId = stoi(tokens[3]);
            } catch (...) {
                std::cout << "Meal ID must be a valid integer.\n";
                return;
            }
            int quantity;
            try {
                quantity = stoi(tokens[4]);
            } catch (...) {
                std::cout << "Quantity must be a valid integer.\n";
                return;
            }

            addMealToOrder(orderId, mealId, quantity);
            std::cout << "Added " << quantity << "x" << meals.at(mealId).name
                 << " to order #" << orderId << ".\n";
        } else if (subcommand == "show") {
            showOrder(orderId);
        } else {
            std::cout << "Unknown subcommand. Use 'add' or 'show'.\n";
        }
    }
}

int main() {
    try {
        registerNewMeal(1, "Spaghetti", 5.99);
        registerNewMeal(2, "Pizza", 8.99);
        registerNewMeal(3, "Salad", 4.99);
    }catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }

    std::string line;
    while (true) {
        std::cout << "> ";
        getline(std::cin, line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        try {
            //Command processing
            if (const auto &command = tokens[0]; command == "menu") {
                processMenuCommand(tokens);
            } else if (command == "order") {
                processOrderCommand(tokens);
            } else if (command == "exit") {
                break;
            } else {
                std::cout << "Invalid command. Use 'menu show', 'order new', 'order <id> add|show', 'exit'.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }

    return 0;
}
