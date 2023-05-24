file_name = input()
exp_result = float(input())

file = open("results/" + file_name)

sum = 0
n = 0
for line in file:
    sum += float(line)
    n += 1

result = sum / n
print(result)

if exp_result != -1:
    percentage = (exp_result * 100) / result
    print(abs(100 - percentage))