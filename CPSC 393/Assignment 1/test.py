from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score
from sklearn.datasets import make_classification
from sklearn.svm import SVC
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Creating a random dataset of 2,000 samples and only 2 features
# (for 2–dimensional space). And yeah, it's a binary classification
# here (`y` contains two classes: 0 and 1).
X, y = make_classification(n_samples=2000, n_features=4,
                           n_informative=2, n_redundant=0,
                           n_classes=2,
                           random_state=32)
# Splitting our dataset by train and test parts.
# `stratify` is here to make our splitting balanced
# in terms of classes.
X_train, X_test, y_train, y_test = train_test_split(X, y,
                                   test_size=0.3, stratify=y,
                                   random_state=32)
# And here we train our model. IMPORTANT: we use kernel='linear'.
svc_model = SVC(kernel='linear', random_state=32)
svc_model.fit(X_train, y_train)

plt.figure(figsize=(10, 8))
# Plotting our two-features-space
sns.scatterplot(x=X_train[:, 0], 
                y=X_train[:, 1], 
                hue=y_train, 
                s=8)
# Constructing a hyperplane using a formula.
w = svc_model.coef_[0]          # w consists of 2 elements
b = svc_model.intercept_[0]      # b consists of 1 element

print(w)
print(b)

x_points = np.linspace(-1, 1)    # generating x-points from -1 to 1
y_points = - ((w[0] / w[3]) * x_points) - ((w[1] / w[3]) * x_points) - ((w[2] / w[3]) * x_points) - (b / w[3])  # getting corresponding y-points
# Plotting a red hyperplane
plt.plot(x_points, y_points, c='r')

plt.show()

# print(X_train)
# print(y_train)