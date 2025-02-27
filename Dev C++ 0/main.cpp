#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

char HOST[] = "127.0.0.1";
char USER[] = "root";
char PASS[] = "root";
char DB[] = "emenu";

// Function to get the current time slot (0 for morning, 1 for evening)
string getCurrentTimeSlot() {
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

    stringstream convert(timeSlot);
    int timeSlotInt;
    convert >> timeSlotInt;

    string query = "SELECT id, name, price FROM Dish WHERE time_slot = " + timeSlot;

    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            // Display header
            cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << endl;
            
            // Display separator line
            cout << setfill('-') << setw(35) << "-" << setfill(' ') << endl;

            while ((row = mysql_fetch_row(result))) {
                // Display data
                cout << setw(5) << row[0] << setw(20) << row[1] << setw(10) << row[2] << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

void displayDishesWithIngredients(MYSQL* obj, const string& dishId, const string& timeSlot) {
    MYSQL_RES* result;
    MYSQL_ROW row;

    stringstream convert(dishId);
    int dishIdInt;
    convert >> dishIdInt;

    string query = "SELECT Ingredient.name AS ingredient_name, "
                   "Ingredient.quantity, Ingredient.unit "
                   "FROM Dish, Dish_Ingredient, Ingredient "
                   "WHERE Dish.id = Dish_Ingredient.dish_id "
                   "AND Dish_Ingredient.ingredient_id = Ingredient.id "
                   "AND Dish.id = " + dishId +
                   "AND time_slot = " + timeSlot;

    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            // Display header
            cout << setw(20) << "Ingredient Name" << setw(10) << "Quantity" << setw(10) << "Unit" << endl;

            // Display separator line
            cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;

            while ((row = mysql_fetch_row(result))) {
                // Display data
                cout << setw(20) << row[0] << setw(10) << row[1] << setw(10) << row[2] << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "No data retrieved" << endl;
        }
    }
}

void searchDishDetailsByOrigin(MYSQL* obj, const string& originName, const string& timeSlot) {
    MYSQL_RES* result;
    MYSQL_ROW row;

    // Construct and execute the query to search dish details by origin
    string query = "SELECT Dish.name, Dish.price "
                   "FROM Dish, Dish_Origin, Origin "
                   "WHERE Dish.id = Dish_Origin.dish_id "
                   "AND Dish_Origin.origin_id = Origin.id "
                   "AND Origin.name = '" + originName + "' "
                   "AND time_slot = " + timeSlot;

    if (mysql_query(obj, query.c_str())) {
        cout << "Query Error" << endl;
        cout << mysql_error(obj) << endl;
    } else {
        result = mysql_store_result(obj);
        if (result) {
            int num_fields = mysql_num_fields(result);

            // Display header
            cout << setw(30) << "Dish Details Based on Origin:" << endl;
            cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

            // Display data
            cout << left << setw(30) << "Name" << setw(10) << "Price" << endl;
            cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

            while ((row = mysql_fetch_row(result))) {
                // Display dish details
                cout << left << setw(30) << row[0] << setw(10) << row[1] << endl;
            }

            // Display footer
            cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

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
    int categoryId;
    int count;
    string categoryIdStr;  // Declare categoryIdStr here

    do {
        // Prompt the user to enter a category ID
        cout << "Enter category ID (1 for Appetizer, 2 for Main Course, 3 for Dessert): ";
        cin >> categoryId;  // Use cin to read an integer

        // Convert the category ID to a string for the query
        stringstream ss;
        ss << categoryId;
        categoryIdStr = ss.str();  // Assign the string to categoryIdStr

        // First, check if the category exists in the database
        string checkQuery = "SELECT COUNT(*) FROM Category WHERE id = " + categoryIdStr;
        MYSQL_RES* checkRes = executeQuery(obj, checkQuery);
        MYSQL_ROW checkRow = mysql_fetch_row(checkRes);
        count = atoi(checkRow[0]);
        mysql_free_result(checkRes);

        if (count == 0) {
            // If the category does not exist, print an error message
            cout << "Error: The category with ID '" << categoryId << "' does not exist in the database." << endl;
        }
    } while (count == 0);  // Repeat the loop if the category does not exist

    // If the category exists, proceed with the rest of your code...
    string currentTimeSlot = getCurrentTimeSlot();
    string query = "SELECT * FROM Dish WHERE category_id = " + categoryIdStr + " AND time_slot = " + currentTimeSlot;
    
	MYSQL_RES* res = executeQuery(obj, query);
	if (res != NULL) {
	    if (mysql_num_rows(res) > 0) {
	        MYSQL_ROW row;
	        while ((row = mysql_fetch_row(res)) != NULL) {
	            cout << "ID: " << row[0] << endl;
	            cout << "Name: " << row[1] << endl;
	            cout << "Category ID: " << row[2] << endl;
	            cout << "Price: " << row[3] << endl;
	            cout << "-------------------" << endl;
	        }
	    } else {
	        cout << "No dishes found for this category." << endl;
	    }
	    mysql_free_result(res);
	} else {
	    cout << "Error executing query." << endl;
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
        } else {
            cout << "Logged in" << endl;

            // Determine the current time slot
            string currentTimeSlot = getCurrentTimeSlot();

            // Display menu based on the current time slot
char choice;
    do {
    	cout << "\n------------Menu for ";
		if (currentTimeSlot == "0") {
  			cout << "Morning-------" << endl;
		} else {
   			cout << "Evening-------" << endl;
		}
		displayMenu(obj, currentTimeSlot);
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
				// Search by origin
			    string originName;
			    cout << "Enter origin name: ";
			    cin.ignore();  // Ignore any previous newline character in the input buffer
			    getline(cin, originName);  // Read the entire line, including spaces
						
			    // Check if the entered origin name exists in the database
			    string checkOriginQuery = "SELECT COUNT(*) FROM Origin WHERE name = '" + originName + "'";
			    MYSQL_RES* checkOriginRes = executeQuery(obj, checkOriginQuery);
			    MYSQL_ROW checkOriginRow = mysql_fetch_row(checkOriginRes);
			    int originCount = atoi(checkOriginRow[0]);
			    mysql_free_result(checkOriginRes);
			
			    if (originCount > 0) {
			        // Origin exists, proceed with the search
			        searchDishDetailsByOrigin(obj, originName, currentTimeSlot);
			    } else {
			        // Origin does not exist, print an error message
			        cout << "Error: The entered origin name '" << originName << "' does not exist in the database." << endl;
			    }
    			break;
			}
	        case '3':{
	        	bool validDishId = false;
				string dishId;
			    do {
			        cout << "Enter the Dish ID: ";
			        cin >> dishId;
			
			        // Check if the entered Dish ID is in the current menu
			        stringstream queryCheck;
			        queryCheck << "SELECT COUNT(*) FROM Dish WHERE id = " << dishId << " AND time_slot = " << currentTimeSlot;
			        if (mysql_query(obj, queryCheck.str().c_str())) {
			            cout << "Query Error" << std::endl;
			            cout << mysql_error(obj) << std::endl;
			            break;  // Exit the loop on query error
			        }
			
			        MYSQL_RES* resultCheck = mysql_store_result(obj);
			        if (resultCheck) {
			            MYSQL_ROW rowCheck = mysql_fetch_row(resultCheck);
			            int count;
			            stringstream(rowCheck[0]) >> count;
			            mysql_free_result(resultCheck);
			
			            if (count > 0) {
			                validDishId = true;
			            } else {
			                cout << "Invalid Dish ID. Please enter a valid ID from the current menu.\n";
			            }
			        } else {
			            cout << "No data retrieved\n";
			            break;  // Exit the loop if no data is retrieved
			        }
			    } while (!validDishId);
			
			    if (validDishId) {
			        cout << "Selected Dish ID: " << dishId << std::endl;
			        displayDishesWithIngredients(obj, dishId, currentTimeSlot);
			    }
			
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
	}while (choice != '4');
}
	mysql_close(obj);
    }
    return 0;
}
