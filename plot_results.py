import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("performance_report.csv")

# Combine algorithm + cores for labels
df["label"] = df["algorithm"] + " (" + df["cores"].astype(str) + " core)"

# ---- Average Waiting Time ----
plt.figure()
plt.bar(df["label"], df["avg_waiting_time"])
plt.ylabel("Average Waiting Time")
plt.title("Average Waiting Time Comparison")
plt.xticks(rotation=20)
plt.tight_layout()
plt.savefig("avg_waiting_time.png")
plt.show()

# ---- Average Turnaround Time ----
plt.figure()
plt.bar(df["label"], df["avg_turnaround_time"])
plt.ylabel("Average Turnaround Time")
plt.title("Average Turnaround Time Comparison")
plt.xticks(rotation=20)
plt.tight_layout()
plt.savefig("avg_turnaround_time.png")
plt.show()

# ---- CPU Utilization ----
plt.figure()
plt.bar(df["label"], df["cpu_utilization"])
plt.ylabel("CPU Utilization (%)")
plt.title("CPU Utilization Comparison")
plt.xticks(rotation=20)
plt.tight_layout()
plt.savefig("cpu_utilization.png")
plt.show()

# ---- Throughput ----
plt.figure()
plt.bar(df["label"], df["throughput"])
plt.ylabel("Throughput (processes/unit time)")
plt.title("Throughput Comparison")
plt.xticks(rotation=20)
plt.tight_layout()
plt.savefig("throughput.png")
plt.show()

print("Plots generated:")
print("- avg_waiting_time.png")
print("- avg_turnaround_time.png")
print("- cpu_utilization.png")
print("- throughput.png")
