-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Dec 15, 2020 at 04:43 PM
-- Server version: 8.0.21
-- PHP Version: 7.4.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `y`
--
CREATE DATABASE IF NOT EXISTS `y` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE `y`;

-- --------------------------------------------------------

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `id` bigint UNSIGNED NOT NULL,
  `name` varchar(32) NOT NULL,
  `password` varchar(61) NOT NULL,
  `email` varchar(32) CHARACTER SET tis620 COLLATE tis620_thai_ci DEFAULT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_login` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=tis620;

-- --------------------------------------------------------

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `id` bigint UNSIGNED NOT NULL,
  `name` varchar(25) NOT NULL,
  `account_id` bigint UNSIGNED NOT NULL,
  `active` tinyint(1) NOT NULL DEFAULT '0',
  `ip` varchar(18) CHARACTER SET tis620 COLLATE tis620_thai_ci DEFAULT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `logged_at` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=tis620;

-- --------------------------------------------------------

--
-- Table structure for table `character_statuses`
--

DROP TABLE IF EXISTS `character_statuses`;
CREATE TABLE `character_statuses` (
  `id` bigint UNSIGNED NOT NULL,
  `character_id` bigint UNSIGNED NOT NULL DEFAULT '0',
  `developer` tinyint UNSIGNED NOT NULL DEFAULT '0',
  `admin` tinyint UNSIGNED NOT NULL DEFAULT '0',
  `cash` int UNSIGNED NOT NULL DEFAULT '0',
  `vip` tinyint UNSIGNED NOT NULL DEFAULT '0',
  `level` smallint UNSIGNED NOT NULL DEFAULT '0',
  `skin` smallint UNSIGNED NOT NULL DEFAULT '264'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `doors`
--

DROP TABLE IF EXISTS `doors`;
CREATE TABLE `doors` (
  `id` bigint UNSIGNED NOT NULL,
  `name` varchar(31) NOT NULL,
  `type` tinyint UNSIGNED NOT NULL,
  `eX` float NOT NULL,
  `eY` float NOT NULL,
  `eZ` float NOT NULL,
  `eA` float NOT NULL,
  `eInt` tinyint UNSIGNED NOT NULL,
  `eWorld` int UNSIGNED NOT NULL,
  `eLabel` varchar(31) NOT NULL,
  `eGtext` varchar(31) NOT NULL,
  `ePickupModel` int NOT NULL,
  `iX` float NOT NULL,
  `iY` float NOT NULL,
  `iZ` float NOT NULL,
  `iA` float NOT NULL,
  `iInt` tinyint UNSIGNED NOT NULL,
  `iWorld` int UNSIGNED NOT NULL,
  `iLabel` varchar(31) NOT NULL,
  `iGtext` varchar(31) NOT NULL,
  `iPickupModel` int NOT NULL,
  `faction_id` bigint UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=tis620;

--
-- Dumping data for table `doors`
--

INSERT INTO `doors` (`id`, `name`, `type`, `eX`, `eY`, `eZ`, `eA`, `eInt`, `eWorld`, `eLabel`, `eGtext`, `ePickupModel`, `iX`, `iY`, `iZ`, `iA`, `iInt`, `iWorld`, `iLabel`, `iGtext`, `iPickupModel`, `faction_id`) VALUES
(1, 'Los Santos Police Department', 0, 1554.33, -1675.63, 16.1953, 271.451, 0, 0, '', '', 1314, 246.858, 63.8856, 1003.64, 357.952, 6, 2001, '', 'Los Santos Police Department', 0, 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `characters`
--
ALTER TABLE `characters`
  ADD PRIMARY KEY (`id`),
  ADD KEY `account_id` (`account_id`);

--
-- Indexes for table `character_statuses`
--
ALTER TABLE `character_statuses`
  ADD PRIMARY KEY (`id`),
  ADD KEY `player_id` (`character_id`);

--
-- Indexes for table `doors`
--
ALTER TABLE `doors`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `accounts`
--
ALTER TABLE `accounts`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `characters`
--
ALTER TABLE `characters`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `character_statuses`
--
ALTER TABLE `character_statuses`
  MODIFY `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `characters`
--
ALTER TABLE `characters`
  ADD CONSTRAINT `FK_account_id` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`);

--
-- Constraints for table `character_statuses`
--
ALTER TABLE `character_statuses`
  ADD CONSTRAINT `character_id` FOREIGN KEY (`character_id`) REFERENCES `characters` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
