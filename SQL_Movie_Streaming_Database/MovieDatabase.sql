SET FOREIGN_KEY_CHECKS = 1; -- enables foreign key contraints

CREATE TABLE Streaming_Site (
  Site_name varchar(80) PRIMARY KEY NOT NULL
);

CREATE TABLE Streaming_Plan (
  name varchar(80) NOT NULL,
  Site_name varchar(80) NOT NULL,
  price FLOAT NOT NULL,
  PRIMARY KEY (name, Site_name),
  FOREIGN KEY (Site_name) REFERENCES Streaming_Site(Site_name) ON DELETE restrict ON UPDATE cascade
);

CREATE TABle Web_Site (
  Web_address varchar(80) PRIMARY KEY NOT NULL,
  Site_name varchar(80) NOT NULL,
  FOREIGN KEY (Site_name) REFERENCES Streaming_Site(Site_name) ON DELETE restrict ON UPDATE cascade
);

Create table Movie (
  m_key int PRIMARY key not NULL auto_increment,
  title varchar(80) NOT NULL,
  length int not NULL CHECK(length > 0),
  genre varchar(80) NOT NULL,
  day INT CHECK(day > 0 and day < 32),
  month INT Check(month > 0 AND month < 12),
  year INT not NULL CHECK(year > 1850)
);

CREATE TABLE sShow (
  s_key int PRIMARY KEY NOT NULL auto_increment,
  genre varchar(80) not NULL,
  title varchar(80) not NULL,
  num_seasons int NOT NULL CHECK(num_seasons > 0)
);
  
CREATE TABLE Season (
  S_num int NOT NULL,
  show_key int NOT NULL,
  day INT CHECK(day > 0 and day < 32),
  month INT Check(month > 0 AND month < 12),
  year INT NOT NULL CHECK(year > 1850),
  PRIMARY KEY (S_num, show_key),
  FOREIGN KEY (show_key) REFERENCES sShow(s_key) ON DELETE RESTRICT
);

Create table Person (
  p_key int PRimary key not NULL auto_increment,
  name varchar(80) not NULL
);

CREATE Table Mwork (
  person_key int Not NULL,
  movie_key int not NULL,
  role varchar(80) NOT NULL,
  PRIMARY key (person_key, movie_key, role),
  Foreign KEY (person_key) REFERENCES Person(p_key),
  FOREIGN key (movie_key) REFERENCES Movie(m_key) ON DELETE cascade
);

CREATE Table Swork (
  person_key INT NOT NULL,
  show_key int not NULL,
  season_num int NOT NULL,
  role varchar(80) NOT NULL,
  PRIMARY Key (person_key, show_key, season_num, role),
  FOREIGN key (person_key) REFERENCES Person(p_key),
  FOREIGN KEY (show_key, season_num) REFERENCES season(show_key, S_num) ON DELETE cascade
);

Create table SOM (
  movie_key int not NULL,
  Site_name varchar(80) not NULL,
  Incl_plan varchar(80) not NULL,
  PRIMARY key(movie_key, Site_name, Incl_plan),
  FOREIGN KEY (movie_key) REFERENCES Movie(m_key) ON DELETE cascade,
  Foreign key (Site_name) REFERENCES Streaming_Plan(Site_name) ON DELETE cascade,
  Foreign key (Incl_plan) references Streaming_Plan(name) ON DELETE cascade
);

CREATE TABLE SOS (
  show_key int not NULL,
  season_num int NOT NULL,
  Site_name varchar(80) not NULL,
  Incl_plan varchar(80) not NULL,
  PRIMARY key(show_key, season_num, Site_name, Incl_plan),
  FOREIGN KEY (show_key, season_num) REFERENCES season(show_key, S_num) ON DELETE cascade,
  Foreign key (Site_name) REFERENCES Streaming_Plan(Site_name) ON DELETE cascade,
  Foreign key (Incl_plan) references Streaming_Plan(name) ON DELETE cascade
);

-- adding all procedure giving easier utility
DELIMITER //
CREATE PROCEDURE searchShowMovie(IN mediaType varchar(10), IN mediaTitle varchar(80))
BEGIN
    IF (mediaType = 'movie') THEN
    SELECT m_key, title, day, month, year FROM movie WHERE title LIKE concat('%', mediaTitle, '%');
    ELSE
    SELECT s_key, title FROM sshow WHERE title LIKE concat('%', mediaTitle, '%');
    END IF;
END //
    
CREATE PROCEDURE searchStreamingSite(IN site varchar(20))
BEGIN
	SELECT title AS 'Movie', day, month, year FROM movie WHERE m_key IN (SELECT movie_key FROM SOM WHERE Site_name = site);

	SELECT title AS 'sSHow' FROM sshow WHERE s_key IN (SELECT show_key FROM season WHERE show_key IN (SELECT show_key FROM SOS WHERE Site_name = site));
END //    

CREATE PROCEDURE totalShowsMovies()
BEGIN
	SELECT (
    SELECT count(m_key) FROM movie ) AS '#ofMovies', 
    (
    SELECT count(s_key) FROM sshow ) AS '#ofShows';

END //

CREATE PROCEDURE selectMovie(IN movieID INT)
BEGIN
	SELECT title, length, genre, day, month, year FROM movie WHERE m_key = movieID;
    
    SELECT movie.title, som.Site_name AS 'Streamed on', som.Incl_plan AS 'Stream Plan', streaming_plan.price, web_site.Web_address
    FROM (((movie INNER JOIN som ON som.movie_key = movie.m_key) INNER JOIN streaming_plan ON (streaming_plan.name = som.Incl_plan AND streaming_plan.Site_name = som.Site_name)) INNER JOIN web_site ON web_site.Site_name = som.Site_name)
    WHERE movie.m_key = movieID;

END//

CREATE PROCEDURE selectShow(IN showID INT)
BEGIN
	SELECT title, genre, num_seasons FROM sshow WHERE s_key = showID;
    
    SELECT sshow.title, season.S_num AS 'Season #', season.day, season.month, season.year, sos.Site_name AS 'Streamed on', sos.Incl_plan AS 'Stream Plan', streaming_plan.price, web_site.Web_address
	FROM ((((sshow INNER JOIN season ON sshow.s_key = season.show_key) INNER JOIN sos ON (sos.season_num = season.S_num AND sos.show_key = season.show_key)) INNER JOIN streaming_plan ON (streaming_plan.name = sos.Incl_plan AND streaming_plan.Site_name = sos.Site_name)) INNER JOIN web_site ON web_site.Site_name = sos.Site_name)
	WHERE s_key = showID;

END//

CREATE PROCEDURE workedOnMovie(IN movieID INT)
BEGIN
	SELECT movie.title, person.name, mwork.role, person.p_key
    FROM ((movie INNER JOIN mwork ON movie.m_key = mwork.movie_key) INNER JOIN person ON person.p_key = mwork.person_key)
    WHERE movie.m_key = movieID;
END//

CREATE PROCEDURE workedOnShow(IN showID INT)
BEGIN
	SELECT sshow.title, season.S_num, person.name, swork.role, person.p_key
    FROM ((( sshow INNER JOIN season ON sshow.s_key = season.show_key) INNER JOIN swork ON (swork.show_key = season.show_key AND swork.season_num = season.S_num)) INNER JOIN person ON person.p_key = swork.person_key)
    WHERE sshow.s_key = showID;
END//


-- insert procedures

CREATE PROCEDURE insertMovie(IN iTitle varchar(80), IN iLength int, IN iGenre varchar(80), IN iDay int, IN iMonth int, IN iYear int)
BEGIN

	INSERT INTO movie (title, length, genre, day, month, year) VALUES (iTitle, iLength, iGenre, iDay, iMonth, iYear);
    
END//

CREATE PROCEDURE insertShow(IN iTitle varchar(80), IN iGenre varchar(80), IN iNumSeasons INT)
BEGIN
	INSERT INTO sshow (title, genre, num_seasons) VALUES (iTitle, iGenre, iNumSeasons);
END//

CREATE PROCEDURE insertPerson(IN iName varchar(80))
BEGIN
	INSERT INTO person (name) VALUES (iName);
END//

CREATE PROCEDURE insertMovieWork(IN iPersonKey INT, IN iMovieKey INT, IN iRole varchar(80))
BEGIN
	INSERT INTO mwork (person_key, movie_key, role) VALUES (iPersonKey, iMovieKey, iRole);
END//

CREATE PROCEDURE insertSeason (IN iSnum INT, IN iShowKey INT, IN iDay int, IN iMonth int, IN iYear int)
BEGIN
	INSERT INTO season(S_num, show_key, day, month, year) VALUES (iSnum, iShowKey, iDay, iMonth, iYear);
END//

CREATE PROCEDURE insertStreamingOnMovie (IN iMovieKey int, IN iSiteName varchar(80), IN iInclPlan varchar(80) )
BEGIN
	INSERT INTO som(movie_key, Site_name, Incl_plan) VALUES (iMovieKey, iSiteName, iInclPlan);
END//

CREATE PROCEDURE insertStreamingOnSeason (IN iShowKey int, IN iSeasonNum int ,IN iSiteName varchar(80), IN iInclPlan varchar(80))
BEGIN
	INSERT INTO sos(show_key, season_num, Site_name, Incl_plan) VALUES (iShowKey, iSeasonNum, iSiteName, iInclPlan) ;
END//

CREATE PROCEDURE insertStreamingPlan (IN iName varchar(80), IN iSiteName varchar(80), IN iPrice float)
BEGIN
	INSERT INTO streaming_plan(name, Site_name, price ) VALUES (iName, iSiteName, iPrice );
END//

CREATE PROCEDURE insertStreamingSite (IN iSiteName varchar(80) )
BEGIN
	INSERT INTO streaming_site(Site_name) VALUES (iSiteName);
END//

CREATE PROCEDURE insertShowWork (IN iPersonKey int, IN iShowKey int, IN iSeasonNum int, IN iRole varchar(80) )
BEGIN
	INSERT INTO swork(person_key, show_key, season_num, role) VALUES(iPersonKey, iShowKey, iSeasonNum, iRole) ;
END//

CREATE PROCEDURE insertWebSite (IN iWebAddress varchar(80), IN iSiteName varchar(80) )
BEGIN
	INSERT INTO web_site(Web_address, Site_name) VALUES (iWebAddress, iSiteName);
END//
    

-- update procedures

CREATE PROCEDURE updateMovie (IN u_m_key INT, IN uTitle varchar(80), IN uLength int, IN uGenre varchar(80), IN uDay int, IN uMonth int, IN uYear int)
BEGIN
	UPDATE movie
    SET title = uTitle, length = uLength, genre = uGenre, day = uDay, month = uMonth, year = uYear
    WHERE u_m_key = m_key;
END//

CREATE PROCEDURE updatePerson (IN uPKey int, IN uName varchar(80) )
BEGIN
	UPDATE person
    SET name = uName
    WHERE uPKey = p_key ;
END//

CREATE PROCEDURE updateSeason (IN uSnum INT, IN uShowKey INT, IN uDay int, IN uMonth int, IN uYear int )
BEGIN
	UPDATE season
    SET day = uDay, month = uMonth, year = uYear
    WHERE (uSnum = S_num AND uShowKey = show_key) ;
END//

CREATE PROCEDURE updateShow (IN uSKey INT ,IN uTitle varchar(80), IN uGenre varchar(80), IN uNumSeasons INT )
BEGIN
	UPDATE sshow
    SET title = uTitle, genre = uGenre, num_seasons = uNumSeasons
    WHERE uSkey = s_key ;
END//

CREATE PROCEDURE updateStreamingPlan (IN uName varchar(80), IN uSiteName varchar(80), IN uPrice float )
BEGIN
	UPDATE streaming_plan
    SET price = uPrice
    WHERE (uName = name AND uSiteName = Site_name) ;
END//


CREATE PROCEDURE updateWebSite (IN uWebAddress varchar(80), IN uSiteName varchar(80))
BEGIN
	UPDATE web_site
    SET Site_name = uSiteName
    WHERE Web_address = uWebAddress ;
END//


-- Delete procedures

CREATE PROCEDURE deleteMovie (IN d_m_key INT)
BEGIN
	DELETE FROM movie WHERE m_key = d_m_key;
END//

CREATE PROCEDURE deleteShow (IN d_s_key INT)
BEGIN
	DELETE FROM sshow WHERE s_key = d_s_key;
END//

CREATE PROCEDURE deleteSeason (IN d_s_key INT, IN d_season_num INT)
BEGIN
	DELETE FROM season WHERE (show_key = d_s_key AND S_num = d_season_num);
END//

CREATE PROCEDURE deletePerson (IN d_p_key INT)
BEGIN
	DELETE FROM person WHERE p_key = d_p_key;
END//

CREATE PROCEDURE deleteMovieWork(IN d_m_key INT, IN d_p_key INT, IN d_role varchar(80))
BEGIN
	DELETE FROM mwork WHERE (d_m_key = movie_key AND d_p_key = person_key AND d_role = role);
END//

CREATE PROCEDURE deleteShowWork(IN d_s_key INT, IN d_seasonNum INT, IN d_p_key INT, IN d_role varchar(80))
BEGIN
	DELETE FROM swork WHERE (d_s_key = show_key AND d_seasonNum = season_num AND d_p_key = person_key AND d_role = role);
END//

CREATE PROCEDURE deleteStreamingSite(IN d_siteName varchar(80))
BEGIN
	DELETE FROM streaming_site WHERE d_siteName = Site_name;
END//

CREATE PROCEDURE deleteWebSite(IN d_webAddress varchar(80))
BEGIN
	DELETE FROM web_site WHERE Web_address = d_webAddress;
END//

CREATE PROCEDURE deleteStreamingPlan(IN d_name varchar(80), IN d_siteName varchar(80))
BEGIN
	DELETE FROM streaming_plan WHERE (d_name = name AND d_siteName = Site_name);
END//

CREATE PROCEDURE deleteStreamingOnMovie(IN d_m_key INT, IN d_siteName varchar(80), IN d_plan varchar(80))
BEGIN
	DELETE FROM som WHERE (d_m_key = movie_key AND d_siteName = Site_name AND d_plan = Incl_plan);
END//

CREATE PROCEDURE deleteStreamingOnSeason(IN d_show_key INT, IN d_seasonNum INT, IN d_siteName varchar(80), IN d_plan varchar(80))
BEGIN
	DELETE FROM sos WHERE (d_show_key = show_key AND d_seasonNum = season_num AND d_siteName = Site_name AND d_plan = Incl_plan);
END//

DELIMITER ;

-- ADD data
INSERT INTO streaming_site VALUES ('NETFLIX');
INSERT INTO streaming_site VALUES ('HULU');
INSERT INTO streaming_site VALUES ('HBO MAX');

INSERT INTO web_site (Web_address, Site_name) VALUES ('hulu.com', 'HULU');
INSERT INTO web_site (Web_address, Site_name) VALUES ('netflix.com', 'NETFLIX');
INSERT INTO web_site (Web_address, Site_name) VALUES ('hbomax.com', 'HBO MAX');

INSERT INTO streaming_plan (name, Site_name, price) VALUES ('Hulu', 'HULU', 6.99);
INSERT INTO streaming_plan (name, Site_name, price) VALUES ('Hulu with Live TV', 'HULU', 64.99);
INSERT INTO streaming_plan (name, Site_name, price) VALUES ('Basic', 'NETFLIX', 8.99);
INSERT INTO streaming_plan (name, Site_name, price) VALUES ('With Ads', 'HBO MAX', 9.99);

INSERT INTO sshow (genre, title, num_seasons)  VALUES ('Science fiction', 'Stranger Things', 3);
INSERT INTO sshow (genre, title, num_seasons)  VALUES ('Science fiction', 'Solar Opposites', 2);
INSERT INTO sshow (genre, title, num_seasons)  VALUES ('Comedy', 'Barry', 2);

INSERT INTO  season (S_num, show_key, year) VALUES (1, (SELECT s_key FROM sshow WHERE title = 'Barry'), 2018);
INSERT INTO  season (S_num, show_key, year) VALUES (2, (SELECT s_key FROM sshow WHERE title = 'Barry'), 2019);  
  
INSERT INTO  season (S_num, show_key, year) VALUES (1, (SELECT s_key FROM sshow WHERE title = 'Solar Opposites'), 2020);  
INSERT INTO  season (S_num, show_key, year) VALUES (2, (SELECT s_key FROM sshow WHERE title = 'Solar Opposites'), 2021);

INSERT INTO  season (S_num, show_key, year) VALUES (1, (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 2016);
INSERT INTO  season (S_num, show_key, year) VALUES (2, (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 2017);
INSERT INTO  season (S_num, show_key, year) VALUES (3, (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 2019);    
  
INSERT INTO movie (title, length, genre, year) VALUES ('The Matrix', 136, 'Science fiction', 1999 );
INSERT INTO movie (title, length, genre, year) VALUES ('No Country for Old Men', 122, 'Mystery & Thriller', 2007 );
INSERT INTO movie (title, length, genre, year) VALUES ('The Shawshank Redemption', 142, 'Drama', 1994 );
INSERT INTO movie (title, length, genre, year) VALUES ('Step Brothers', 98, 'Comedy', 2008 );
INSERT INTO movie (title, length, genre, year) VALUES ('The Conjuring', 112, 'Horror', 2013 );  
  
INSERT INTO person (name) VALUES ('James Wan');
INSERT INTO person (name) VALUES ('Adam McKay');  
INSERT INTO person (name) VALUES ('Lana Wachowski');
INSERT INTO person (name) VALUES ('Lilly Wachowski');  
INSERT INTO person (name) VALUES ('Frank Darabont');  
INSERT INTO person (name) VALUES ('Joel Coen');  
INSERT INTO person (name) VALUES ('Ethan Coen');
INSERT INTO person (name) VALUES ('Justin Roiland');
INSERT INTO person (name) VALUES ('Winona Ryder');
INSERT INTO person (name) VALUES ('Bill Hader');  

INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'James Wan'), (SELECT m_key FROM movie WHERE title = 'The Conjuring'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Adam McKay'), (SELECT m_key FROM movie WHERE title = 'Step Brothers'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Lana Wachowski'), (SELECT m_key FROM movie WHERE title = 'The Matrix'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Lilly Wachowski'), (SELECT m_key FROM movie WHERE title = 'The Matrix'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Frank Darabont'), (SELECT m_key FROM movie WHERE title = 'The Shawshank Redemption'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Joel Coen'), (SELECT m_key FROM movie WHERE title = 'No Country for Old Men'), 'Director');
INSERT INTO mwork (person_key, movie_key, role) VALUES ((SELECT p_key FROM person WHERE name = 'Ethan Coen'), (SELECT m_key FROM movie WHERE title = 'No Country for Old Men'), 'Director');

INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Justin Roiland'), (SELECT s_key FROM sshow WHERE title = 'Solar Opposites'), 1, "Voice Actor");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Justin Roiland'), (SELECT s_key FROM sshow WHERE title = 'Solar Opposites'), 2, "Voice Actor");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Winona Ryder'), (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 1, "Actress");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Winona Ryder'), (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 2, "Actress");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Winona Ryder'), (SELECT s_key FROM sshow WHERE title = 'Stranger Things'), 3, "Actress");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Bill Hader'), (SELECT s_key FROM sshow WHERE title = 'Barry'), 1, "Star");
INSERT INTO swork (person_key, show_key, season_num, role) VALUES ((SELECT p_key FROM person WHERE name = 'Bill Hader'), (SELECT s_key FROM sshow WHERE title = 'Barry'), 2, "Star");

CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'The Matrix'), 'HULU', 'Hulu');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'The Matrix'), 'HBO MAX', 'With Ads');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'The Shawshank Redemption'), 'HBO MAX', 'With Ads');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'The Conjuring'), 'HBO MAX', 'With Ads');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'The Conjuring'), 'NETFLIX', 'Basic');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'No Country for Old Men'), 'HBO MAX', 'With Ads');
CALL insertStreamingOnMovie((Select m_key FROM movie WHERE title = 'Step Brothers'), 'NETFLIX', 'Basic');

CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Stranger Things'), 1, 'NETFLIX', 'Basic');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Stranger Things'), 2, 'NETFLIX', 'Basic');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Stranger Things'), 3, 'NETFLIX', 'Basic');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Barry'), 1, 'HBO MAX', 'With Ads');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Barry'), 2, 'HBO MAX', 'With Ads');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Solar Opposites'), 1, 'HULU', 'Hulu');
CALL insertStreamingOnSeason((Select s_key FROM sshow WHERE title = 'Solar Opposites'), 2, 'HULU', 'Hulu');

