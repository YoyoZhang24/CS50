SELECT s.name FROM songs AS s
INNER JOIN artists ON artists.id = s.artist_id
WHERE artists.name = "Post Malone";