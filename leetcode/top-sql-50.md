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

## Basic Aggregate Functions

### 620. Not Boring Movies

```sql
select * from cinema c
where c.id % 2 <> 0 and c.description != 'boring'
order by c.rating desc;
```

### 1251. Average Selling Price

```sql
-- Solution One
select
    p.product_id,
    round(
        case
            when sum(u.units) is null then 0
            else sum(p.price * u.units) / sum(u.units)
        end,
        2
    ) as average_price
from prices p
left join UnitsSold u on 
    p.product_id = u.product_id
    and u.purchase_date between p.start_date and p.end_date
group by p.product_id

-- Solution Two
select
    p.product_id,
    round(
        coalesce(
            sum(p.price*u.units) / nullif(sum(u.units), 0),
            0
        ),
        2
    ) as average_price
from prices p
left join UnitsSold u on 
    p.product_id = u.product_id
    and u.purchase_date between p.start_date and p.end_date
group by p.product_id;
```

### 1075. Project Employees I

```sql
select
    p.project_id,
    round(avg(e.experience_years), 2) as average_years
from project p
join employee e on p.employee_id = e.employee_id
group by p.project_id;
```

### 1633. Percentage of Users Attended a Contest

```sql
-- Solution One
with user_count as (select count(*) as total from users)
select
    r.contest_id,
    round(
        count(*)/user_count.total * 100,
        2
    ) as percentage
from users u
join register r on u.user_id = r.user_id
CROSS JOIN user_count
group by r.contest_id
order by percentage desc, r.contest_id asc;


-- Solution Two
select
  r.contest_id,
  round(
    (count(r.contest_id) / (select count(*) from Users)) * 100,
    2
  ) as percentage
from Users as u, Register as r
where u.user_id = r.user_id
group by r.contest_id
order by percentage desc, contest_id asc;
```

### 1211. Queries Quality and Percentage

```sql
select
    query_name,
    round(
        sum(rating/position) / count(*),
        2
    ) as quality,
    round(
        (sum(
            case when rating < 3 then 1 else 0 end
        ) / count(*) * 100),
        2
    ) as poor_query_percentage
from queries
group by query_name
```

### 1193. Monthly Transactions I

```sql
select
    TO_CHAR(date_trunc('month', t.trans_date), 'YYYY-MM') as month,
    t.country as country,
    count(*) as trans_count,
    sum(
        case when t.state='approved' then 1 else 0 end
    ) as approved_count,
    sum(t.amount) as trans_total_amount,
    sum(
        case when t.state='approved' then t.amount else 0 end
    ) as approved_total_amount
from transactions t
group by t.country, date_trunc('month', t.trans_date);
```

### 1174. Immediate Food Delivery II

```sql
select 
    round(
        (
            sum(
                case
                    when d.order_date = d.customer_pref_delivery_date then 1.0
                    else 0.0
                end
            )
            / count(*)
        ) * 100,
        2
    ) as immediate_percentage
from Delivery d
where (d.customer_id, d.order_date) in (
    select customer_id, min(order_date) from Delivery group by customer_id
);
```

### 550. Game Play Analysis IV

```sql
with first_login as (
    select player_id, min(event_date) as event_date from activity group by player_id
)
select
    round(
        (count(a.*) * 1.00) / (select count(*) from first_login),
        2
    ) as fraction
from activity a
join first_login fl on
    a.player_id = fl.player_id
    and (a.event_date - fl.event_date) = 1;
```
