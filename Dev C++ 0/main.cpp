#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <ctime>
#include <sstream>

using namespace std;

char HOST[] = "127.0.0.1";
char USER[] = "root";
char PASS[] = "root";
char DB[] = "menu";

// Function to get the current time slot (0 for morning, 1 for evening)
string getCurrentTimeSlot() {
    // Implement your logic to determine the current time slot here
    // For simplicity, let's assume morning if the hour is before 12, else evening
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
    int hour = now->tm_hour;

    // Using a stringstream for conversion
    stringstream ss;
    ss << ((hour < 12) ? 0 : 1);

    return ss.str();
}

// Function to display menu based on time slot
void displayMenu(MYSQL* obj, const string& timeSlot) {
    MYSQL_RES* result;
    MYSQL_ROW row;

    // Convert timeSlot to integer without using stoi
    stringstream convert(timeSlot);
    int timeSlotInt;
    convert >> timeSlotInt;

    string query = "SELECT name, price FROM Dish WHERE time_slot = " + timeSlot;
    
    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << mysql_fetch_field_direct(result, i)->name << ": " << row[i] << " ";
                }
                cout << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

// Function to display details of a specific dish
void displayDishDetails(MYSQL* obj, const string& dishId) {
    MYSQL_RES* result;
    MYSQL_ROW row;

    // Convert dishId to integer without using stoi
    stringstream convert(dishId);
    int dishIdInt;
    convert >> dishIdInt;

    string query = "SELECT * FROM Dish WHERE id = " + dishId;

    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << mysql_fetch_field_direct(result, i)->name << ": " << row[i] << " ";
                }
                cout << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

void displayDishesWithIngredients(MYSQL* obj, const string& timeSlot) {
    MYSQL_RES* result;
    MYSQL_ROW row;
    
    stringstream convert(timeSlot);
    int timeSlotInt;
    convert >> timeSlotInt;


    string query = "SELECT Dish.name AS dish_name, "
                   "Ingredient.name AS ingredient_name, "
                   "Ingredient.quantity, Ingredient.unit "
                   "FROM Dish, Dish_Ingredient, Ingredient where Dish.id = Dish_Ingredient.dish_id and Dish_Ingredient.ingredient_id = Ingredient.id and time_slot = " + timeSlot;
                
    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << mysql_fetch_field_direct(result, i)->name << ": " << row[i] << " ";
                }
                cout << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

// Function to display details of a specific ingredient
void displayIngredientDetails(MYSQL* obj, const string& ingredientId) {
    MYSQL_RES* result;
    MYSQL_ROW row;

    // Convert ingredientId to integer without using stoi
    stringstream convert(ingredientId);
    int ingredientIdInt;
    convert >> ingredientIdInt;

    string query = "SELECT * FROM Ingredient WHERE id = " + ingredientId;

    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < num_fields; i++) {
                    cout << mysql_fetch_field_direct(result, i)->name << ": " << row[i] << " ";
                }
                cout << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

MYSQL_RES* executeQuery(MYSQL* obj, const string& query) {
    if (mysql_query(obj, query.c_str())) {
        cout << "Error: " << mysql_error(obj) << endl;
        return 0;
    }

    MYSQL_RES* res = mysql_store_result(obj);
    if (res == 0) {
        cout << "Error: " << mysql_error(obj) << endl;
    }

    return res;
}

string sanitizeInput(const string& input) {
    string sanitized;
    for (int i = 0; i < input.length(); i++) {
        char c = input[i];
        if (isalnum(c) || c == ' ') {
            sanitized += c;
        }
    }
    return sanitized;
}

void displayDishesByCategory(MYSQL* obj) {
    // Prompt the user to enter a category ID
    cout << "Dish categories:" << endl;
    cout << "1. Appetizer" << endl;
    cout << "2. Main Course " << endl;
    cout << "3. Dessert" << endl;
    cout << "Enter category you want to search (enter 1, 2 or 3): ";
    int categoryId;
    cin >> categoryId;  // Use cin to read an integer

    // Convert the category ID to a string for the query
    stringstream ss;
    ss << categoryId;
    string categoryIdStr = ss.str();

    // First, check if the category exists in the database
    string checkQuery = "SELECT COUNT(*) FROM Category WHERE id = " + categoryIdStr;
    MYSQL_RES* checkRes = executeQuery(obj, checkQuery);
    MYSQL_ROW checkRow = mysql_fetch_row(checkRes);
    int count = atoi(checkRow[0]);
    mysql_free_result(checkRes);

    if (count == 0) {
        // If the category does not exist, print an error message and return
        cout << "Error: The category with ID '" << categoryId << "' does not exist in the database." << endl;
        return;
    }

    string currentTimeSlot = getCurrentTimeSlot();
    string query = "SELECT * FROM Dish WHERE category_id = " + categoryIdStr + " AND time_slot = " + currentTimeSlot;
    // Print out the query for debugging
    cout << "Executing query: " << query << endl;
    MYSQL_RES* res = executeQuery(obj, query);
    if (res != 0) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != 0) {
            cout << "ID: " << row[0] << endl;
            cout << "Name: " << row[1] << endl;
            cout << "Category ID: " << row[2] << endl;
            cout << "Price: " << row[3] << endl;
            cout << "Time Slot: " << row[4] << endl;
            cout << "-------------------" << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "No dishes found for this category." << endl;
    }
}

int main() {
    MYSQL* obj;

    if (!(obj = mysql_init(0))) {
        cout << "Error" << endl;
    } else {
        if (!mysql_real_connect(obj, HOST, USER, PASS, DB, 3306, NULL, 0)) {
            cout << "Error" << endl;
            cout << mysql_error(obj) << endl;
        } 
		else {
            cout << "Logged in" << endl;

            // Determine the current time slot
            string currentTimeSlot = getCurrentTimeSlot();
            
           // Display menu based on the current time slot	
		char choice;
	    do {
	    	cout << "Menu for ";
			if (currentTimeSlot == "0") {
	  			  cout << "Morning" << endl;
			} else {
	   				 cout << "Evening" << endl;
			}
			displayMenu(obj, currentTimeSlot);
			cout << endl;
	        std::cout << "1. Search by category\n";
	        std::cout << "2. Search by origin\n";
	        std::cout << "3. Dish ingredients detail\n";
	        std::cout << "4. Exit\n";
	        std::cout << "Enter your choice: ";
	        std::cin >> choice;
	
		    switch (choice) {
			    case '1': {
			    	displayDishesByCategory(obj);
			        break;
			    }
		        case '2': {
					break;
				}
		        case '3':{
					break;
				}
		        case '4': {
					std::cout << "Exiting program...\n";
					break;
				}
		        default: {
		            std::cout << "Invalid choice. Please try again.\n";
		            break;
		        }
			}
	    } while (choice != '4');

        }
        mysql_close(obj);
    }
    return 0;
}
