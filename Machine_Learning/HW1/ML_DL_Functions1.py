import numpy as np

def LeastSquares(X,y):
  '''
    Calculates the Least squares solution to the problem X*theta=y using the least squares method
    :param X: numpy input matrix, size [N,m+1] (feature 0 is a column of 1 for bias)
    :param y: numpy input vector, size [N]
    :return theta = (Xt*X)^(-1) * Xt * y: numpy output vector, size [m+1]
    N is the number of samples and m is the number of features=28
  '''
  theata = np.linalg.inv(X.T @ X) @ (X.T @ y)
  return theata

def classification_accuracy(model,X,s):
  '''
    calculate the accuracy for the classification problem
    :param model: the classification model class
    :param X: numpy input matrix, size [N,m]
    :param s: numpy input vector of ground truth labels, size [N]
    :return: accuracy of the model = (correct classifications)/(total classifications) type float
    N is the number of samples and m is the number of features=28
  '''
  pred_s = model.predict(X)
  count = 0
  count = np.sum(pred_s == s)
  return count / len(s)

def linear_regression_coeff_submission():
  '''
    copy the values from your notebook into a list in here. make sure the values
    seperated by commas
    :return: list of coefficiants for the linear regression problem. length 28
  '''
  return [-2.43764836e-02,  3.71835851e-02, -3.24112752e-02, -2.30854128e-04,
 -2.82038788e-02, -2.01254179e-02,  5.69928122e-02,  1.98060408e-03,
  1.38065873e-02, -3.06940048e-02,  4.08522654e-02, -4.85840993e-03,
  6.92347768e-02,  1.55387315e-01,  7.66115111e-01,  2.02029655e-02,
  2.75611841e-02,  1.50280340e-02,  3.77801941e-03,  7.32413879e-03,
  3.74606714e-02,  2.00997192e-02,  1.99419324e-02, -3.45149084e-02,
 -5.98459094e-03, -1.28644571e-02, -1.86290935e-02, -4.88727762e-02]

def linear_regression_intrcpt_submission():
  '''
    copy the intercept value from your notebook into here.
    :return: the intercept value. type float
  '''
  return -1.024471460753375e-16

def classification_coeff_submission():
  '''
    copy the values from your notebook into a list in here. make sure the values
    seperated by commas
    :return: list of list of coefficiants for the classification problem.  length 28
  '''
  return [[-0.29330478, -0.0963733,   0.2655859,  -0.12398681, -0.35913508, -0.18918997,
  -0.00908056, -0.05028814,  0.05850141,  0.11993417, -0.53844597, -0.14257523,
  -0.07841036,  0.84560937,  2.77499234, -0.39507254, -0.11237241, -0.10032611,
  -0.14115489, -0.04992495,  0.06849519, -0.00582753,  0.10157649, -0.188081,
  -0.29624615,  0.09044242,  0.12434415,  0.35810888]]

def classification_intrcpt_submission():
  '''
    copy the intercept value from your notebook into here.
    :return: list with the intercept value. length 1
  '''
  return [0.4186804]

def classification_classes_submission():
  '''
    copy the classes values from your notebook into a list in here. make sure the values
    seperated by commas
    :return: list of classes for the classification problem. length 2.
  '''
  return [0, 1]