import matplotlib.pyplot as plt
import csv
import os

os.makedirs("results/charts", exist_ok = True)
methods = []
buildTimes = []
memUsages = []
queryTimes = []

with open("results/benchmark_results.csv") as file:
	readList = csv.DictReader(file)
	for row in readList:
		methods.append(row["method"])
		buildTimes.append(float(row["buildmsTimes"]))
		memUsages.append(float(row["memorySize"]))
		queryTimes.append(float(row["avgQueryTimes"]))


print("methods:", methods)
print("buildTimes:", buildTimes)
print("memUsages:", memUsages)
print("queryTimes:", queryTimes)


# time chart
fig, ax = plt.subplots()
ax.bar(methods, buildTimes, width = 0.4)
ax.set_title("Build Time")
ax.set_ylabel("Milliseconds")
fig.savefig("results/charts/buildTimes.png")
plt.close(fig)

# memory chart
fig, ax = plt.subplots()
ax.bar(methods, memUsages, width = 0.4)
ax.set_title("Memory Usage")
ax.set_ylabel("Kilobytes")
fig.savefig("results/charts/memUsages.png")
plt.close(fig)

# query chart
fig, ax = plt.subplots()
ax.bar(methods, queryTimes, width = 0.4)
ax.set_title("Average Query Time")
ax.set_ylabel("Microseconds")
fig.savefig("results/charts/queryTimes.png")
plt.close(fig)
