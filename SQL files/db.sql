CREATE TABLE Category (
    id INT,
    name VARCHAR(255),
    PRIMARY KEY (id)
);

CREATE TABLE Origin (
    id INT,
    name VARCHAR(255),
    PRIMARY KEY (id)
);

CREATE TABLE Ingredient (
    id INT,
    name VARCHAR(255),
    quantity DECIMAL,
    unit VARCHAR(255),
    import_date DATE,
    expired_date DATE,
    PRIMARY KEY (id)
);

CREATE TABLE Dish (
    id INT,
    name VARCHAR(255),
    category_id INT,
    price DECIMAL,
    time_slot INT,
    FOREIGN KEY (category_id) REFERENCES Category(id),
    PRIMARY KEY (id)
);

CREATE TABLE Dish_Origin (
    dish_id INT,
    origin_id INT,
    FOREIGN KEY (dish_id) REFERENCES Dish(id),
    FOREIGN KEY (origin_id) REFERENCES Origin(id),
    PRIMARY KEY (dish_id, origin_id)
);

CREATE TABLE Dish_Ingredient (
    dish_id INT,
    ingredient_id INT,
    FOREIGN KEY (dish_id) REFERENCES Dish(id),
    FOREIGN KEY (ingredient_id) REFERENCES Ingredient(id),
    PRIMARY KEY (dish_id, ingredient_id)
);