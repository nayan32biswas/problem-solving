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

## Sorting and Grouping

### 2356. Number of Unique Subjects Taught by Each Teacher

```sql
select
    teacher_id,
    count(distinct subject_id) as cnt
from teacher
group by teacher_id;
```

### 1141. User Activity for the Past 30 Days I

```sql
select 
    a.activity_date as day,
    count(distinct a.user_id) as active_users
from activity a
where a.activity_date between '2019-06-28' and '2019-07-27'
group by a.activity_date
```

### 1070. Product Sales Analysis III

```sql
select
    s.product_id,
    s.year as first_year,
    s.quantity,
    s.price
from sales s
where (s.product_id, s.year) in (
    select product_id, min(year) as fist_year from sales group by product_id
);
```

### 596. Classes With at Least 5 Students

```sql
select class from courses
group by class having count(*)>=5;
```

### 1729. Find Followers Count

```sql
select
    user_id,
    count(*) as followers_count
from followers
group by user_id
order by user_id asc;
```

### 619. Biggest Single Number

```sql
select
    max(num) as num
from (
    select num from mynumbers mn
    group by mn.num having count(*) = 1
);
```

### 1045. Customers Who Bought All Products

```sql
with total_product as (
    select count(*) as total from product
)
select
    c.customer_id
from customer c
cross join total_product
group by c.customer_id, total_product.total
having count(distinct c.product_key) = total_product.total;
```

## Advanced Select and Joins

### 1731. The Number of Employees Which Report to Each Employee

```sql
select
    e.employee_id,
    e.name,
    count(*) as reports_count,
    round(avg(e1.age)) as average_age
from Employees e
join Employees e1 on e.employee_id = e1.reports_to
group by e.employee_id, e.name
order by e.employee_id;
```

### 1789. Primary Department for Each Employee

```sql
select
    x.employee_id,
    x.department_id
from (
    select
        e.*,
        row_number() over(partition by e.employee_id order by primary_flag desc) as rn
    from employee e
) as x
where x.rn = 1;
```

### 610. Triangle Judgement

```sql
select
    *,
    case
        when (
            x+y>z and x+z>y and y+z>x
        ) then 'Yes'
        else 'No'
    end as triangle
from Triangle
```

### 180. Consecutive Numbers

```sql
-- Solution One
select
    distinct l.num as ConsecutiveNums
from logs l
join logs l1 on l.id = l1.id + 1
join logs l2 on l.id = l2.id + 2
where l.num = l1.num and l.num = l2.num

-- Solution Two
select
    distinct x.num as ConsecutiveNums
from (
    select
        *,
        lag(num, 1, null) over(order by id) as prev_num,
        lead(num, 1, null) over(order by id) as next_num
    from logs
    order by id
) as x
where x.num = x.prev_num and x.num = x.next_num;
```

### 1164. Product Price at a Given Date

```sql
with products_with_lower_date as (
    select * from products where change_date <= '2019-08-16'
),
products_expluding_future as (
    select * from (
        select
            *,
            row_number() over(partition by product_id order by change_date desc) as rn
        from products_with_lower_date
    ) as x
    where x.rn = 1
)
select
    product_id,
    new_price as price
from products_expluding_future
union
select
    product_id,
    10 as price
from products
where product_id not in (
    select distinct product_id from products where change_date <= '2019-08-16'
);
```

## 1204. Last Person to Fit in the Bus

```sql
select
    x.person_name
from (
    select
        *,
        sum(weight) over(order by turn) as seq_sum
    from Queue
) as x
where seq_sum <= 1000
order by x.turn desc
limit 1;
```

### 1907. Count Salary Categories

```sql
with income_range as (
    select
        case
            when income < 20000 then 'Low Salary'
            when income between 20000 and 50000 then 'Average Salary'
            else 'High Salary'
        end as category,
        count(*) as accounts_count
    from accounts
    group by category
)
select
    a.category,
    case
        when ir.accounts_count is null then 0
        else ir.accounts_count
    end
from income_range ir
full outer join unnest(
    ARRAY['Low Salary', 'Average Salary', 'High Salary']::text[]
) AS a(category) on a.category = ir.category;
```

## Subqueries

### 1978. Employees Whose Manager Left the Company

```sql
select
    e.employee_id
from employees e
where
    e.manager_id is not null
    and e.salary < 30000
    and e.manager_id not in (
        select employee_id from employees
    )
order by e.employee_id asc;
```

### 626. Exchange Seats

```sql
select
    case
        when id%2=0 then id-1
        when id=(select max(id) from seat) then id
        else id+1
    end as id,
    student
from seat
order by id asc;
```

### 1341. Movie Rating

```sql
(
    select
        name as results
    from movierating mr
    join users u on mr.user_id = u.user_id
    group by name
    order by count(*) desc, name asc
    limit 1
)
union all
(
    select
        title as results
    from movierating mr
    join movies m on mr.movie_id = m.movie_id
    where created_at between '2020-02-01' and '2020-02-29'
    group by title
    order by avg(rating) desc, title asc
    limit 1
);
```

### 1321. Restaurant Growth

```sql
with sales as (
    select
        visited_on,
        sum(amount) as amount
    from customer
    group by visited_on
    order by visited_on asc
)
select
    visited_on,
    sum(amount) over (
        order by visited_on
        ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
    ) as amount,
    round(
        avg(amount) over (
            order by visited_on
            ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
        ),
        2
    ) as average_amount
from sales
order by visited_on
offset 6;
```

### 602. Friend Requests II: Who Has the Most Friends

```sql
select id, count(*) as num from (
    select requester_id as id from RequestAccepted
    union all
    select accepter_id as id from RequestAccepted
)
group by id
order by num desc
limit 1;
```
