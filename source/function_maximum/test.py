file_name = input()
file = open("results/" + file_name)

sum = 0
n = 0
for line in file:
    sum += float(line)
    n += 1

print(sum / n)