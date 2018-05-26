python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.21.2 N-5-2.pcap plotmeTP-flow/TP-A1.plotme plotmeTPP-flow/TP-A1.plotme A1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.21.2 N-5-2.pcap plotmeTP-flow/TP-A2.plotme plotmeTPP-flow/TP-A2.plotme A2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.21.2 N-5-2.pcap plotmeTP-flow/TP-A3.plotme plotmeTPP-flow/TP-A3.plotme A3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.25.2 N-6-2.pcap plotmeTP-flow/TP-B1.plotme plotmeTPP-flow/TP-B1.plotme B1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.25.2 N-6-2.pcap plotmeTP-flow/TP-B2.plotme plotmeTPP-flow/TP-B2.plotme B2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.25.2 N-6-2.pcap plotmeTP-flow/TP-B3.plotme plotmeTPP-flow/TP-B3.plotme B3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.22.2 N-5-3.pcap plotmeTP-flow/TP-C1.plotme plotmeTPP-flow/TP-C1.plotme C1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.22.2 N-5-3.pcap plotmeTP-flow/TP-C2.plotme plotmeTPP-flow/TP-C2.plotme C2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.22.2 N-5-3.pcap plotmeTP-flow/TP-C3.plotme plotmeTPP-flow/TP-C3.plotme C3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.9.2 N-1-2.pcap plotmeTP-flow/TP-D1.plotme plotmeTPP-flow/TP-D1.plotme D1

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.13.2 N-2-2.pcap plotmeTP-flow/TP-E1.plotme plotmeTPP-flow/TP-E1.plotme E1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.13.2 N-2-2.pcap plotmeTP-flow/TP-E2.plotme plotmeTPP-flow/TP-E2.plotme E2

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.16.2 N-3-2.pcap plotmeTP-flow/TP-F1.plotme plotmeTPP-flow/TP-F1.plotme F1

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G1.plotme plotmeTPP-flow/TP-G1.plotme G1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G2.plotme plotmeTPP-flow/TP-G2.plotme G2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G3.plotme plotmeTPP-flow/TP-G3.plotme G3
python tp-new.py --startTime=2 --destPort 50003 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G4.plotme plotmeTPP-flow/TP-G4.plotme G4
python tp-new.py --startTime=2 --destPort 50004 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G5.plotme plotmeTPP-flow/TP-G5.plotme G5
python tp-new.py --startTime=2 --destPort 50005 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G6.plotme plotmeTPP-flow/TP-G6.plotme G6
python tp-new.py --startTime=2 --destPort 50006 --destIp 10.0.24.2 N-6-1.pcap plotmeTP-flow/TP-G7.plotme plotmeTPP-flow/TP-G7.plotme G7

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.19.2 N-4-2.pcap plotmeTP-flow/TP-H1.plotme plotmeTPP-flow/TP-H1.plotme H1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.19.2 N-4-2.pcap plotmeTP-flow/TP-H2.plotme plotmeTPP-flow/TP-H2.plotme H2

rm -rf plotmeTPP-flow
cp perflowgnuplot plotmeTP-flow
cd plotmeTP-flow
gnuplot perflowgnuplot
mkdir -p ../../Graphs 
cp ProportionalFairshare-*.png ../../Graphs/
