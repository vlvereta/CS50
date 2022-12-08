-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Get the description of the situation to know what happenned
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2021 AND street = "Humphrey Street";
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
--
-- Get the transcripts from witnesses' interviews
SELECT transcript from interviews WHERE transcript LIKE "%bakery%" AND day = 28 AND month = 7 AND year = 2021;
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
--
-- I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
--
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
-- I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.
--
-- Get the origin airport (id: 8)
SELECT * FROM airports WHERE city = "Fiftyville";
-- Get the earliest flight from Fiftyville next day (destination airport id: 4, flight id: 36)
SELECT * FROM flights WHERE origin_airport_id = 8 AND day = 29 AND month = 7 AND year = 2021 ORDER BY hour, minute LIMIT 1;
-- Get the destination airport (city: New York City)
SELECT * FROM airports WHERE id = 4;
-- Get passengers on that flight
SELECT * FROM passengers WHERE flight_id = 36;
--
-- Get people who withdraw money
SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- Get info from bakery_security_logs within 10 minutes of the crime
SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND minute >= 15 AND minute <= 25;
-- Got several people who withdraw money and exit the parking lot
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       | 396669    | 2014          |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | 467400    | 2014          |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- People from the table above and who takes the searched flight:
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | 467400    | 2014          |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- Check the phone calls
SELECT * FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration < 60;
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- The thief found!
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- The accomplice found!
SELECT name FROM people WHERE phone_number = "(375) 555-8161";
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+
