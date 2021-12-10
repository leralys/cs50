-- In 8.sql, write a SQL query to list the names of all people who starred in Toy Story.
-- Your query should output a table with a single column for the name of each person.
-- You may assume that there is only one movie in the database with the title Toy Story.
-- Executing 8.sql results in a table with 1 column and 4 rows.

-- CREATE INDEX person_index ON stars(person_id);
-- CREATE INDEX movie_index ON stars(movie_id);
-- CREATE INDEX title_index ON movies(title);
SELECT name FROM people JOIN stars ON people.id = stars.person_id JOIN movies on stars.movie_id = movies.id WHERE title = "Toy Story";