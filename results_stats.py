import datetime
import statistics

with open('results.txt') as f:
    results = [each[13:21] for each in f.readlines()]

deltas = [(datetime.datetime.strptime(t, '%H:%M:%S') - datetime.datetime.strptime(s, '%H:%M:%S')).seconds for s, t in zip(results, results[1:])]

print("Time per request:")
print("    Mean: {:.2f}".format(statistics.mean(deltas)))
print("    Median: {}".format(statistics.median(deltas)))
print("    Stdev: {:.2f}".format(statistics.stdev(deltas)))
