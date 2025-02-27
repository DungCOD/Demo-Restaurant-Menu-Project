-- Insert data into Category table
INSERT INTO Category (id, name) VALUES
(1, 'Appetizer'),
(2, 'Main Course'),
(3, 'Dessert');

-- Insert data into Origin table
INSERT INTO Origin (id, name) VALUES
(1, 'North'),
(2, 'South'),
(3, 'Middle');

-- Insert data into Ingredient table
INSERT INTO Ingredient (id, name, quantity, unit, import_date, expired_date) VALUES
(1, 'Rice', 500, 'g', '2023-01-01', '2023-02-01'),
(2, 'Chicken', 1000, 'g', '2023-01-05', '2023-02-05'),
(3, 'Tofu', 200, 'g', '2023-01-10', '2023-03-10'),
(4, 'Lettuce', 300, 'g', '2023-01-03', '2023-01-20'),
(5, 'Beef', 800, 'g', '2023-01-08', '2023-02-08'),
(6, 'Pork', 400, 'g', '2023-01-12', '2023-01-28'),
(7, 'Pasta', 600, 'g', '2023-01-15', '2023-02-15'),
(8, 'Salmon', 500, 'g', '2023-01-18', '2023-02-18'),
(9, 'Sugar', 250, 'g', '2023-01-22', '2023-02-22'),
(10, 'Flour', 300, 'g', '2023-01-25', '2023-02-25'),
(11, 'Milk', 300, 'g', '2023-01-25', '2023-02-25'),
(12, 'Bun', 300, 'g', '2023-02-05', '2023-02-20'),
(13, 'Shrimp', 400, 'g', '2023-02-10', '2023-03-10'),
(14, 'Fish sauce', 150, 'ml', '2023-02-15', '2023-03-15'),
(15, 'Vanilla Extract', 50, 'ml', '2023-02-20', '2023-03-20');

-- Insert data into Dish table
INSERT INTO Dish (id, name, category_id, price, time_slot) VALUES
(1, 'Bun dau mam tom', 2, 50000, 0),
(2, 'Bun cha', 2, 40000, 0),
(3, 'Kem oc que', 3, 10000, 1),
(4, 'Salad', 1, 50000, 1),
(5, 'Banh bao', 2, 10000, 0),
(6, 'Lau ga', 2, 300000, 1),
(7, 'Thit nuong', 2, 300000, 1),
(8, 'Banh xeo', 2, 100000, 1),
(9, 'Bun bo Hue', 2, 100000, 1),
(10, 'Com tam', 2, 100000, 1);

-- Insert data into Dish_Origin table
INSERT INTO Dish_Origin (dish_id, origin_id) VALUES
(1, 1),
(2, 1),
(3, 1),
(3, 2),
(3, 3),
(4, 1),
(4, 2),
(4, 3),
(5, 1),
(6, 3),
(7, 1),
(7, 2),
(7, 3),
(8, 3),
(9, 3),
(10, 2);

-- Insert data into Dish_Ingredient table
INSERT INTO Dish_Ingredient (dish_id, ingredient_id) VALUES
(1, 3),
(1, 6),
(1, 12),
(2, 6),
(2, 12),
(2, 14),
(3, 9),
(3, 11),
(3, 15),
(4, 4),
(4, 8),
(5, 6),
(5, 10),
(6, 2),
(6, 3),
(6, 4),
(7, 2),
(7, 5),
(7, 6),
(7, 8),
(8, 6),
(8, 10),
(8, 13),
(9, 6),
(9, 12),
(9, 13),
(10, 1),
(10, 6),
(10, 14);