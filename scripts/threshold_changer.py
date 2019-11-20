import json
import pandas
import math
import sys
import os
import numpy as np
import re
import helpers
import confusion_metrics

# args start
output_directory = "../data/nab_tuned_results_CDD_LSTM"
input_directory = "../data/nab_tuned_results_CDD_LSTM"
input_summary_file = "../data/nab_tuned_summary.csv"
model_summary_file = "../data/sherlock-CDD__LSTM_list.csv"

max_training_ratio = 0.15
threshold_max_multipler = 2
max_training_ratio_buffer = 0.95

input_summary = pandas.read_csv(input_summary_file, index_col="file")
# args end

model_dataframe = pandas.read_csv(model_summary_file, index_col="file")

reg_x = re.compile(r'\.(csv)')
csv_input_files = []
for path, dnames, fnames in os.walk(input_directory):
    csv_input_files.extend([os.path.join(path, f) for f in fnames if reg_x.search(f)])

csv_input_files.sort()

for f in csv_input_files:
    print("Processing " + f)
    # Getting old values
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

    file_name = f.split('/')[-1]

    # Threshold training ratio
    prediction_training_count = 0
    for i in prediction_training:
        if i == 0:
            break
        prediction_training_count += i
    
    ## Training ratio
    first_label_ratio = input_summary['first_label_ratio'][file_name]
    if first_label_ratio < max_training_ratio:
        total_training_length = int(first_label_ratio * max_training_ratio_buffer * len(value))
    else:
        total_training_length = int(max_training_ratio * float(len(value))) ## TODO Fix training ratio issue 
    
    threshold_training_count = total_training_length - prediction_training_count

    if threshold_training_count <= 0:
        print("Cant train threshold !! (lstmcnn input window)sequance length is too large")
        model_dataframe.at[file_name, 'threshold_parameters'] = "Cant train threshold !! (lstmcnn input window)sequance length is too large"
        continue

    threshold_training_ratio = float(threshold_training_count)/float(len(value) - prediction_training_count)

    threshold_max = -1
    for i in range(prediction_training_count, prediction_training_count+threshold_training_count):
        threshold_training[i] = 1
        if threshold_max < warp_distance[i]:
            threshold_max = warp_distance[i]

    threshold = threshold_max_multipler*threshold_max

    for i in range(prediction_training_count+threshold_training_count, len(positive_detection)):
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
    
    newDataFrame.to_csv(fout)

    
    # Calculating confusion metrics
    metrics = confusion_metrics.confusion_metrics(label=label, positive_detection=positive_detection, prediction_training=prediction_training, threshold_training=threshold_training)
    metrics.calculate_metrics()

    model_dataframe.at[file_name, 'TP'] = metrics.get_TP()
    model_dataframe.at[file_name, 'TN'] = metrics.get_TN()
    model_dataframe.at[file_name, 'FP'] = metrics.get_FP()
    model_dataframe.at[file_name, 'FN'] = metrics.get_FN()
    # Setting new threshold_max_multipler in threshold_parameters
    if type(model_dataframe.at[file_name, 'threshold_parameters']) is str :
        threshold_parameter_list = model_dataframe.at[file_name, 'threshold_parameters'].split(";")
        threshold_parameters = ""
        for s in threshold_parameter_list:
                setting_max_multiplier = False
                if "training_ratio" in s:
                        if "training_ratio=" == s[:15]:
                                threshold_parameters += "training_ratio="+str(threshold_training_ratio)+";"
                                setting_max_multiplier = True
                if "threshold_max_multipler" in s:
                        if "threshold_max_multipler=" == s[:24]:
                                threshold_parameters += "threshold_max_multipler="+str(threshold_max_multipler)+";"
                                setting_max_multiplier = True
                if not setting_max_multiplier and not s == "":
                        threshold_parameters += s+";"
        
        model_dataframe.at[file_name, 'threshold_parameters'] = threshold_parameters
    print("Done " + fout)

print("Done !!")