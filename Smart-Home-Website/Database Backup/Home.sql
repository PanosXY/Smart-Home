-- MySQL dump 10.13  Distrib 5.5.30, for debian-linux-gnu (armv5tel)
--
-- Host: localhost    Database: Home
-- ------------------------------------------------------
-- Server version	5.5.30-1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `LoginFlag`
--

DROP TABLE IF EXISTS `LoginFlag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `LoginFlag` (
  `Flag` enum('True','False') DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LoginFlag`
--

LOCK TABLES `LoginFlag` WRITE;
/*!40000 ALTER TABLE `LoginFlag` DISABLE KEYS */;
INSERT INTO `LoginFlag` VALUES ('False');
/*!40000 ALTER TABLE `LoginFlag` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `OutsideSensor`
--

DROP TABLE IF EXISTS `OutsideSensor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `OutsideSensor` (
  `ID` varchar(2) DEFAULT NULL,
  `Lumens` int(11) DEFAULT NULL,
  `Temp` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `OutsideSensor`
--

LOCK TABLES `OutsideSensor` WRITE;
/*!40000 ALTER TABLE `OutsideSensor` DISABLE KEYS */;
/*!40000 ALTER TABLE `OutsideSensor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Radar`
--

DROP TABLE IF EXISTS `Radar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Radar` (
  `ID` varchar(2) DEFAULT NULL,
  `AlarmReport` enum('True','False') DEFAULT NULL,
  `AlarmMove` enum('True','False') DEFAULT NULL,
  `AlarmSet` enum('True','False') DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Radar`
--

LOCK TABLES `Radar` WRITE;
/*!40000 ALTER TABLE `Radar` DISABLE KEYS */;
INSERT INTO `Radar` VALUES ('7E','False','False','False');
/*!40000 ALTER TABLE `Radar` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Switch`
--

DROP TABLE IF EXISTS `Switch`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Switch` (
  `ID` varchar(2) DEFAULT NULL,
  `Temp` float DEFAULT NULL,
  `Dimmer` int(11) DEFAULT NULL,
  `OnOff` enum('True','False') DEFAULT NULL,
  `Lumens` int(11) DEFAULT NULL,
  `DimmerSet` int(11) DEFAULT NULL,
  `OnOffSet` enum('True','False') DEFAULT NULL,
  `SetMov` enum('True','False') DEFAULT NULL,
  `SetAutoLumensOnOff` enum('True','False') DEFAULT NULL,
  `SetAutoLumens` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Switch`
--

LOCK TABLES `Switch` WRITE;
/*!40000 ALTER TABLE `Switch` DISABLE KEYS */;
INSERT INTO `Switch` VALUES ('BF',27.2,8,'False',10,8,'True','False','False',18);
/*!40000 ALTER TABLE `Switch` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Thermostat`
--

DROP TABLE IF EXISTS `Thermostat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Thermostat` (
  `ID` varchar(2) DEFAULT NULL,
  `Temp` float DEFAULT NULL,
  `OnOff` enum('True','False') DEFAULT NULL,
  `TempUser` float DEFAULT NULL,
  `OnOffSet` enum('True','False') DEFAULT NULL,
  `TempUserSet` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Thermostat`
--

LOCK TABLES `Thermostat` WRITE;
/*!40000 ALTER TABLE `Thermostat` DISABLE KEYS */;
INSERT INTO `Thermostat` VALUES ('AE',33.3,'False',25,'False',25);
/*!40000 ALTER TABLE `Thermostat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Users`
--

DROP TABLE IF EXISTS `Users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Users` (
  `Name` varchar(20) DEFAULT NULL,
  `Surname` varchar(20) DEFAULT NULL,
  `Username` varchar(20) DEFAULT NULL,
  `ID` int(11) DEFAULT NULL,
  `Password` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Users`
--

LOCK TABLES `Users` WRITE;
/*!40000 ALTER TABLE `Users` DISABLE KEYS */;
INSERT INTO `Users` VALUES ('Panos','Skiadas','PanosXY',0,'8027fec524affea438a44b0c8c21eb00'),('Guest','Guest','Guest',1,'e10adc3949ba59abbe56e057f20f883e'),('Visitor','Visitor','Visitor',2,'127870930d65c57ee65fcc47f2170d38');
/*!40000 ALTER TABLE `Users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-06-18 18:16:40
