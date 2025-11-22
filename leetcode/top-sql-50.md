# Top SQL 50

## Select

### 1757. Recyclable and Low Fat Products

```sql
select product_id from products where low_fats = 'Y' and recyclable = 'Y';
```

### 584. Find Customer Referee

```sql
select name from customer where referee_id is null or referee_id != 2;
```

### 595. Big Countries

```sql
select name, population, area from world
where area >= 3000000 or population >= 25000000;
```

### 1148. Article Views I

```sql
select distinct author_id as id from views
where author_id = viewer_id
order by id;
```

### 1683. Invalid Tweets

```sql
select tweet_id from tweets where LENGTH(content) > 15;
```

## Basic Joins

### 1378. Replace Employee ID With The Unique Identifier

```sql
select eUni.unique_id, e.name from employees e
left join EmployeeUNI eUni on eUni.id = e.id;
```

### 1068. Product Sales Analysis I

```sql
select p.product_name, s.year, s.price from sales s
join product p on p.product_id = s.product_id;
```

### 1581. Customer Who Visited but Did Not Make Any Transactions

```sql
select v.customer_id, count(v.customer_id) as count_no_trans from visits v
left join transactions t on t.visit_id = v.visit_id
where t.transaction_id is null
group by v.customer_id;
```

### 197. Rising Temperature

```sql
select two.id from weather one
join weather two on DATEDIFF(two.recordDate, one.recordDate) = 1
where one.temperature < two.temperature;
```

### 1661. Average Time of Process per Machine

```sql
select a1.machine_id, round(avg(a2.timestamp - a1.timestamp), 3) as processing_time from activity a1
join activity a2 on 
    a2.machine_id = a1.machine_id
    and a2.process_id = a1.process_id
    and a2.activity_type = 'end'
    and a1.activity_type = 'start'
group by a1.machine_id;
```

### 577. Employee Bonus

```sql
select e.name, b.bonus
from employee e
left join bonus b on b.empId = e.empId
where b.bonus is null or b.bonus < 1000
```

### 1280. Students and Examinations

```sql
select
    s.student_id,
    s.student_name,
    sub.subject_name,
    sum(CASE WHEN e.student_id IS NOT NULL THEN 1 ELSE 0 END) as attended_exams
from students s
cross join subjects sub
left join examinations e
    on e.student_id = s.student_id and e.subject_name = sub.subject_name
group by s.student_id, s.student_name, sub.subject_name
order by s.student_id, sub.subject_name;
```

### 570. Managers with at Least 5 Direct Reports

```sql
select name from employee
where id in (
    select managerId from employee
    where managerId is not null
    group by managerId having count(managerId)>=5
);
```

### 1934. Confirmation Rate

```sql
select
    s.user_id,
    ROUND(
        sum(
            CASE WHEN c.action = 'confirmed' THEN 1 ELSE 0 END
        ) / COUNT(*),
        2
    ) as confirmation_rate
from signups s
left join confirmations c on s.user_id = c.user_id
group by s.user_id
```
