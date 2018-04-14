#Mbps
mkdir plotmeTP
printf "Flow Throughput (Mbps)\n"
python calculateTP_A.py N-22-0.pcap
python calculateTP_B.py N-26-0.pcap
python calculateTP_C.py N-23-0.pcap
python calculateTP_D.py N-10-0.pcap
python calculateTP_E.py N-14-0.pcap
python calculateTP_F.py N-17-0.pcap
python calculateTP_G.py N-25-0.pcap
python calculateTP_H.py N-20-0.pcap


cp gnuplotscript plotmeTP
cd plotmeTP
gnuplot gnuplotscript
cd ..

mkdir plotmeRouterTP
printf "\n\nRouter Throughput (Mbps)\n"
printf "R1 "
python calculateTP_R.py N-0-0.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R1.plotme

printf "R2 "
python calculateTP_R.py N-1-1.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R2.plotme

printf "R3 "
python calculateTP_R.py N-2-1.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R3.plotme

printf "R4 "
python calculateTP_R.py N-3-1.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R4.plotme

printf "R5 "
python calculateTP_R.py N-4-1.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R5.plotme

printf "R6 "
python calculateTP_R.py N-5-1.pcap
mv plotmeRouterTP/TP.plotme plotmeRouterTP/TP-R6.plotme

cp gnuplotscriptRouters plotmeRouterTP
cd plotmeRouterTP
gnuplot gnuplotscriptRouters
cd ..

mkdir plotmeTPP
printf "\n\nFlow Throughput (Packets/s)\n"
python calculateTPP_A.py N-22-0.pcap
python calculateTPP_B.py N-26-0.pcap
python calculateTPP_C.py N-23-0.pcap
python calculateTPP_D.py N-10-0.pcap
python calculateTPP_E.py N-14-0.pcap
python calculateTPP_F.py N-17-0.pcap
python calculateTPP_G.py N-25-0.pcap
python calculateTPP_H.py N-20-0.pcap


cp gnuplotscriptPackets plotmeTPP
cd plotmeTPP
gnuplot gnuplotscriptPackets
cd ..

#PACKETS/sec
mkdir plotmeRouterTPP
printf "\n\nRouter Throughput (Packets/s)\n"
printf "R1 "
python calculateTPP_R.py N-0-0.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R1.plotme

printf "R2 "
python calculateTPP_R.py N-1-1.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R2.plotme

printf "R3 "
python calculateTPP_R.py N-2-1.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R3.plotme

printf "R4 "
python calculateTPP_R.py N-3-1.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R4.plotme

printf "R5 "
python calculateTPP_R.py N-4-1.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R5.plotme

printf "R6 "
python calculateTPP_R.py N-5-1.pcap
mv plotmeRouterTPP/TP.plotme plotmeRouterTPP/TP-R6.plotme

cp gnuplotscriptRoutersPackets plotmeRouterTPP
cd plotmeRouterTPP
gnuplot gnuplotscriptRoutersPackets

#Cwnd plot
cd ..
cp gnuplotscriptQ ../
cp gnuplotscriptCwnd ../cwndTraces/

cd ..
gnuplot gnuplotscriptQ

cd cwndTraces
gnuplot gnuplotscriptCwnd
