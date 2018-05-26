python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.11.2 N-3-2.pcap plotmeTP-flow/TP-A1.plotme plotmeTPP-flow/TP-A1.plotme A1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.11.2 N-3-2.pcap plotmeTP-flow/TP-A2.plotme plotmeTPP-flow/TP-A2.plotme A2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.11.2 N-3-2.pcap plotmeTP-flow/TP-A3.plotme plotmeTPP-flow/TP-A3.plotme A3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.15.2 N-4-2.pcap plotmeTP-flow/TP-B1.plotme plotmeTPP-flow/TP-B1.plotme B1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.15.2 N-4-2.pcap plotmeTP-flow/TP-B2.plotme plotmeTPP-flow/TP-B2.plotme B2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.15.2 N-4-2.pcap plotmeTP-flow/TP-B3.plotme plotmeTPP-flow/TP-B3.plotme B3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.12.2 N-3-3.pcap plotmeTP-flow/TP-C1.plotme plotmeTPP-flow/TP-C1.plotme C1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.12.2 N-3-3.pcap plotmeTP-flow/TP-C2.plotme plotmeTPP-flow/TP-C2.plotme C2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.12.2 N-3-3.pcap plotmeTP-flow/TP-C3.plotme plotmeTPP-flow/TP-C3.plotme C3

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D1.plotme plotmeTPP-flow/TP-D1.plotme D1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D2.plotme plotmeTPP-flow/TP-D2.plotme D2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D3.plotme plotmeTPP-flow/TP-D3.plotme D3
python tp-new.py --startTime=2 --destPort 50003 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D4.plotme plotmeTPP-flow/TP-D4.plotme D4
python tp-new.py --startTime=2 --destPort 50004 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D5.plotme plotmeTPP-flow/TP-D5.plotme D5
python tp-new.py --startTime=2 --destPort 50005 --destIp 10.0.6.2 N-1-2.pcap plotmeTP-flow/TP-D6.plotme plotmeTPP-flow/TP-D6.plotme D6

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E1.plotme plotmeTPP-flow/TP-E1.plotme E1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E2.plotme plotmeTPP-flow/TP-E2.plotme E2
python tp-new.py --startTime=2 --destPort 50002 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E3.plotme plotmeTPP-flow/TP-E3.plotme E3
python tp-new.py --startTime=2 --destPort 50003 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E4.plotme plotmeTPP-flow/TP-E4.plotme E4
python tp-new.py --startTime=2 --destPort 50004 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E5.plotme plotmeTPP-flow/TP-E5.plotme E5
python tp-new.py --startTime=2 --destPort 50005 --destIp 10.0.14.2 N-4-1.pcap plotmeTP-flow/TP-E6.plotme plotmeTPP-flow/TP-E6.plotme E6

python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.9.2 N-2-2.pcap plotmeTP-flow/TP-F1.plotme plotmeTPP-flow/TP-F1.plotme F1
python tp-new.py --startTime=2 --destPort 50001 --destIp 10.0.9.2 N-2-2.pcap plotmeTP-flow/TP-F2.plotme plotmeTPP-flow/TP-F2.plotme F2


rm -rf plotmeTPP-flow
cp perflowgnuplot plotmeTP-flow
cd plotmeTP-flow
gnuplot perflowgnuplot
mkdir -p ../../Graphs 
cp ProportionalFairshare-*.png ../../Graphs/
