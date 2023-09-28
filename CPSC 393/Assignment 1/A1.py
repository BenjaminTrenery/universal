from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

#puts iris dataset into a dataframe
myDataset = pd.read_csv("iris.csv")

#changes species column to where a plant that is a iris-setosa is given a 1 value and a plant that is not-iris-setosa and 0 value
myDataset = myDataset.replace({"Species": {"Iris-setosa": 1, "Not-Iris-setosa": 0}})

#array of the names of the columns and create a data frame where X contains on the features and y the species of the plant
predictors = ["SepalLengthCm", "SepalWidthCm", "PetalLengthCm", "PetalWidthCm"]
X = myDataset[predictors]
y = myDataset["Species"]

#splits data into train and test data and creates svm model
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.1, random_state = 1234)
myClassifier = svm.SVC(kernel = 'linear')
myClassifier = myClassifier.fit(X_train, y_train)
y_pred = myClassifier.predict(X_test)

#calculates the metrics of accuracy, precision and recall based on the test data and the predicted values
print("Accuracy: ", accuracy_score(y_test, y_pred))
print("Precision: ", precision_score(y_test, y_pred))
print("Recall: ", recall_score(y_test, y_pred))


#function for calculating different y_points based on the different line equations
def yPoints(tryPoints, x_points, w, b):
    
    if(tryPoints == 0):
        y_points = - ((w[1] / w[0]) * x_points) - ((w[2] / w[0]) * x_points) - ((w[3] / w[0]) * x_points) - (b / w[0])  
    elif(tryPoints == 1):
        y_points = - ((w[0] / w[1]) * x_points) - ((w[2] / w[1]) * x_points) - ((w[3] / w[1]) * x_points) - (b / w[1])
    elif(tryPoints == 2):
        y_points = - ((w[0] / w[2]) * x_points) - ((w[1] / w[2]) * x_points) - ((w[3] / w[2]) * x_points) - (b / w[2])
    elif(tryPoints == 3):
        y_points = - ((w[0] / w[3]) * x_points) - ((w[1] / w[3]) * x_points) - ((w[2] / w[3]) * x_points) - (b / w[3])
        
    return y_points

#plots the svm model into a plt plot 
def plotSVM(preOne, preTwo, tryPoints, myClassifier):
    
    #size of the plot
    plt.figure(figsize = (10, 8))
    
    #puts the data points based on the features
    sns.scatterplot(x=X_train.iloc[:, preOne], 
                    y=X_train.iloc[:, preTwo], 
                    hue=y_train, 
                    s=8)

    # finds the slope and intercept of the model
    w = myClassifier.coef_[0]
    b = myClassifier.intercept_[0]

    #find the range of x value points
    x_min, x_max = X_train.iloc[:, preOne].min() - 1, X_train.iloc[:, preOne].max() + 1

    x_points = np.linspace(x_min, x_max)
    
    #uses the yPoints function to calculate the y points
    y_points = yPoints(tryPoints, x_points, w, b)

    plt.plot(x_points, y_points, c = 'r')
    
    # Encircle support vectors
    plt.scatter(myClassifier.support_vectors_[:, preOne],
                myClassifier.support_vectors_[:, preTwo], 
                s=50, 
                facecolors='none', 
                edgecolors='k', 
                alpha=.5)
    
    #gets the unit vector of just two features that are being used
    myUVector = [myClassifier.coef_[0][preOne], myClassifier.coef_[0][preTwo]]
    # Step 2 (unit-vector):
    w_hat = myUVector / (np.sqrt((myClassifier.coef_[0][preOne] ** 2) + (myClassifier.coef_[0][preTwo] ** 2)))
    # Step 3 (margin):
    margin = 1 / (np.sqrt((myClassifier.coef_[0][preOne] ** 2) + (myClassifier.coef_[0][preTwo] ** 2)))
    # Step 4 (calculate points of the margin lines):
    decision_boundary_points = np.array(list(zip(x_points, y_points)))
    
    points_of_line_above = decision_boundary_points + w_hat * margin
    points_of_line_below = decision_boundary_points - w_hat * margin
    # Plot margin lines
    # Blue margin line above
    plt.plot(points_of_line_above[:, 0], 
            points_of_line_above[:, 1], 
            'b--', 
            linewidth=2)
    # Green margin line below
    plt.plot(points_of_line_below[:, 0], 
            points_of_line_below[:, 1], 
            'g--',
            linewidth=2)


    plt.show()
    
plotSVM(2, 0, 1, myClassifier)
plotSVM(2, 1, 1, myClassifier)
plotSVM(2, 3, 3, myClassifier)