-- Active: 1744872911483@@127.0.0.1@3306@lab1

--����һ
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
--��FOREIGN KEYԼ����ʵ�����Լ��
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
--�鿴�ļ�λ��
SHOW VARIABLES LIKE 'secure_file_priv';

-- --����
-- SET FOREIGN_KEY_CHECKS = 0;
-- TRUNCATE TABLE Score;
-- TRUNCATE TABLE Course;
-- TRUNCATE TABLE Teacher;
-- TRUNCATE TABLE Student;
-- SET FOREIGN_KEY_CHECKS = 1;
-- SHOW TABLES;
-- SELECT * FROM Student;
-- --����
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

--�����
--�޸Ļ�����
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
--���������������Լ�������ƣ������ʹ�ø�������ɾ��������Լ��
--���û����������ʹ��INDEX NAME��ɾ��������Լ��

--9
/*Ϊʲô����Ҫ����������������ǰ�����һ��������Ҫ��ô��
�������ô���Ļ���ɾ���������൱��ɾ���������*/
-- ΪSNO�д�������
ALTER TABLE score ADD INDEX idx_sno (SNO);
-- ΪCNO�д�������
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
/*ʹ�����൱�߼��Ķ��ɾ����
���Լ����Լ�join�Թ�� delete����е�WHERE�޷���Ŀ����ִ�в��� �����ƣ�*/

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

--��ѯ
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
/*�� SQL ��ѯ�����ۺϺ������� COUNT, SUM, AVG��ʱ��SELECT �б������зǾۺϵ��б�������� GROUP BY �Ӿ���*/
--7
SELECT SNO,NAME,AGE MAX_AGE
FROM student
WHERE AGE=(SELECT MAX(AGE) FROM student);
/*�ۺϺ������� COUNT, SUM, AVG��Ӧ�� SELECT �� HAVING �Ӿ���ʹ�ã��������� WHERE �Ӿ���*/
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

--11(���DB_Design�γ�����ѧ����ѧ�źͳɼ�)
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
/*LEFT JOIN �ұ���û��ƥ�����ʱ��������е���ֵΪNULL*/
--13
SELECT course.CNO,course.NAME,AVG(score.DEGREE)
FROM course
LEFT JOIN score ON score.CNO=course.CNO
WHERE course.CCREDIT>3
GROUP BY course.CNO;
--14
/*ʹ��˫NOT EXISTS*/
SELECT SNO FROM student
WHERE NOT EXISTS(
    SELECT * FROM course
    WHERE course.TNO="TA90023"
    AND NOT EXISTS(
        SELECT * FROM score
        WHERE score.SNO=student.SNO AND score.CNO=course.CNO
    )
);

/*����ѡ��ZDH�Ŀ� �� ���ֿε���������ZDH��������*/
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
/*����ѡ��ZYͬѧѡ�Ŀ� �� ���ֿε���������ZYͬѧѡ������*/
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
/*229ϵ��ѧ��������score����*/

--22
--EXISTS���ֻ����True False
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
/*%��ͨ�������ʾ�����ַ�*/

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
#DESC��ʾ��������

--27
SELECT NAME
FROM course 
WHERE NAME REGEXP '.*_[^_]*r.*';
/*  .*ƥ�������ַ�
    [^_]*ƥ����»����ַ�
    r��ʾ������ʽ�е��ַ�r
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
/*ʹ��������join
    1.�ڲ��ѯ����ÿ�ſγ̵�ƽ���ɼ�
    2.�м��ѯ����ÿ�ſγ̵����ƽ���ɼ�
    3.����ѯ����ʦ��Ϳγ̱�������������ȡ��ʦ�Ĺ��źͿγ̱���Լ����ƽ���ɼ�
    PS�����û�е����㣬���޷�������Щû���ڿε���ʦ����Ϣ
*/


--����
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


--��ͼ 
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


--������
--1
CREATE TABLE teacher_salary(
    TNO CHAR(7) NOT NULL PRIMARY KEY,
    SAL FLOAT NOT NULL,
    FOREIGN KEY (TNO) REFERENCES Teacher(TNO)
);

--2 
--ʹ��NOT IN
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

--ʹ��NOT EXISTS
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
        SET MESSAGE_TEXT = '���Ų�������teacher��';
    END IF;
END;
/*NOT IN�᷵����������NOT EXISTS+SELECT 1ֻ����д������жϡ�
���߱�ǰ��Ч�ʸ���*/

--�׳���������
INSERT INTO teacher_salary (TNO, SAL)
VALUES ('TA11111', 5000);

--3
/*INSERT��������ֻ��NEW
UPDATE��������ֻ��OLD��NEW*/
CREATE TRIGGER sal_update
BEFORE UPDATE ON teacher_salary
FOR EACH ROW
BEGIN
    DECLARE teacher_position VARCHAR(50);
    DECLARE original_sal FLOAT;
    SET original_sal = OLD.SAL;
  
    -- ��ȡְ�ƣ�����ѯһ�Σ�
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
        SIGNAL SQLSTATE '01000' -- ʹ�þ��漶���SQLSTATE
        SET MESSAGE_TEXT = '�ѳɹ��޸�';
    END IF;
END;
CREATE TRIGGER sal_insert
BEFORE INSERT ON teacher_salary
FOR EACH ROW
BEGIN
    DECLARE teacher_position VARCHAR(50);
  
    -- ��ȡְ�ƣ�����ѯһ�Σ�
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
        SIGNAL SQLSTATE '01000' -- ʹ�þ��漶���SQLSTATE
        SET MESSAGE_TEXT = '�����޸ģ��ѳɹ�����н��';
END;

--insert�޸�����
INSERT INTO teacher_salary (TNO, SAL)
VALUES ('TA90021', 8000);
SELECT TNO,SAL
FROM teacher_salary
WHERE TNO='TA90021';

--update�޸�����
UPDATE teacher_salary SET SAL=3000 WHERE TNO='TA90021';
SELECT TNO,SAL
FROM teacher_salary
WHERE TNO='TA90021';

DROP TRIGGER sal_insert;
DROP TRIGGER sal_update;
DROP TRIGGER validate_tno_insert;


--��ֵ
UPDATE score
JOIN course ON score.CNO=course.CNO
SET score.DEGREE=NULL
WHERE course.NAME="DB_Design";

SELECT s.SNO,s.DEGREE
FROM score s
ORDER BY s.DEGREE ASC;

--������
--������ѧ���ĳɼ���10%,91�����ϵ��޸�Ϊ100
UPDATE score SET DEGREE=DEGREE*1.1 WHERE DEGREE<=91;
UPDATE score SET DEGREE=100 WHERE DEGREE>91;

--ɾȥ�Լ�����Ϣ
DELETE FROM student WHERE SNO="PB231518080";
