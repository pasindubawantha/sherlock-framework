import json
import pandas
import math
import sys
import os
import numpy as np
import re

# args start
output_directory = "../data/nab_tuned_results"
input_directory = "../data/nab_tuned_results_back"

threshold_training_ratio = 0.045
threshold_max_multipler = 3
# args end

reg_x = re.compile(r'\.(csv)')
csv_input_files = []
for path, dnames, fnames in os.walk(input_directory):
    csv_input_files.extend([os.path.join(path, f) for f in fnames if reg_x.search(f)])
csv_input_files.sort()

for f in csv_input_files:
    # f = '../data/nab_tuned_results_back/ambient_temperature_system_failure.csv'
    # fout = '../data/nab_tuned_results/ambient_temperature_system_failure.csv'

    dataframe = pandas.read_csv(f)

    timestamp = np.array(dataframe['timestamp'])
    value = np.array(dataframe['value'])
    prediction_training = np.array(dataframe['prediction_training'])
    prediction = np.array(dataframe['prediction'])
    label = np.array(dataframe['label'])
    warp_distance = np.array(dataframe['warp_distance'])
    threshold_training = np.zeros(len(timestamp))
    distance_threshold = np.ones(len(timestamp))*(-1)
    positive_detection = np.zeros(len(timestamp))

    prediction_training_count = 0
    for i in prediction_training:
        prediction_training_count += i
        if i != 1:
            break

    threshold_training_count = int(len(threshold_training)*threshold_training_ratio)
    threshold_max = -1
    for i in range(prediction_training_count, prediction_training_count+threshold_training_count):
        threshold_training[i] = 1
        if threshold_max < warp_distance[i]:
            threshold_max = warp_distance[i]

    threshold = threshold_max_multipler*threshold_max

    for i in range(prediction_training_count-1+threshold_training_count, len(positive_detection)):
        distance_threshold[i] = threshold
        if warp_distance[i] >= threshold:
            positive_detection[i] = 1


    newData = {'value':value,
        'prediction_training':prediction_training,
        'prediction':prediction,
        'label':label,
        'warp_distance':warp_distance,
        'threshold_training':threshold_training,
        'distance_threshold':distance_threshold,
        'positive_detection':positive_detection}
    newDataFrame = pandas.DataFrame(newData, index=timestamp) 
    newDataFrame.index.name = "timestamp"
    newDataFrame = newDataFrame[['value','prediction_training','prediction','label','warp_distance', 'threshold_training', 'distance_threshold', 'positive_detection']]
    fout = output_directory + '/' + f.split('/')[-1]
    print(fout)
    newDataFrame.to_csv(fout)