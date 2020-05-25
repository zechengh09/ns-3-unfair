import argparse
import json
import multiprocessing
import os
from os import path
import time
from collections import deque

from matplotlib import pyplot as plt
import numpy as np

# Whether to plot per-experiment average throughput.
DO_PLT = False
# The number of link segments on each round trip path.
LINKS_PER_RTT = 4
# Packet size in bytes
PACKET_SIZE = 1380
# RTT window to consider receiving rate and loss rate
RTT_WINDOW = 2

def failed(fln, fals):
    # TODO: This does not work.
    for cnf in fals:
        if (f"{cnf['bandwidth_Mbps']}Mbps-"
            f"{cnf['delay_us'] * 4}us-"
            f"{cnf['experiment_duration_s']}s"
            f"{cnf['queue_capacity_p']}p"
            f"{cnf['other_flows']}") in fln:
            print(f"Discarding: {fln}")
            return True
    return False

def parse_csv(flp, out_dir, do_plt=True):
    # Parse a csv file
    print(f"Parsing: {flp}")
    bw_Mbps, rtt_us, queue_p, dur_s, num_flows = path.basename(flp)[:-4].split("-")
    # Link bandwidth (Mbps).
    bw_Mbps = float(bw_Mbps[:-4])
    # RTT (us).
    rtt_us = float(rtt_us[:-2])
    # Queue size (packets).
    queue_p = float(queue_p[:-1])
    # Experiment duration (s).
    dur_s = float(dur_s[:-1])
    # Total number of flows
    num_flows = float(num_flows)

    one_way_ms = rtt_us / 1000 / 2.0
    rtt_ms = rtt_us / 1000

    # There could be better ways using pandas to get inter-arrival time and loss rate 
    packet_queue = deque()
    loss_queue = deque()

    # Load csv file to ndarray
    data = np.genfromtxt(flp, dtype=float, delimiter=",", names=True)

    output = np.empty([len(data), 3], dtype=float) # RTT ratio, packet arrival rate, loss rate
    for i in range(len(data)):

        seq = data[i][0]
        recv_time = data[i][1]
        sent_time = data[i][2]

        # RTT ratio
        output[i][0] = (recv_time - sent_time) / one_way_ms

        # Pop out earlier packets
        while (len(packet_queue) > 0 and 
               packet_queue[0] < recv_time - (rtt_ms * RTT_WINDOW)):
            packet_queue.popleft()

        # Inter-arrival time
        if (len(packet_queue) > 0):
            output[i][1] = (recv_time - packet_queue[0]) / len(packet_queue)
        else:
            output[i][1] = 0

        packet_queue.append(recv_time)

        # Pop out earlier loss
        while(len(loss_queue) > 0 and
              loss_queue[0] < recv_time - (rtt_ms * RTT_WINDOW)):
            loss_queue.popleft()

        if (i > 0 and seq - data[i-1][0] > PACKET_SIZE):
            # Loss
            prev_seq = data[i-1][0]
            prev_recv_time = data[i-1][1]
            loss_count = int((seq - prev_seq) / PACKET_SIZE) - 1
            loss_interval = (recv_time - prev_recv_time) / (loss_count + 1)
            for i in range(1, loss_count + 1):
                loss_queue.append(prev_recv_time + (loss_interval * i))

        # Loss rate
        output[i][2] = len(loss_queue) / float(len(loss_queue) + len(packet_queue))

    # Write the array to output file
    print("Saving " + out_dir + "/" + path.basename(flp)[:-4] + ".npz")
    np.savez_compressed(out_dir + "/" + path.basename(flp)[:-4] + ".npz", output)

def main():
    # Parse command line arguments.
    psr = argparse.ArgumentParser(
        description="Parses the output of gen_training_data.py.")
    psr.add_argument(
        "--exp-dir",
        help=("The directory in which the experiment results are stored "
              "(required)."),
        required=True, type=str)
    psr.add_argument(
        "--out-dir", help=("The directory in which to store output files "
                           "(required)."),
        required=True, type=str)
    args = psr.parse_args()
    exp_dir = args.exp_dir
    out_dir = args.out_dir

    # Determine which configurations failed.
    fals = []
    fals_flp = path.join(exp_dir, "failed.json")
    if path.exists(fals_flp):
        with open(fals_flp, "r") as fil:
            fals = json.load(fil)

    csvs = [(path.join(exp_dir, fln), out_dir, DO_PLT)
             for fln in os.listdir(exp_dir)
             if not failed(fln, fals))]
    print(f"Num files: {len(csvs)}")

    tim_srt_s = time.time()
    with multiprocessing.Pool() as pol:
        pol.starmap(parse_csv, csvs)
    print(f"Done parsing - time: {time.time() - tim_srt_s:.2f} seconds")

