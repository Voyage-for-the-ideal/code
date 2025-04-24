-- Active: 1744872911483@@127.0.0.1@3306@lab1

--任务一
CREATE DATABASE lab1;
CREATE TABLE Student(
    SNO CHAR(11) NOT NULL PRIMARY KEY,
    NAME VARCHAR(3) NOT NULL ,
    GENDER VARCHAR(6) NOT NULL,
    AGE INT NOT NULL,
    DEPART INT
    );
CREATE TABLE Teacher(
    TNO CHAR(7) NOT NULL PRIMARY KEY,
    NAME VARCHAR(3) NOT NULL,
    GENDER VARCHAR(6) NOT NULL,
    BRITHDAY DATE,
    POSITION VARCHAR(20) NOT NULL,
    DEPART INT
    );
CREATE TABLE Course(
    CNO CHAR(8) NOT NULL PRIMARY KEY,
    NAME VARCHAR(30) NOT NULL,
    CPNO CHAR(8),
    FOREIGN KEY (CPNO) REFERENCES Course(CNO),
    CCREDIT FLOAT,
    TNO CHAR(7),
    FOREIGN KEY (TNO) REFERENCES Teacher(TNO)
);
--用FOREIGN KEY约束来实现外键约束
ALTER TABLE Course
MODIFY COLUMN NAME VARCHAR(30) NOT NULL;


CREATE TABLE Score(
    SNO CHAR(11),
    FOREIGN KEY (SNO) REFERENCES Student(SNO),
    CNO CHAR(8),
    FOREIGN KEY (CNO) REFERENCES Course(CNO),
    PRIMARY KEY(SNO, CNO),
    DEGREE INT,
    SEMESTER CHAR(6)
);
--查看文件位置
SHOW VARIABLES LIKE 'secure_file_priv';

-- --调试
-- SET FOREIGN_KEY_CHECKS = 0;
-- TRUNCATE TABLE Score;
-- TRUNCATE TABLE Course;
-- TRUNCATE TABLE Teacher;
-- TRUNCATE TABLE Student;
-- SET FOREIGN_KEY_CHECKS = 1;
-- SHOW TABLES;
-- SELECT * FROM Student;
-- --调试
SELECT * FROM Student;
LOAD DATA INFILE "C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\Teacher.csv" INTO TABLE Teacher
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;    

LOAD DATA INFILE "C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\Student.csv" INTO TABLE Student
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;

SET FOREIGN_KEY_CHECKS = 0;
LOAD DATA INFILE "C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\Course.csv" INTO TABLE Course
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;
SET FOREIGN_KEY_CHECKS = 1;

LOAD DATA INFILE "C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\Score.csv" INTO TABLE Score
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;

--任务二
--修改基本表
--1
ALTER TABLE student
ADD COLUMN BIRTHDAY INT;

--2
SELECT NAME,YEAR(NOW())-AGE FROM Student;

--3
UPDATE Student SET AGE=AGE-2;
ALTER TABLE student
MODIFY COLUMN AGE CHAR NOT NULL;

--4
ALTER TABLE student
DROP COLUMN BIRTHDAY;

--5
CREATE TABLE student_course(
    SNO CHAR(11) NOT NULL PRIMARY KEY,
    NUM_COURSE INT NOT NULL,
    FOREIGN KEY (SNO) REFERENCES Student(SNO)
);
DESCRIBE student_course;
--6
INSERT INTO student_course(SNO,NUM_COURSE)
SELECT SNO,COUNT(*) FROM score GROUP BY SNO;
SELECT * FROM student_course;
--7
DELETE FROM student_course WHERE NUM_COURSE=3 or NUM_COURSE=1;
DROP TABLE student_course;

--8
ALTER TABLE student
ADD constraint name_unique UNIQUE(NAME);
ALTER TABLE student
DROP constraint name_unique;
--如果命名了完整性约束的名称，则可以使用该名称来删除完整性约束
--如果没有命名，就使用INDEX NAME来删除完整性约束

--9
/*为什么这里要单独创建索引，是前面的哪一步导致需要这么做
如果不这么做的话，删掉主键就相当于删掉了外键？*/
-- 为SNO列创建索引
ALTER TABLE score ADD INDEX idx_sno (SNO);
-- 为CNO列创建索引
ALTER TABLE score ADD INDEX idx_cno (CNO);
ALTER TABLE score
DROP PRIMARY KEY;
ALTER TABLE score
ADD PRIMARY KEY(SNO,CNO,SEMESTER);
--10
ALTER TABLE student
RENAME COLUMN DEPART TO DEPARTMENT;
DESCRIBE student;
--11
DELETE FROM Score WHERE SNO IN (SELECT SNO FROM student WHERE DEPARTMENT=229);
DELETE score FROM score
JOIN (
    SELECT SNO,MIN(DEGREE) AS min_degree
    FROM score
    GROUP BY SNO
) AS min_score
ON score.SNO=min_score.SNO AND score.DEGREE=min_score.min_degree; 
/*使用了相当高级的多表删除法
（自己和自己join以规避 delete语句中的WHERE无法对目标组执行操作 的限制）*/

--12
ALTER TABLE score
ADD COLUMN GRADE CHAR;
UPDATE score SET GRADE = 
    CASE 
        WHEN DEGREE >= 90 THEN 'A'
        WHEN DEGREE >= 80 THEN 'B' 
        WHEN DEGREE >= 70 THEN 'C'
        WHEN DEGREE >= 60 THEN 'D'
        ELSE 'F'
    END;
SELECT * FROM student;

--13
INSERT INTO student
VALUES("PB231518080","WYL","male",20,229)

--查询
--1
SELECT SNO,NAME,`DEPARTMENT`
FROM student
WHERE DEPARTMENT=229;
--2
SELECT SNO,NAME
FROM student
WHERE DEPARTMENT=229 AND SNO!="PB231518080";
--3
SELECT SNO,NAME,`DEPARTMENT`
FROM student
WHERE DEPARTMENT!=(
    SELECT DEPARTMENT FROM student WHERE NAME="YH"
);
--4
SELECT SNO,NAME
FROM student
WHERE DEPARTMENT NOT IN(
    SELECT DEPARTMENT FROM student WHERE NAME="YH"or NAME="WYL"
);
--5
SELECT TNO,NAME
FROM teacher
WHERE DEPART=229;
--6
SELECT DEPART,COUNT(*) AS PERSON
FROM teacher
WHERE DEPART=11 or DEPART=229
GROUP BY DEPART;
/*当 SQL 查询包含聚合函数（如 COUNT, SUM, AVG）时，SELECT 列表中所有非聚合的列必须出现在 GROUP BY 子句中*/
--7
SELECT SNO,NAME,AGE MAX_AGE
FROM student
WHERE AGE=(SELECT MAX(AGE) FROM student);
/*聚合函数（如 COUNT, SUM, AVG）应在 SELECT 或 HAVING 子句中使用，而不是在 WHERE 子句中*/
--8
SELECT SNO,NAME,AGE MIN_AGE
FROM student
WHERE AGE=(SELECT MIN(AGE) FROM student) AND DEPARTMENT=229;
--9
SELECT stu.SNO,stu.NAME,sco.DEGREE
FROM student stu,score sco
WHERE stu.SNO=sco.SNO 
    AND sco.DEGREE<=80
    AND sco.CNO=(
        SELECT CNO FROM course WHERE NAME='DB_Design'
    );
--10
SELECT DISTINCT stu.SNO,stu.NAME
FROM student stu
JOIN score sc ON stu.SNO=sc.SNO
JOIN course c ON sc.CNO=c.CNO
JOIN teacher t ON c.TNO=t.TNO
WHERE t.NAME="ZDH";

--11(输出DB_Design课程所有学生的学号和成绩)
SELECT score.SNO,score.DEGREE,score.`GRADE`
FROM score
JOIN course ON score.CNO=course.CNO
WHERE course.NAME="DB_Design"
ORDER BY DEGREE DESC;
--12
SELECT course.CNO,course.NAME,AVG(score.DEGREE) AS AVG_DEGREE
FROM course
LEFT JOIN score ON score.CNO=course.CNO
GROUP BY course.CNO;
/*LEFT JOIN 右表中没有匹配的行时，结果集中的列值为NULL*/
--13
SELECT course.CNO,course.NAME,AVG(score.DEGREE)
FROM course
LEFT JOIN score ON score.CNO=course.CNO
WHERE course.CCREDIT>3
GROUP BY course.CNO;
--14
/*使用双NOT EXISTS*/
SELECT SNO FROM student
WHERE NOT EXISTS(
    SELECT * FROM course
    WHERE course.TNO="TA90023"
    AND NOT EXISTS(
        SELECT * FROM score
        WHERE score.SNO=student.SNO AND score.CNO=course.CNO
    )
);

/*即，选了ZDH的课 且 这种课的数量等于ZDH开课数量*/
SELECT SNO
FROM (
    SELECT SNO, COUNT(DISTINCT CNO) cnt
    FROM Score
    WHERE CNO IN (SELECT CNO FROM Course WHERE TNO = 'TA90023')
    GROUP BY SNO
) t
WHERE cnt = (SELECT COUNT(DISTINCT CNO) FROM Course WHERE TNO = 'TA90023');

--15
SELECT score.CNO,course.NAME,
    MAX(score.DEGREE) AS MAX_DEGREE,
    MIN(score.DEGREE) AS MIN_DEGREE,
    MAX(score.DEGREE)-MIN(score.DEGREE) AS DIFF_DEGREE
FROM score
JOIN course ON score.CNO=course.CNO
GROUP BY score.CNO;
--16
SELECT SNO,COUNT(*) AS NUM
FROM score
WHERE DEGREE<=78
GROUP BY SNO;
--17
SELECT DISTINCT teacher.TNO,teacher.NAME
FROM teacher
JOIN course ON teacher.TNO=course.TNO
WHERE EXISTS(
    SELECT * FROM score
    WHERE score.DEGREE<72
);
--18
SELECT student.SNO,student.NAME
FROM student
WHERE student.SNO IN(
    SELECT SNO
    FROM score
    GROUP BY SNO
    HAVING COUNT(DISTINCT CNO)>6
);

--19
/*即，选了ZY同学选的课 且 这种课的数量等于ZY同学选课数量*/
SELECT SNO
FROM (
    SELECT SNO,COUNT(DISTINCT CNO) cnt
    FROM score
    WHERE CNO IN (SELECT CNO FROM score WHERE SNO="PB230000002")
    GROUP BY SNO
)AS t
WHERE cnt=(SELECT COUNT(DISTINCT CNO) FROM Score WHERE SNO="PB230000002");  

--20
SELECT student.NAME,AVG(score.DEGREE)
FROM student
LEFT JOIN score ON student.SNO=score.SNO
GROUP BY student.SNO;

--21
SELECT student.DEPARTMENT,COUNT(*),AVG(score.DEGREE)
FROM student
LEFT JOIN score ON student.SNO=score.SNO
GROUP BY student.DEPARTMENT
/*229系的学生都不在score里面*/

--22
--EXISTS语句只返回True False
SELECT DISTINCT student.NAME
FROM student
JOIN score ON student.SNO=score.SNO
WHERE NOT EXISTS(
    SELECT * FROM score
    JOIN course ON score.CNO=course.CNO
    WHERE score.SNO=course.CNO AND (course.NAME="DB_Design" OR course.NAME="Data_Mining")
);

--23
SELECT course.NAME,stu_avg_age
FROM course
LEFT JOIN (SELECT score.CNO,AVG(student.AGE) as stu_avg_age
FROM student
RIGHT JOIN score ON student.SNO=score.SNO
GROUP BY score.CNO
)
AS t ON course.CNO=t.CNO
GROUP BY course.CNO;

--24
SELECT DISTINCT student.SNO,student.NAME
FROM student
JOIN score
WHERE student.SNO=score.SNO
    AND score.CNO IN(
        SELECT CNO FROM course
        WHERE course.NAME LIKE "%Computer%"
    )
/*%是通配符，表示任意字符*/

--25
SELECT s.SNO, s.CNO, s.DEGREE
FROM score s
JOIN (
    SELECT CNO, AVG(DEGREE) AS avg_degree
    FROM score
    GROUP BY CNO
) AS t ON s.CNO = t.CNO
WHERE s.DEGREE BETWEEN (t.avg_degree - 5) AND (t.avg_degree + 5);

--26
SELECT c.CNO,c.NAME,num_A
FROM course c
JOIN (
    SELECT score.CNO,COUNT(*) AS num_A
    FROM score
    WHERE score.GRADE="A"
    GROUP BY score.CNO
) AS t ON c.CNO=t.CNO
ORDER BY num_A DESC;
#DESC表示降序排列

--27
SELECT NAME
FROM course 
WHERE NAME REGEXP '.*_[^_]*r.*';
/*  .*匹配任意字符
    [^_]*匹配非下划线字符
    r表示正则表达式中的字符r
*/
--28
SELECT teacher.TNO,CNO,max_avg_grade
FROM teacher
LEFT JOIN(SELECT course.TNO,course.CNO,max_avg_grade
        FROM course
        JOIN(
    SELECT CNO,MAX(avg_grade) AS max_avg_grade
    FROM (
        SELECT CNO,AVG(DEGREE) AS avg_grade
        FROM score
        GROUP BY CNO
    ) AS t1
    GROUP BY CNO
)AS t2 ON course.CNO=t2.CNO
)AS t3 ON teacher.TNO=t3.TNO;
/*使用了三层join
    1.内层查询计算每门课程的平均成绩
    2.中间查询计算每门课程的最大平均成绩
    3.外层查询将教师表和课程表连接起来，获取教师的工号和课程编号以及最大平均成绩
    PS：如果没有第三层，则无法返回那些没有授课的老师的信息
*/


--索引
--1
CREATE INDEX NAME_INDEX ON Student(NAME);

--2
CREATE UNIQUE INDEX CNO_INDEX ON Course(CNO);

--3
CREATE INDEX score_idx ON Score(SNO ASC, DEGREE DESC);

--4
SHOW INDEX FROM score;

--5
DROP INDEX CNO_INDEX ON Course;


--视图 
--1
CREATE VIEW db_229_student
AS
SELECT SNO,NAME,GENDER,AGE,DEPARTMENT
FROM student
WHERE DEPARTMENT=229
WITH CHECK OPTION;

--2
UPDATE db_229_student
SET 
    SNO="PB231518081,
    NAME="WYQ
WHERE SNO="PB230000031";

SELECT * FROM student;
--3
SELECT SNO,NAME,GENDER
FROM db_229_student
WHERE AGE<21;

--4
INSERT INTO student
VALUES("SA242290001","QWE","male",22,229);
SELECT * FROM db_229_student;

--5
DROP VIEW db_229_student;


--触发器
--1
CREATE TABLE teacher_salary(
    TNO CHAR(7) NOT NULL PRIMARY KEY,
    SAL FLOAT NOT NULL,
    FOREIGN KEY (TNO) REFERENCES Teacher(TNO)
);

--2 
--使用NOT IN
CREATE TRIGGER validate_tno_insert
BEFORE INSERT ON teacher_salary
FOR EACH ROW
BEGIN
    IF (NEW.TNO NOT IN (SELECT TNO FROM Teacher)) 
    THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'TNO not in Teacher';
    END IF;
END;

--使用NOT EXISTS
CREATE TRIGGER validate_tno_update
BEFORE UPDATE ON teacher_salary
FOR EACH ROW
BEGIN
    IF NOT EXISTS (
        SELECT 1 
        FROM teacher 
        WHERE TNO = NEW.TNO
    ) THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = '工号不存在于teacher表';
    END IF;
END;
/*NOT IN会返回整个表，而NOT EXISTS+SELECT 1只会进行存在性判断。
后者比前者效率更高*/

--抛出错误用例
INSERT INTO teacher_salary (TNO, SAL)
VALUES ('TA11111', 5000);

--3
/*INSERT触发器中只有NEW
UPDATE触发器中只有OLD和NEW*/
CREATE TRIGGER sal_update
BEFORE UPDATE ON teacher_salary
FOR EACH ROW
BEGIN
    DECLARE teacher_position VARCHAR(50);
    DECLARE original_sal FLOAT;
    SET original_sal = OLD.SAL;
  
    -- 获取职称（仅查询一次）
    SELECT POSITION INTO teacher_position 
    FROM teacher 
    WHERE TNO = NEW.TNO;

    CASE teacher_position
        WHEN 'Instructor' THEN
            IF NEW.SAL < 4000 THEN
                SET NEW.SAL = 4000;
                
            ELSEIF NEW.SAL > 7000 THEN
                SET NEW.SAL = 7000;
            END IF;
        WHEN 'Associate Professor' THEN
            IF NEW.SAL < 7000 THEN
                SET NEW.SAL = 7000;
            ELSEIF NEW.SAL > 10000 THEN
                SET NEW.SAL = 10000;
            END IF;
        WHEN 'Professor' THEN
            IF NEW.SAL < 10000 THEN
                SET NEW.SAL = 10000;
            ELSEIF NEW.SAL > 13000 THEN
                SET NEW.SAL = 13000;
            END IF;
    END CASE;
    IF NEW.SAL != original_sal THEN
        SIGNAL SQLSTATE '01000' -- 使用警告级别的SQLSTATE
        SET MESSAGE_TEXT = '已成功修改';
    END IF;
END;
CREATE TRIGGER sal_insert
BEFORE INSERT ON teacher_salary
FOR EACH ROW
BEGIN
    DECLARE teacher_position VARCHAR(50);
  
    -- 获取职称（仅查询一次）
    SELECT POSITION INTO teacher_position 
    FROM teacher 
    WHERE TNO = NEW.TNO;

    CASE teacher_position
        WHEN 'Instructor' THEN
            IF NEW.SAL < 4000 THEN
                SET NEW.SAL = 4000;
            ELSEIF NEW.SAL > 7000 THEN
                SET NEW.SAL = 7000;
            END IF;
        WHEN 'Associate Professor' THEN
            IF NEW.SAL < 7000 THEN
                SET NEW.SAL = 7000;
            ELSEIF NEW.SAL > 10000 THEN
                SET NEW.SAL = 10000;
            END IF;
        WHEN 'Professor' THEN
            IF NEW.SAL < 10000 THEN
                SET NEW.SAL = 10000;
            ELSEIF NEW.SAL > 13000 THEN
                SET NEW.SAL = 13000;
            END IF;
    END CASE;
        SIGNAL SQLSTATE '01000' -- 使用警告级别的SQLSTATE
        SET MESSAGE_TEXT = '触发修改，已成功调整薪资';
END;

--insert修改样例
INSERT INTO teacher_salary (TNO, SAL)
VALUES ('TA90021', 8000);
SELECT TNO,SAL
FROM teacher_salary
WHERE TNO='TA90021';

--update修改样例
UPDATE teacher_salary SET SAL=3000 WHERE TNO='TA90021';
SELECT TNO,SAL
FROM teacher_salary
WHERE TNO='TA90021';

DROP TRIGGER sal_insert;
DROP TRIGGER sal_update;
DROP TRIGGER validate_tno_insert;


--空值
UPDATE score
JOIN course ON score.CNO=course.CNO
SET score.DEGREE=NULL
WHERE course.NAME="DB_Design";

SELECT s.SNO,s.DEGREE
FROM score s
ORDER BY s.DEGREE ASC;

--开放题
--将所有学生的成绩加10%,91分以上的修改为100
UPDATE score SET DEGREE=DEGREE*1.1 WHERE DEGREE<=91;
UPDATE score SET DEGREE=100 WHERE DEGREE>91;

--删去自己的信息
DELETE FROM student WHERE SNO="PB231518080";
