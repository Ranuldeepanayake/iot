-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: mysql-1
-- Generation Time: Aug 27, 2022 at 08:28 PM
-- Server version: 8.0.27
-- PHP Version: 7.4.20

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `iot`
--
CREATE DATABASE IF NOT EXISTS `iot` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE `iot`;

-- --------------------------------------------------------

--
-- Table structure for table `device`
--

CREATE TABLE `device` (
  `device_id` int NOT NULL,
  `device_name` varchar(25) NOT NULL,
  `device_type` varchar(25) NOT NULL,
  `topic_ambient_light` decimal(8,2) DEFAULT '0.00',
  `topic_soil_moisture` decimal(8,2) DEFAULT '0.00',
  `topic_air_temperature` decimal(8,2) DEFAULT '0.00',
  `topic_test_led` varchar(3) DEFAULT 'on',
  `topic_led_strip` int DEFAULT '7',
  `topic_water_pump` int DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `device`
--

INSERT INTO `device` (`device_id`, `device_name`, `device_type`, `topic_ambient_light`, `topic_soil_moisture`, `topic_air_temperature`, `topic_test_led`, `topic_led_strip`, `topic_water_pump`) VALUES
(1, 'databaseService1', 'database-service', '15.83', '27.00', '31.76', 'off', 7, 5);

-- --------------------------------------------------------

--
-- Table structure for table `topic_temperature`
--

CREATE TABLE `topic_temperature` (
  `id` int NOT NULL,
  `timestamp` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `value` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `topic_temperature`
--

INSERT INTO `topic_temperature` (`id`, `timestamp`, `value`) VALUES
(1, '2022-08-06 15:47:44', 45),
(2, '2022-08-06 15:48:24', 45),
(3, '2022-08-06 15:49:45', 45),
(4, '2022-08-27 14:41:46', 33);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `device`
--
ALTER TABLE `device`
  ADD PRIMARY KEY (`device_id`);

--
-- Indexes for table `topic_temperature`
--
ALTER TABLE `topic_temperature`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `device`
--
ALTER TABLE `device`
  MODIFY `device_id` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `topic_temperature`
--
ALTER TABLE `topic_temperature`
  MODIFY `id` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
