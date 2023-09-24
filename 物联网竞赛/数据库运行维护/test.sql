/*
 Navicat Premium Data Transfer

 Source Server         : localhost
 Source Server Type    : MySQL
 Source Server Version : 50734
 Source Host           : localhost:3306
 Source Schema         : test

 Target Server Type    : MySQL
 Target Server Version : 50734
 File Encoding         : 65001

 Date: 04/11/2021 16:31:01
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for courses
-- ----------------------------
DROP TABLE IF EXISTS `courses`;
CREATE TABLE `courses`  (
  `CNO` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '课程号',
  `CNAME` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '课程名',
  `CREDIT` int(255) NULL DEFAULT NULL COMMENT '学分',
  PRIMARY KEY (`CNO`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of courses
-- ----------------------------
INSERT INTO `courses` VALUES ('C1', '数据库', 3);
INSERT INTO `courses` VALUES ('C2', '数据结构', 3);
INSERT INTO `courses` VALUES ('C3', '操作系统', 4);
INSERT INTO `courses` VALUES ('C4', '物联网技术应用', 5);

-- ----------------------------
-- Table structure for enrolls
-- ----------------------------
DROP TABLE IF EXISTS `enrolls`;
CREATE TABLE `enrolls`  (
  `SNO` decimal(6, 0) NOT NULL,
  `CNO` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `GRADE` decimal(4, 0) NULL DEFAULT NULL,
  PRIMARY KEY (`SNO`, `CNO`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of enrolls
-- ----------------------------
INSERT INTO `enrolls` VALUES (990027, 'C1', 90);
INSERT INTO `enrolls` VALUES (990027, 'C3', 95);
INSERT INTO `enrolls` VALUES (990027, 'C4', 88);
INSERT INTO `enrolls` VALUES (990652, 'C1', 88);
INSERT INTO `enrolls` VALUES (990652, 'C4', 83);
INSERT INTO `enrolls` VALUES (990668, 'C3', 84);
INSERT INTO `enrolls` VALUES (990674, 'C2', 77);
INSERT INTO `enrolls` VALUES (990676, 'C3', 90);
INSERT INTO `enrolls` VALUES (990684, 'C1', 82);
INSERT INTO `enrolls` VALUES (990684, 'C3', 85);
INSERT INTO `enrolls` VALUES (991091, 'C2', 93);

-- ----------------------------
-- Table structure for students
-- ----------------------------
DROP TABLE IF EXISTS `students`;
CREATE TABLE `students`  (
  `SNO` decimal(6, 0) NOT NULL COMMENT '学号',
  `SNAME` char(8) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '姓名',
  `AGE` decimal(3, 0) NULL DEFAULT NULL COMMENT '年龄',
  `SEX` char(2) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '性别',
  `BPLACE` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '籍贯',
  PRIMARY KEY (`SNO`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of students
-- ----------------------------
INSERT INTO `students` VALUES (990027, '胡伟光', 22, '男', '湖南');
INSERT INTO `students` VALUES (990652, '张春海', 24, '男', '河北');
INSERT INTO `students` VALUES (990668, '王丽', 22, '女', '四川');
INSERT INTO `students` VALUES (990674, '丁晓品', 24, '男', '黑龙江');
INSERT INTO `students` VALUES (990676, '贺正路', 23, '男', '湖南');
INSERT INTO `students` VALUES (990684, '刘文曲', 24, '女', '辽宁');
INSERT INTO `students` VALUES (991091, '程会民', 23, '男', '山西');

-- ----------------------------
-- Table structure for teachers
-- ----------------------------
DROP TABLE IF EXISTS `teachers`;
CREATE TABLE `teachers`  (
  `TNO` int(11) NOT NULL,
  `TNAME` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `AGE` int(11) NULL DEFAULT NULL,
  `PS` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`TNO`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of teachers
-- ----------------------------
INSERT INTO `teachers` VALUES (1420, '周振虎', 38, '副教授');
INSERT INTO `teachers` VALUES (1433, '王志伟', 28, '副教授');
INSERT INTO `teachers` VALUES (1481, '刘集春', 30, '讲师');

-- ----------------------------
-- Table structure for teaching
-- ----------------------------
DROP TABLE IF EXISTS `teaching`;
CREATE TABLE `teaching`  (
  `CNO` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '课程号',
  `CLASS` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '班级',
  `TNO` decimal(4, 0) NULL DEFAULT NULL COMMENT '教师号',
  `SNUM` int(11) NULL DEFAULT NULL COMMENT '学生数',
  PRIMARY KEY (`CNO`, `CLASS`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of teaching
-- ----------------------------
INSERT INTO `teaching` VALUES ('C1', 'E851', 1420, 30);
INSERT INTO `teaching` VALUES ('C1', 'E852', 1420, 28);
INSERT INTO `teaching` VALUES ('C2', 'E851', 1420, 22);
INSERT INTO `teaching` VALUES ('C3', 'E651', 1481, 30);
INSERT INTO `teaching` VALUES ('C3', 'E852', 1481, 28);
INSERT INTO `teaching` VALUES ('C4', 'E851', 1433, 24);

SET FOREIGN_KEY_CHECKS = 1;
