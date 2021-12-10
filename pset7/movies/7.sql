-- In 7.sql, write a SQL query to list all movies released in 2010 and their ratings,
-- in descending order by rating.
-- For movies with the same rating, order them alphabetically by title.
-- Executing 7.sql results in a table with 2 columns and 6,835 rows.

SELECT rating, title FROM ratings JOIN movies ON ratings.movie_id = movies.id WHERE year = 2010 ORDER BY -rating, title;