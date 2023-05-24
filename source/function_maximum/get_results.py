file_name = input()
exp_result = float(input())

file = open("results/" + file_name)

sum_res = 0
sum_time = 0
n = 0
for line in file:
    line = line.split(' ')
    sum_res += float(line[0])
    sum_time += float(line[1])
    n += 1

result = sum_res / n
time = sum_time / n
print(result)
if exp_result != -1:
    percentage = (exp_result * 100) / result
    print(abs(100 - percentage))

print(time)
