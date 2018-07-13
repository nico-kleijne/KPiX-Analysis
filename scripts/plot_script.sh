#!/bin/bash
#calib_files=(/scratch/data/tracker_test/2018_03_21_19_05_27.bin /scratch/data/tracker_test/2018_03_22_16_39_25.bin /scratch/data/tracker_test/2018_03_23_10_26_45.bin /scratch/data/tracker_test/2018_03_28_15_55_53.bin /scratch/data/tracker_test/2018_03_29_10_33_24.bin /scratch/data/tracker_test/2018_03_29_13_03_39.bin /scratch/data/tracker_test/2018_04_03_12_54_16.bin /scratch/data/tracker_test/2018_04_30_14_29_29.bin  /scratch/data/tracker_test/2018_05_04_14_40_01.bin)
#calib_extension=.newCalib.root
#calib_files_normal=(/scratch/data/tracker_test/2018_06_01_15_44_24.bin /scratch/data/tracker_test/2018_06_04_17_22_01.bin)
#calib_files_high=(/scratch/data/tracker_test/2018_06_01_15_20_17.bin /scratch/data/tracker_test/2018_06_04_17_56_49.bin)
#for i in ${calib_files_high[*]}
#do
	#./bin/new_CalibrationFitter ../kpixDaq/kpix.master/xml/calibration_base_file_high_gain.xml $i
	#python python/plot_producer.py $i$calib_extension -n pedestal  -d 'hist'
	#python python/plot_producer.py $i$calib_extension -n slope  -d 'hist'
	#python python/plot_producer.py $i$calib_extension -n RMSfc  -d 'P'
#done

#for i in ${calib_files_normal[*]}
#do
	#./bin/new_CalibrationFitter ../kpixDaq/kpix.master/xml/calibration_base_file.xml $i
	#python python/plot_producer.py $i$calib_extension -n pedestal  -d 'hist'
	#python python/plot_producer.py $i$calib_extension -n slope  -d 'hist'
	#python python/plot_producer.py $i$calib_extension -n RMSfc  -d 'P'
#done
#pedestal_files=(/scratch/data/tracker_test/2018_04_24_17_37_33.bin /scratch/data/tracker_test/2018_03_28_14_21_24.bin /scratch/data/tracker_test/2018_03_28_16_18_31.bin /scratch/data/tracker_test/2018_03_29_13_21_35.bin /scratch/data/tracker_test/2018_03_29_16_24_51.bin /scratch/data/tracker_test/2018_04_03_12_50_41.bin /scratch/data/tracker_test/2018_04_24_18_14_43.bin /scratch/data/tracker_test/2018_05_02_17_13_38.bin /scratch/data/tracker_test/2018_05_02_17_17_16.bin)
#pedestal_files=(/scratch/data/tracker_test/2018_06_01_14_32_57.bin /scratch/data/tracker_test/2018_06_01_15_00_35.bin /scratch/data/tracker_test/2018_06_01_15_18_30.bin /scratch/data/tracker_test/2018_06_04_17_03_25.bin /scratch/data/tracker_test/2018_06_04_17_04_45.bin) #(/scratch/data/tracker_test/2018_03_28_16_13_28.bin /scratch/data/tracker_test/2018_03_28_16_18_31.bin)
pedestal_files=(/scratch/data/tracker_test/2018_06_05_16_55_46.bin /scratch/data/tracker_test/2018_06_05_16_57_05.bin /scratch/data/tracker_test/2018_06_05_16_58_18.bin /scratch/data/tracker_test/2018_06_05_16_59_45.bin /scratch/data/tracker_test/2018_06_05_17_01_28.bin /scratch/data/tracker_test/2018_06_05_17_02_53.bin)

analysis_extension=.root
for i in ${pedestal_files[*]}
do
	./bin/analysis $i
	#python python/plot_producer.py $i$analysis_extension -n hist -c 0821 0822 0823 0825 0830 0831 -b 0 -d 'same hist' -o 'block_detail_821to831_rebinned.png' -r 4 --yrange 0 0.2
	#python python/plot_producer.py $i$analysis_extension -n time -c 0821 0822 0823 0825 0830 0831 -b 0 -d 'same hist' -o 'block_detail_time_821to831_rebinned.png' -r 40 --ylog 1 --yrange 0.01 2
	#python python/plot_producer.py $i$analysis_extension -n time -c 0809 0810 0811 0813 0817 0819 0820 -b 0 -d 'same hist' -o 'block_detail_809to820_rebinned.png' -r 4 --yrange 0 0.2 # --color 416 600 400 616
	#python python/plot_producer.py $i$analysis_extension -n time -c 0809 0810 0811 0813 0817 0819 0820 -b 0 -d 'same hist' -o 'block_detail_time_809to820_rebinned.png' -r 40 --ylog 1 --yrange 0.01 2 # --color 416 600 400 616
	python python/plot_producer.py $i$analysis_extension -n Channel_entries_k_ -d 'hist e' -o 'block_zoom_550to650.png' -f no --xrange 550 650
	python python/plot_producer.py $i$analysis_extension -n timesta -d 'hist'
	python python/plot_producer.py $i$analysis_extension -n entr  -d 'hist'
done

#python python/plot_producer.py /scratch/data/tracker_test/2018_06_01_15_00_35.bin.root /scratch/data/tracker_test/2018_06_01_15_18_30.bin.root -n Channel_entries_k_ -d 'hist e same' -b 4 -o 'block_comparison_zoom.png' -f no --xrange 750 850 --legend all_enabled_2018_06_01_15_00_35 304_disabled_2018_06_01_15_18_30 --yrange 0 1.5
#python python/plot_producer.py /scratch/data/tracker_test/2018_06_04_17_04_45.bin.root /scratch/data/tracker_test/2018_06_04_17_03_25.bin.root -n Channel_entries_k_ -d 'hist e same' -b 4 -o 'block_comparison_zoom.png' -f no --xrange 750 850 --legend all_enabled_2018_06_04 304_disabled_2018_06_04 --yrange 0 1.5

#python python/plot_producer.py /scratch/data/tracker_test/2018_06_05_16_55_46.bin /scratch/data/tracker_test/2018_06_05_16_57_05.bin /scratch/data/tracker_test/2018_06_05_16_58_18.bin /scratch/data/tracker_test/2018_06_05_16_59_45.bin /scratch/data/tracker_test/2018_06_05_17_01_28.bin /scratch/data/tracker_test/2018_06_05_17_02_53.bin -n Channel_entries_k_ -d 'hist e same' -b 4 -o 'block_comparison.png' -f no 

#python python/plot_producer.py  /scratch/data/tracker_test/2018_06_04_17_22_01.bin$calib_extension /scratch/data/tracker_test/2018_06_04_17_56_49.bin$calib_extension -n RMSfc  -d 'P same'
#python python/plot_producer.py /scratch/data/tracker_test/2018_06_04_17_03_25.bin.root /scratch/data/tracker_test/2018_06_04_17_04_45.bin.root /scratch/data/tracker_test/2018_06_05_16_05_38.bin.root /scratch/data/tracker_test/2018_06_05_16_13_58.bin.root /scratch/data/tracker_test/2018_06_05_16_26_07.bin.root /scratch/data/tracker_test/2018_06_05_16_39_15.bin.root /scratch/data/tracker_test/2018_06_05_16_54_03.bin.root -n Channel_entries_k_ -d 'hist same' -o 'board_comparison.png' -f no --yrange 0 4


#source_files=(/scratch/data/tracker_test/2018_05_07_16_49_42.bin) #/scratch/data/tracker_test/2018_04_19_16_28_33.bin /scratch/data/tracker_test/2018_04_19_18_01_13.bin /scratch/data/tracker_test/2018_04_30_16_42_27.bin)
#for i in ${source_files[*]}
#do
	#./bin/analysis $i
	#python python/plot_producer.py $i$analysis_extension -n entr  -d 'hist'
	#python python/plot_producer.py $i$analysis_extension -n timesta -d 'hist'
#done



