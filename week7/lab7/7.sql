SELECT AVG(s.energy) FROM songs AS s
INNER JOIN artists ON artists.id = s.artist_id
WHERE artists.name = "Drake";