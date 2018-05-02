#Usage startTime
printf "Flow Throughput\n\n" | tee output.file

python tp-new.py --startTime=2 --destIp 10.0.21.2 N-22-0.pcap plotmeTP/TP-A.plotme plotmeTPP/TP-A.plotme A | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.25.2 N-26-0.pcap plotmeTP/TP-B.plotme plotmeTPP/TP-B.plotme B | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.22.2 N-23-0.pcap plotmeTP/TP-C.plotme plotmeTPP/TP-C.plotme C | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.9.2 N-10-0.pcap plotmeTP/TP-D.plotme plotmeTPP/TP-D.plotme D  | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.13.2 N-14-0.pcap plotmeTP/TP-E.plotme plotmeTPP/TP-E.plotme E | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.16.2 N-17-0.pcap plotmeTP/TP-F.plotme plotmeTPP/TP-F.plotme F | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.24.2 N-25-0.pcap plotmeTP/TP-G.plotme plotmeTPP/TP-G.plotme G | tee output.file
python tp-new.py --startTime=2 --destIp 10.0.19.2 N-20-0.pcap plotmeTP/TP-H.plotme plotmeTPP/TP-H.plotme H | tee output.file


cp gnuplotscript plotmeTP
cd plotmeTP
gnuplot gnuplotscript
cd ..

cp gnuplotscriptPackets plotmeTPP
cd plotmeTPP
gnuplot gnuplotscriptPackets
cd ..

printf "\nRouter Throughput\n\n" | tee output.file

python tp-new.py --startTime=2 N-0-0.pcap plotmeRouterTP/TP-R1.plotme plotmeRouterTPP/TP-R1.plotme R1 | tee output.file
python tp-new.py --startTime=2 N-1-1.pcap plotmeRouterTP/TP-R2.plotme plotmeRouterTPP/TP-R2.plotme R2 | tee output.file
python tp-new.py --startTime=2 N-2-1.pcap plotmeRouterTP/TP-R3.plotme plotmeRouterTPP/TP-R3.plotme R3 | tee output.file
python tp-new.py --startTime=2 N-3-1.pcap plotmeRouterTP/TP-R4.plotme plotmeRouterTPP/TP-R4.plotme R4 | tee output.file
python tp-new.py --startTime=2 N-4-1.pcap plotmeRouterTP/TP-R5.plotme plotmeRouterTPP/TP-R5.plotme R5 | tee output.file
python tp-new.py --startTime=2 N-5-1.pcap plotmeRouterTP/TP-R6.plotme plotmeRouterTPP/TP-R6.plotme R6 | tee output.file

cp gnuplotscriptRouters plotmeRouterTP
cd plotmeRouterTP
gnuplot gnuplotscriptRouters
cd ..

cp gnuplotscriptRoutersPackets plotmeRouterTPP
cd plotmeRouterTPP
gnuplot gnuplotscriptRoutersPackets
cd ..

mkdir -p ../Graphs
cp plotmeRouterTP/TP.png ../Graphs/TP-Router.png
cp plotmeRouterTP/TPPercent.png ../Graphs/TP-Router-Percent.png
cp plotmeRouterTPP/TP.png ../Graphs/TP-Router-Packets.png
cp plotmeTP/TP.png ../Graphs/TP-Flow.png
cp plotmeTP/TPPercent.png ../Graphs/TP-Flow-Percent.png
cp plotmeTPP/TP.png ../Graphs/TP-Flow-Packets.png

#Cwnd plot
cp gnuplotscriptQ ../
cp gnuplotscriptCwnd ../cwndTraces/

cd ..
gnuplot gnuplotscriptQ
cp queueSize.png Graphs/
cp queueStats.txt Graphs/

cd cwndTraces
gnuplot gnuplotscriptCwnd
cp Cwnd*.png ../Graphs/
