python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.21.2 N-5-2.pcap plotmeTP-flow/TP-A1.plotme plotmeTPP-flow/TP-A1.plotme A1
python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.25.2 N-6-2.pcap plotmeTP-flow/TP-B1.plotme plotmeTPP-flow/TP-B1.plotme B1
python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.22.2 N-5-3.pcap plotmeTP-flow/TP-C1.plotme plotmeTPP-flow/TP-C1.plotme C1
python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.9.2 N-1-2.pcap plotmeTP-flow/TP-D1.plotme plotmeTPP-flow/TP-D1.plotme D1
python tp-new.py --startTime=2 --destPort 50000 --destIp 10.0.13.2 N-2-2.pcap plotmeTP-flow/TP-E1.plotme plotmeTPP-flow/TP-E1.plotme E1

rm -rf plotmeTPP-flow
cp perflowgnuplot plotmeTP-flow
cd plotmeTP-flow
gnuplot perflowgnuplot
mkdir -p ../../Graphs 
cp ProportionalFairshare-*.png ../../Graphs/
