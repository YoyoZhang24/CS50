-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1: Look at the crime scene description based on the given time and location.
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND street = "Chamberlin Street";
-- Output:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time —
-- each of their interview transcripts mentions the courthouse.

-- 2: Look at the interviews as were mentioned in the crime scene report.
SELECT name, transcript FROM interviews
WHERE month = 7 AND day = 28 AND year = 2020;
-- Ruth | Sometime within ten minutes of the theft,
-- I saw the thief get into a car in the courthouse parking lot and drive away.
-- If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at the courthouse,
-- I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse,
-- they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- 3: Based on Ruth, check security footage.
SELECT minute, people.name FROM courthouse_security_logs
JOIN people ON courthouse_security_logs.license_plate = people.license_plate
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;
-- 16 | Patrick
-- 18 | Ernest
-- 18 | Amber
-- 19 | Danielle
-- 20 | Roger
-- 21 | Elizabeth
-- 23 | Russell
-- 23 | Evelyn

-- 4: Based on Raymond, check the airport flight.
SELECT flights.hour, flights.id FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE airports.city = 'Fiftyville' AND flights.year = 2020
AND flights.month = 7 AND flights.day = 29;
-- Earliest: 8 | 36

-- 5: Find passangers & destination of the earliest flight
SELECT people.name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id = 36
-- Names: Doris, Roger, Ernest, Edward, Evelyn, Madison, Bobby, Danielle
-- Suspects: Ernest, Danielle, Roger, Evelyn

SELECT airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE flights.id = 36;
-- City: London

-- 6: According to Raymond, check phonecalls
SELECT people.name,duration FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60;

SELECT people.name,duration FROM phone_calls
JOIN people ON phone_calls.receiver = people.phone_number
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60;
-- Pairs: Roger & Jack, Evelyn & Larry, Ernest & Berthold, Evelyn & Melissa, Madison & James, Russell & Philip, Kimberly & Jacqueline, Bobby & Doris, Victoria & Anna
-- Suspects: Roger, Evelyn, Ernest

-- 7: Based on Eugene, check ATM on Fifer Street
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.year = 2020 AND atm_transactions.month = 7 AND
atm_transactions.day = 28 AND atm_transactions.atm_location = 'Fifer Street'
AND atm_transactions.transaction_type = 'withdraw';
-- Names: Ernest, Russell, Roy, Bobby, Elizabeth, Danielle, Madison, Victoria
-- Thief: Ernest