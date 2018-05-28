#Usage startTime
printf "Flow Throughput\n\n" | tee -a overallTP.txt

python tp-new.py --startTime=2 --destIp 10.0.11.2 N-12-0.pcap plotmeTP/TP-A.plotme plotmeTPP/TP-A.plotme A | tee -a overallTP.txt
python tp-new.py --startTime=2 --destIp 10.0.15.2 N-16-0.pcap plotmeTP/TP-B.plotme plotmeTPP/TP-B.plotme B | tee -a overallTP.txt
python tp-new.py --startTime=2 --destIp 10.0.12.2 N-13-0.pcap plotmeTP/TP-C.plotme plotmeTPP/TP-C.plotme C | tee -a overallTP.txt
python tp-new.py --startTime=2 --destIp 10.0.6.2 N-7-0.pcap plotmeTP/TP-D.plotme plotmeTPP/TP-D.plotme D | tee -a overallTP.txt
python tp-new.py --startTime=2 --destIp 10.0.14.2 N-15-0.pcap plotmeTP/TP-E.plotme plotmeTPP/TP-E.plotme E | tee -a overallTP.txt
python tp-new.py --startTime=2 --destIp 10.0.9.2 N-10-0.pcap plotmeTP/TP-F.plotme plotmeTPP/TP-F.plotme F | tee -a overallTP.txt

cp gnuplotscript plotmeTP
cd plotmeTP
gnuplot gnuplotscript
cd ..

cp gnuplotscriptPackets plotmeTPP
cd plotmeTPP
gnuplot gnuplotscriptPackets
cd ..

printf "\nRouter Throughput\n\n" | tee -a overallTP.txt

python tp-new.py --startTime=2 N-0-0.pcap plotmeRouterTP/TP-R1.plotme plotmeRouterTPP/TP-R1.plotme R1 | tee -a overallTP.txt
python tp-new.py --startTime=2 N-1-1.pcap plotmeRouterTP/TP-R2.plotme plotmeRouterTPP/TP-R2.plotme R2 | tee -a overallTP.txt
python tp-new.py --startTime=2 N-2-1.pcap plotmeRouterTP/TP-R3.plotme plotmeRouterTPP/TP-R3.plotme R3 | tee -a overallTP.txt
python tp-new.py --startTime=2 N-3-1.pcap plotmeRouterTP/TP-R4.plotme plotmeRouterTPP/TP-R4.plotme R4 | tee -a overallTP.txt

cp gnuplotscriptRouters plotmeRouterTP
cd plotmeRouterTP
gnuplot gnuplotscriptRouters
cd ..

cp gnuplotscriptRoutersPackets plotmeRouterTPP
cd plotmeRouterTPP
gnuplot gnuplotscriptRoutersPackets
cd ..

mkdir -p ../Graphs
cp plotmeRouterTP/TP-Router.png ../Graphs/TP-Router.png
cp plotmeRouterTP/TP-Router-Percent.png ../Graphs/TP-Router-Percent.png
cp plotmeRouterTPP/TP-Router-Packets.png ../Graphs/TP-Router-Packets.png
cp plotmeTP/TP-Flow.png ../Graphs/TP-Flow.png
cp plotmeTP/TP-Flow-Percent.png ../Graphs/TP-Flow-Percent.png
cp plotmeTPP/TP-Flow-Packets.png ../Graphs/TP-Flow-Packets.png
cp overallTP.txt ../Graphs/

#Cwnd plot
cp gnuplotscriptQ ../
cp gnuplotscriptCwnd ../cwndTraces/

cd ../queueTraces/
echo "Queue 1" > QueueStatsAfter2sec.txt
drops=`awk '{if ($1 >= 2) print $0}' drop-0.plotme | wc -l`
marks=`awk '{if ($1 >= 2) print $0}' mark-0.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt

echo "Queue 2" >> QueueStatsAfter2sec.txt
drops=`awk '{if ($1 >= 2) print $0}' drop-1.plotme | wc -l`
marks=`awk '{if ($1 >= 2) print $0}' mark-1.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt

echo "Queue 3" >> QueueStatsAfter2sec.txt
drops=`awk '{if ($1 >= 2) print $0}' drop-2.plotme | wc -l`
marks=`awk '{if ($1 >= 2) print $0}' mark-2.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt

echo "Queue 4" >> QueueStatsAfter2sec.txt
drops=`awk '{if ($1 >= 2) print $0}' drop-3.plotme | wc -l`
marks=`awk '{if ($1 >= 2) print $0}' mark-3.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt

cp QueueStatsAfter2sec.txt ../Graphs/

cd ..
gnuplot gnuplotscriptQ
cp queueSize.png Graphs/
cp queueStats.txt Graphs/

cd cwndTraces
gnuplot gnuplotscriptCwnd
cp Cwnd*.png ../Graphs/
