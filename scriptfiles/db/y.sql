-- phpMyAdmin SQL Dump
-- version 4.9.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 08, 2020 at 10:55 AM
-- Server version: 10.4.11-MariaDB
-- PHP Version: 7.2.26

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `y`
--

-- --------------------------------------------------------

--
-- Table structure for table `accounts`
--

CREATE TABLE `accounts` (
  `id` bigint(20) UNSIGNED NOT NULL,
  `name` varchar(32) NOT NULL,
  `password` varchar(61) NOT NULL,
  `email` varchar(32) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT current_timestamp(),
  `last_login` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=tis620;

--
-- Dumping data for table `accounts`
--

INSERT INTO `accounts` (`id`, `name`, `password`, `email`, `created_at`, `last_login`) VALUES
(11, 'John_Connor', '$2y$12$WLB0pzGmfXoJzJla7YtumeB3eut4Y37o2kIVPwRKfmxtBgdG1coku', '', '2020-10-08 15:40:35', '0000-00-00 00:00:00');

-- --------------------------------------------------------

--
-- Table structure for table `characters`
--

CREATE TABLE `characters` (
  `id` bigint(20) UNSIGNED NOT NULL,
  `name` varchar(25) NOT NULL,
  `account_id` bigint(20) UNSIGNED NOT NULL,
  `active` tinyint(1) NOT NULL,
  `ip` varchar(18) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT current_timestamp(),
  `logged_at` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=tis620;

--
-- Dumping data for table `characters`
--

INSERT INTO `characters` (`id`, `name`, `account_id`, `active`, `ip`, `created_at`, `logged_at`) VALUES
(6, 'John_Connor', 11, 0, '', '2020-10-08 15:40:42', '0000-00-00');

-- --------------------------------------------------------

--
-- Table structure for table `character_statuses`
--

CREATE TABLE `character_statuses` (
  `id` bigint(20) UNSIGNED NOT NULL,
  `character_id` bigint(20) UNSIGNED NOT NULL DEFAULT 0,
  `developer` tinyint(1) UNSIGNED NOT NULL DEFAULT 0,
  `admin` tinyint(1) UNSIGNED NOT NULL DEFAULT 0,
  `cash` int(10) UNSIGNED NOT NULL DEFAULT 0,
  `vip` tinyint(3) UNSIGNED NOT NULL DEFAULT 0,
  `level` smallint(5) UNSIGNED NOT NULL DEFAULT 0,
  `skin` smallint(5) UNSIGNED NOT NULL DEFAULT 264
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `character_statuses`
--

INSERT INTO `character_statuses` (`id`, `character_id`, `developer`, `admin`, `cash`, `vip`, `level`, `skin`) VALUES
(1, 6, 0, 0, 0, 0, 0, 264);

-- --------------------------------------------------------

--
-- Table structure for table `doors`
--

CREATE TABLE `doors` (
  `id` bigint(20) UNSIGNED NOT NULL,
  `name` varchar(31) NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL,
  `eX` float NOT NULL,
  `eY` float NOT NULL,
  `eZ` float NOT NULL,
  `eA` float NOT NULL,
  `eInt` tinyint(3) UNSIGNED NOT NULL,
  `eWorld` int(10) UNSIGNED NOT NULL,
  `eLabel` varchar(31) NOT NULL,
  `eGtext` varchar(31) NOT NULL,
  `ePickupModel` int(10) NOT NULL,
  `iX` float NOT NULL,
  `iY` float NOT NULL,
  `iZ` float NOT NULL,
  `iA` float NOT NULL,
  `iInt` tinyint(3) UNSIGNED NOT NULL,
  `iWorld` int(10) UNSIGNED NOT NULL,
  `iLabel` varchar(31) NOT NULL,
  `iGtext` varchar(31) NOT NULL,
  `iPickupModel` int(10) NOT NULL,
  `faction_id` bigint(20) UNSIGNED DEFAULT NULL
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
  MODIFY `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT for table `characters`
--
ALTER TABLE `characters`
  MODIFY `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `character_statuses`
--
ALTER TABLE `character_statuses`
  MODIFY `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

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
