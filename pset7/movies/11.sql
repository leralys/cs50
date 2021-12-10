-- In 11.sql, write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in,
-- starting with the highest rated.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Chadwick Boseman.
-- Executing 11.sql results in a table with 1 column and 5 rows.


SELECT movies.title from people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
JOIN ratings on movies.id = ratings.movie_id
WHERE people.name = "Chadwick Boseman"
ORDER BY rating DESC
LIMIT 5;

