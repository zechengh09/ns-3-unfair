#! /usr/bin/env python3
"""Runs experiments to generate training data. """

import argparse
import json
import logging
import logging.handlers
import multiprocessing
import os
from os import path
import re
import subprocess
import time
import traceback

import matplotlib.pyplot as plt
import numpy as np


# Bandwidth (Mbps).
BWS_Mbps = 10
# Link delay (us).
DELAYS_us = 20_000
# Ack period (us).
# Router queue size (packets).
QUEUE = 1000
# Simulation duration (s).
DUR_s = 300
# Delay until ACK pacing begins.
WARMUP_s = [100, DUR_s]

#  MODEL = path.expanduser("~/models/subzero_error.pth")
#  SCALE_PARAMS = path.expanduser("~/models/subzero_error.csv")
MODEL = path.expanduser("/data/out/1588127399/net.pth")
SCALE_PARAMS = path.expanduser("/data/out/1588127399/scale_params.csv")
RECALC_us = 1000000
NUM_OTHER_FLOWS = list(range(1, 33))
USE_RENO = [True, False]
# Whether to return before running experiments.
DRY_RUN = False
# Whether to capture pcap traces.
PCAP = False
# Whether to run the simulations synchronously or in parallel.
SYNC = False
# Path to the ns-3 top-level directory.
# Warning: If you move this file from the ns-3 top-level directory, then you
#          must update this variable.
NS3_DIR = path.dirname(path.realpath(__file__))
# Used to synchronize writing to the error log.
LOCK = multiprocessing.Lock()
# Name of the error log file.
ERR_FLN = "failed.json"
# Default destination for email updates.
EMAIL_DST = "c.canel@icloud.com"


def check_output(cnf):
    args = ([path.join(NS3_DIR, "build", "scratch", "ai-multi-flow"),] +
            [f"--{arg}={val}" for arg, val in cnf.items()])
    cmd = f"LD_LIBRARY_PATH={os.environ['LD_LIBRARY_PATH']} {' '.join(args)}"
    print(f"Running: {cmd}")
    try:
        res = subprocess.check_output(
            args, stderr=subprocess.STDOUT, env=os.environ).decode().split("\n")
    except subprocess.CalledProcessError as e:
        print(e.stdout)
        print(e.stderr)
        traceback.print_exc()
        logging.getLogger().exception("Exception while running:\n%s\n\n", cmd)
        # Only one worker should access the error log at a time.
        LOCK.acquire()
        err_flp = path.join(cnf["out_dir"], ERR_FLN)
        # If the error log already exists, then read the existing log and append
        # to it. Otherwise, start a new log.
        if path.exists(err_flp):
            with open(err_flp, "r") as fil:
                err_log = json.load(fil)
        else:
            err_log = []
        # Record the full command (including LD_LIBRARY_PATH) in the error log
        # for ease of debugging. Do not do "cnf['cmd'] = ..." to maintain the
        # invariant that cnf contains only command line arguments.
        err_log.append(dict(cnf, cmd=cmd))
        with open(err_flp, "w") as fil:
            json.dump(err_log, fil, indent=4)
        LOCK.release()
        # The output is empty.
        res = []
    return res


def run(cnf):
    # Build the arguments array, run the simulation, and iterate over each line
    # in its output.
    for line in check_output(cnf):
        if line.startswith("Fairness"):
            match = re.search(r"Fairness: ([\d.]+)", line)
            assert match is not None, f"Improperly formed output line: {line}"
            return float(match.group(1))
    return None


def main():
    print(f"ns-3 dir: {NS3_DIR}")
    # Parse command line arguments.
    psr = argparse.ArgumentParser(description="Generates training data.")
    psr.add_argument(
        "--out-dir",
        help="The directory in which to store output files (required).",
        required=True, type=str)
    psr.add_argument(
        "--log-dst", default=EMAIL_DST,
        help="The email address to which updates will be sent.", type=str)
    args = psr.parse_args()
    # The ID of this experiment.
    eid = str(round(time.time()))
    # Create a new output directory based on the current time.
    out_dir = path.join(args.out_dir, eid)
    # For organization purposes, store the pcap files in a subdirectory.
    sim_dir = path.join(out_dir, "sim")
    # This also creates out_dir.
    os.makedirs(sim_dir)
    # Create email logger.
    # TODO: Create an email logger only if an SMTP server is running on the
    #       local machine.
    log = logging.getLogger()
    '''
    log.addHandler(logging.handlers.SMTPHandler(
        mailhost="localhost",
        fromaddr=f"{os.getlogin()}@maas.cmcl.cs.cmu.edu",
        toaddrs=args.log_dst,
        subject=f"[{path.basename(__file__)}] {eid}"))
    '''
    # Compile ns-3.
    subprocess.check_call([path.join(NS3_DIR, "waf")])
    # Since we are running the application binary directly, we need to make sure
    # the ns-3 library can be found.
    os.environ["LD_LIBRARY_PATH"] = (
        f"/usr/lib/gcc/x86_64-linux-gnu/7:{path.join(NS3_DIR, 'build')}:{'/opt/libtorch/lib'}")

    # Assemble the configurations.
    cnfs = [{
            "bandwidth_Mbps": BWS_Mbps, 
            "delay_us": DELAYS_us, 
            "queue_capacity_p": QUEUE, 
            "experiment_duration_s": DUR_s,
            "warmup_s": warm,
            "pcap": "true" if PCAP else "false",
            "out_dir": sim_dir,
            "num_other_flows": num,
            "model": MODEL,
            "scale_params_file": SCALE_PARAMS,
            "recalc_us": RECALC_us,
            "use_reno": "true" if reno else "false",
        } for num in NUM_OTHER_FLOWS
          for warm in WARMUP_s
          for reno in USE_RENO]
    #  } for num, warm in itertools.produc(NUM_OTHER_FLOW, WARMUP_s)

    # Record the configurations.
    with open(path.join(out_dir, "configurations.json"), "w") as fil:
        json.dump(cnfs, fil, indent=4)

    # Simulate the configurations.
    print(f"Num simulations: {len(cnfs)}")
    if DRY_RUN:
        return
    tim_srt_s = time.time()
    if SYNC:
        data = [run(cnf) for cnf in cnfs]
    else:
        with multiprocessing.Pool() as pool:
            data = pool.map(run, cnfs)
    print(f"Done with simulation - time: {time.time() - tim_srt_s:.2f} seconds")
    data = list(zip(cnfs, data))
    
    with open("data.json", "w") as f:
        json.dump(data, f)

    for reno in USE_RENO: 
        _, ax = plt.subplots()
        str_reno = "true" if reno else "false"
        for warm in WARMUP_s:
            selected = [(k['num_other_flows'], v) for k, v in data if k['warmup_s'] == warm and k['use_reno'] == str_reno]
            selected.sort(key=lambda x: x[0])
            xs, ys = zip(*selected)
            ax.plot(list(xs), list(ys), label=("With" if warm < DUR_s else "Without") + " ACK pacing")
        flow = 'Reno' if reno else 'Cubic'
        ax.set_xlabel(f"Number of {flow} Flows")
        ax.set_ylabel("Jain's Fairness Index")
        ax.legend()
    plt.show()
    #  plt.savefig(f"{flow}.pdf")
    '''
    # Graphs results. Generate an achieved throughput vs. ack period graph for
    # each configuration of bandwidth and delay.
    #
    # Returns a sorted list of all unique values for a particular key.
    get_all = lambda key: sorted(list({cnf[key] for cnf in cnfs}))
    # Use get_all() instead of directly looping over BWS_Mbps and DELAYS_us in
    # case we filtered the configurations and did not run all experiments.
    for bw_Mbps in get_all("bandwidth_Mbps"):
        for dly_us in get_all("delay_us"):
            # Select only the results with this bandwidth and delay, and pick
            # the value of the ACK period as the x value. Then, split the (x, y)
            # pairs into a list of xs and a list of ys.
            xs, ys = zip(*[(cnf["ack_period_us"], results)
                           for cnf, results in data
                           if (cnf["bandwidth_Mbps"] == bw_Mbps and
                               cnf["delay_us"] == dly_us)])
            # Convert from us to ms.
            xs = np.array(xs) / 1e3
            plt.plot(xs, ys)
            plt.xlabel("ACK delay (ms)")
            plt.ylabel("Average throughput (Mb/s)")
            plt.ylim(bottom=0, top=bw_Mbps)
            plt.savefig(path.join(
                out_dir,
                f"{bw_Mbps}Mbps_{dly_us}us_{QUEUE}packets_{DUR_s}s.pdf"))
            plt.close()

    print(f"Results in: {out_dir}")
    log.critical("Finished.")
    '''


if __name__ == "__main__":
    main()
