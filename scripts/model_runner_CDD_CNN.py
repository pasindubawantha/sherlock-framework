import json
import pandas
import math
import sys
import os
import numpy as np
import re
import shutil
import helpers
import confusion_metrics

# args
sherlock = "../debug/src/SherlockCDDCNN"
input_dir = "../data/nab_tuned/"
# input_dir = "../data/test/"
input_summary_file = "../data/nab_tuned_summary.csv"
output_dir = "../data/nab_tuned_results_CDD_CNN/"
# output_dir = "../data/test_results/"
model_summary_file = "../data/sherlock-CDD__CNN_list.csv"
nan_folder = "../data/nab_nan/CDD_CNN/"
k
max_training_ratio = 0.15
prediction_training_ratio_fraction = 0.75
threshold_max_multipler = 2
max_training_ratio_buffer = 0.95

input_summary = pandas.read_csv(input_summary_file, index_col="file")

model_summary = []
model_summary.append("file,mse,TP,FP,FN,TN,parameters,threshold_parameters,no_of_anomalies,first_label,length,first_label_ratio")
helpers.sherlock_dump_summary(model_summary, model_summary_file)

reg_x = re.compile(r'\.(csv)')
csv_input_files = []
for path, dnames, fnames in os.walk(input_dir):
    csv_input_files.extend([os.path.join(path, f) for f in fnames if reg_x.search(f)])

csv_input_files.sort()

try:
    shutil.rmtree(output_dir)
    shutil.rmtree(nan_folder)
except OSError:
    print("No previous ", output_dir)

os.mkdir(output_dir)
os.mkdir(nan_folder)

file_count = 0
for f in csv_input_files:
    print("####### Running Sherlock : " + f)
    fconfig = f[:-3] + "json"
    fout = output_dir + f.split('/')[-1]

    shutil.copyfile(fconfig, output_dir + fconfig.split('/')[-1])

    # Training ratio
    dataframe = pandas.read_csv(f)
    value = np.array(dataframe['value'])
    timestamp = np.array(dataframe['timestamp'])
    label = np.array(dataframe['label'])

    jsonf = open(fconfig, "r")
    jsond = json.load(jsonf)
    sequance_length = int(jsond['prediction_model']['model']['CNN']['matWidth']) * int(jsond['prediction_model']['model']['CNN']['matHeight'])

    fname = f.split("/")[-1]
    first_label_ratio = input_summary['first_label_ratio'][fname]

    ## Prediction training ratio
    if first_label_ratio < max_training_ratio:
        prediction_training_ratio = first_label_ratio*max_training_ratio_buffer*prediction_training_ratio_fraction
    else:
        prediction_training_ratio = max_training_ratio*prediction_training_ratio_fraction
    
    if int(prediction_training_ratio*len(value)) +3 < sequance_length:
        prediction_training_ratio = float(sequance_length + 3) / float(len(value))
    
    ## Threshold training ratio
    if first_label_ratio < max_training_ratio:
        total_training_length = int(first_label_ratio * max_training_ratio_buffer * len(value))
    else:
        total_training_length = int(max_training_ratio * len(value)) ## TODO Fix training ratio issue 
    
    threshold_training_count = total_training_length - (int(prediction_training_ratio*len(value)))

    if threshold_training_count <= 0:
        print("Cant train threshold !! (lstmcnn input window)sequance length is too large")
        model_summary.append(fname+",-,-,-,-,-,-,Cant train threshold !! (lstmcnn input window)sequance length is too large,"+str(input_summary['no_of_anomalies'][fname])+","+str(input_summary['first_label'][fname])+","+str(input_summary['length'][fname])+","+str(input_summary['first_label_ratio'][fname]))
        helpers.sherlock_dump_summary(model_summary, model_summary_file)
        continue

    threshold_training_ratio = float(threshold_training_count) / float(len(value) - (int(prediction_training_ratio*len(value))))

    args = f+" "+fout+" "+fconfig+" "+str(prediction_training_ratio)+" "+str(threshold_training_ratio)+" "+str(threshold_max_multipler)

    print(sherlock + " " + args)
    task_output = os.popen(sherlock + " " + args).read()
    print(task_output)

    print("####### Post processing : " + f)

    # TODO Read the fout and calculate mse,TP,FP,FN,TN,
    dataframe = pandas.read_csv(fout)
    prediction_training = np.array(dataframe['prediction_training'])
    prediction = np.array(dataframe['prediction'])
    warp_distance = np.array(dataframe['warp_distance'])
    threshold_training = np.array(dataframe['threshold_training'])
    distance_threshold = np.array(dataframe['distance_threshold'])
    positive_detection = np.array(dataframe['positive_detection'])

            ## Check if its a nan generating file
    if np.isnan(prediction[-1]):
        print("file with nan")
        shutil.copyfile(fconfig, nan_folder + fconfig.split('/')[-1])
        shutil.copyfile(f, nan_folder + f.split('/')[-1])
        model_summary.append(fname+"nan,nan,nan,nan,nan,nan,nan,nan,nan,nan,nan")
        helpers.sherlock_dump_summary(model_summary, model_summary_file)
        continue
    
    prediction_training_count = 0
    for i in prediction_training:
        if i == 0:
            break
        prediction_training_count += 1

    mse = helpers.MSE(value[prediction_training_count:], prediction[prediction_training_count:])

    metrics = confusion_metrics.confusion_metrics(label=label, positive_detection=positive_detection, prediction_training=prediction_training, threshold_training=threshold_training)
    metrics.calculate_metrics()

    TP = metrics.get_TP()
    TN = metrics.get_TN()
    FP = metrics.get_FP()
    FN = metrics.get_FN()

    # Getting Params to write

    ## prediction params 
    prediction_params = "lstmWeight="+str(jsond['prediction_model']['model']['lstmW'])+";"
    prediction_params += "cnnWeight="+str(jsond['prediction_model']['model']['cnnW'])+";"
    prediction_params += "lstmCells="+str(jsond['prediction_model']['model']['LSTM']['memCells'])+";"
    prediction_params += "CL1filters="+str(jsond['prediction_model']['model']['CNN']['ConvolutionLayers'][0]['filters'])+";"
    prediction_params += "CL1kernal_size="+str(jsond['prediction_model']['model']['CNN']['ConvolutionLayers'][0]['filterSize'])+";"
    prediction_params += "CL1strides="+str(jsond['prediction_model']['model']['CNN']['ConvolutionLayers'][0]['stride'])+";"
    prediction_params += "PL1pool_size="+str(1)+";"
    prediction_params += "CNNDL1units="+str(jsond['prediction_model']['model']['CNN']['FullyConnectedLayers'][0]['outputs'])+";"
    prediction_params += "CNNDL2units="+str(jsond['prediction_model']['model']['CNN']['FullyConnectedLayers'][1]['outputs'])+";"
    prediction_params += "CNNDL3units="+str(jsond['prediction_model']['model']['CNN']['FullyConnectedLayers'][2]['outputs'])+";"
    prediction_params += "epochs="+str(jsond['prediction_model']['trainingIterations'])+";"
    prediction_params += "sequance_length="+str(sequance_length)+";"
    prediction_params += "training_ratio="+str(prediction_training_ratio)

    ## threshold params 
    threshold_params = "comparision_window_size="+str(jsond['dtw_window'])+";"
    threshold_params += "threshold_max_multipler="+str(threshold_max_multipler)+";"
    threshold_params += "training_ratio="+str(threshold_training_ratio)+";"

    ## Update model summary
    model_summary_row = fname+","
    model_summary_row += str(mse)+","
    model_summary_row += str(TP)+","
    model_summary_row += str(FP)+","
    model_summary_row += str(FN)+","
    model_summary_row += str(TN)+","
    model_summary_row += prediction_params+","
    model_summary_row += threshold_params+","
    model_summary_row += str(input_summary['no_of_anomalies'][fname])+","
    model_summary_row += str(input_summary['first_label'][fname])+","
    model_summary_row += str(input_summary['length'][fname])+","
    model_summary_row += str(input_summary['first_label_ratio'][fname])
    model_summary.append(model_summary_row)
    helpers.sherlock_dump_summary(model_summary, model_summary_file)

    file_count += 1
    print("+++++++++++++++ Processed " +str(file_count) + " of " + str(len(csv_input_files)))

print("Done !!")